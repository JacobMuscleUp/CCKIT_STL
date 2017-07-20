#ifndef CCKIT_CSV_MAP_H
#define CCKIT_CSV_MAP_H

#include <fstream>
#include <sstream>
#include <vector>
#include <string>
#include "../internal/config.h"
#include "graph.h"

namespace cckit
{
	struct csv_map
	{
		int rows, cols;
		adjacency_list<std::string> graph;
		std::vector<std::vector<adjacency_list_node<std::string>*> > nodes;
		bool initialized;

		csv_map(const char* _fileName);

		void search(int _row, int _col, int _range);
		void write(const char* _fileName);
	};
}

namespace cckit
{
	csv_map::csv_map(const char* _fileName)
		: rows(0), cols(0), graph(), nodes(), initialized(false)
	{
		std::ifstream ifs0(_fileName); if (!ifs0.is_open()) return; initialized = true;
		std::string str0;

		for (int cols0 = 0; std::getline(ifs0, str0); ++rows, cols0 = 0) {
			nodes.push_back(decltype(nodes)::value_type());
			std::istringstream iss0(str0);
			for (; std::getline(iss0, str0, ','); ++cols0, nodes[rows].push_back(graph.add_vertex(str0)));
			if (cols < cols0)
				cols = cols0;
		}
		for (int row = 0; row < rows; ++row)
			for (int col = nodes[row].size(); col < cols; ++col, nodes[row].push_back(graph.add_vertex("X")));

		for (int row = 0; row < rows; ++row) {
			for (int col = 0; col < nodes[row].size(); ++col) {
				auto SetEdge
					= [this](adjacency_list_node<std::string>* _head
						, adjacency_list_node<std::string>* _tail) {
					graph.set_edge(_head, _tail, (_tail->value() == "X") ? -1 : 1);
				};
				if (row != 0)
					SetEdge(nodes[row][col], nodes[row - 1][col]);
				if (row < rows - 1)
					SetEdge(nodes[row][col], nodes[row + 1][col]);
				if (col != 0)
					SetEdge(nodes[row][col], nodes[row][col - 1]);
				if (col != nodes[row].size() - 1)
					SetEdge(nodes[row][col], nodes[row][col + 1]);
			}
		}

		ifs0.close();
	}

	void csv_map::search(int _row, int _col, int _range)
	{
		assert(_row < rows && _col < cols);

		graph.bfsearch(nodes[_row][_col], _range, [](cckit::adjacency_list_node<std::string>* _arg) {
			_arg->set_value("S");
		}, [](cckit::adjacency_list_node<std::string>* _arg) {
			_arg->set_value("R");
		});
	}

	void csv_map::write(const char* _fileName)
	{
		std::ofstream ofs0(_fileName); if (!ofs0.is_open()) return;
		for (int row = 0; row < rows; ++row, ofs0 << "\n") {
			for (int col = 0; col < nodes[row].size(); ++col) {
				ofs0 << nodes[row][col]->value() << ",";
			}
		}

		ofs0.close();
	}
}

#endif // !CCKIT_CSV_MAP_H