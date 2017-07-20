#ifndef CCKIT_GRAPH_H
#define CCKIT_GRAPH_H

#include "../internal/config.h"
#include "../list.h"
#include "../queue.h"
#include "../algorithm.h"
#include <unordered_map>

namespace cckit
{
	template<typename T>
	class adjacency_list;

	template<typename T>
	class adjacency_list_node
	{
		typedef adjacency_list_node<T> this_type;
	public:
		typedef T value_type;

	private:
		explicit adjacency_list_node(const value_type& _val = value_type());
	public:
		const value_type& value() const;
		void set_value(const value_type& _val);

	private:
		std::unordered_map<this_type*, int> mAdjacencyMap;
		value_type mVal;
		this_type* mpPredecessor;
		bool mVisited;
		int mDistance;

		template<typename T>
		friend class adjacency_list;
	};

	template<typename T>
	class adjacency_list
	{
		typedef adjacency_list<T> this_type;
	public:
		typedef T value_type;
		typedef adjacency_list_node<T> node_type;

	public:
		adjacency_list();
		~adjacency_list();

		node_type* add_vertex(const value_type& _val);
		void set_edge(node_type* _head, node_type* _tail, int _weight);
		bool is_edge(node_type* _head, node_type* _tail) const;

		template<typename UnaryFunction>
		void bfsearch(node_type* _src, int _range, UnaryFunction _func) const;
		template<typename UnaryFunction0, typename UnaryFunction1>
		void bfsearch(node_type* _src, int _range, UnaryFunction0 _func0, UnaryFunction1 _func1) const;

	private:
		list<node_type*> mVertices;
	};
}

namespace cckit
{
	template<typename T>
	inline adjacency_list_node<T>::adjacency_list_node(const value_type& _val)
		: mAdjacencyMap(), mVal(_val), mpPredecessor(nullptr), mVisited(false), mDistance(0)
	{}

	template<typename T>
	inline const typename adjacency_list_node<T>::value_type& 
		adjacency_list_node<T>::value() const
	{
		return mVal;
	}

	template<typename T>
	inline void adjacency_list_node<T>::set_value(const value_type& _val)
	{
		mVal = _val;
	}
	
	template<typename T>
	inline adjacency_list<T>::adjacency_list()
		: mVertices()
	{}

	template<typename T>
	inline adjacency_list<T>::~adjacency_list()
	{
		auto current = mVertices.cbegin();
		auto end = mVertices.cend();
		for (auto next = current; current != end; current = next) {
			++next;
			delete *current;
		}
	}

	template<typename T>
	inline typename adjacency_list<T>::node_type*
		adjacency_list<T>::add_vertex(const value_type& _val)
	{
		mVertices.push_back(new node_type(_val));
		return mVertices.back();
	}

	template<typename T>
	inline void adjacency_list<T>::set_edge(node_type* _head, node_type* _tail, int _weight)
	{
		_head->mAdjacencyMap[_tail] = _weight;
	}

	template<typename T>
	inline bool adjacency_list<T>::is_edge(node_type* _head, node_type* _tail) const
	{
		return _head->mAdjacencyMap[_tail] != -1;
	}

	template<typename T>
	template<typename UnaryFunction>
	void adjacency_list<T>::bfsearch(node_type* _src, int _range, UnaryFunction _func) const
	{
		bfsearch(_src, _range, [](node_type*) {}, _func);
	}

	template<typename T>
	template<typename UnaryFunction0, typename UnaryFunction1>
	void adjacency_list<T>::bfsearch(node_type* _src, int _range, UnaryFunction0 _func0, UnaryFunction1 _func1) const
	{
		queue<node_type*> queue;

		queue.push(_src);
		cckit::for_each(mVertices.begin(), mVertices.cend(), [](node_type* _arg) {
			_arg->mVisited = false;
			_arg->mDistance = 0;
		});
		_src->mVisited = true;
		_func0(_src);

		do {
			node_type* currentNode = queue.front();
			queue.pop();
			if (currentNode->mDistance >= _range) return;

			auto end = currentNode->mAdjacencyMap.end();
			for (auto current = currentNode->mAdjacencyMap.begin(); current != end; ++current) {
				auto adjacentNode = current->first;
				if (!adjacentNode->mVisited && is_edge(currentNode, adjacentNode)) {
					adjacentNode->mpPredecessor = currentNode;
					adjacentNode->mVisited = true;
					adjacentNode->mDistance = currentNode->mDistance + 1;
					queue.push(adjacentNode);

					_func1(adjacentNode);
				}
			}
		} while (!queue.empty());
	}
}
#endif // !CCKIT_GRAPH_H