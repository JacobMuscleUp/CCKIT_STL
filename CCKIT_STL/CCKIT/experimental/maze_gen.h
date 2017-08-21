#include <set>
#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include "../stack.h"
#include "../queue.h"
#include "../random.h"

using std::endl;

namespace cckit
{
	template<typename T>
	auto sample(const std::vector<T>& _vector)
		-> decltype(_vector[-1]);

	/*
	struct cell;
	struct grid;
	struct distances;
	struct grid_dist;
	*/

	struct cell
	{
		int row, col;
		std::set<cell*> links;
		cell *east, *south, *west, *north;
		std::vector<cell*> neighbors;

		cell() {}

		cell(int _row, int _col)
			: row(_row), col(_col)
			, links() , neighbors()
		{}

		void link(cell* _other, bool _bidir = true)
		{
			links.insert(_other);
			if (_bidir) { _other->link(this, false); }
		}

		void unlink(cell* _other, bool _bidir = true)
		{
			links.erase(_other);
			if (_bidir) { _other->unlink(this, false); }
		}

		bool linked(cell* _other)
		{
			return (links.count(_other) == 1) ? true : false;
		}

		void update_neighbors()
		{
			if (east) { neighbors.push_back(east); }
			if (south) { neighbors.push_back(south); }
			if (west) { neighbors.push_back(west); }
			if (north) { neighbors.push_back(north); }
		}

		std::vector<cell*> unvisited_neighbors() const
		{
			std::vector<cell*> unvisited_neighbors;
			for (size_t i = 0; i < neighbors.size(); ++i) {
				cell* neighbor = neighbors[i];
				if (neighbor->links.empty())
					unvisited_neighbors.push_back(neighbor);
			}

			return unvisited_neighbors;
		}

		std::vector<cell*> visited_neighbors() const
		{
			std::vector<cell*> visited_neighbors;
			for (size_t i = 0; i < neighbors.size(); ++i) {
				cell* neighbor = neighbors[i];
				if (!neighbor->links.empty())
					visited_neighbors.push_back(neighbor);
			}

			return visited_neighbors;
		}
	};

	struct grid
	{
		int rows, cols;
		cell*** cells;

		grid(int _rows = 4, int _cols = 4)
			: rows(_rows), cols(_cols)
		{
			cells = new cell**[rows];
			for (int i = 0; i < rows; ++i) {
				cells[i] = new cell*[cols];
				for (int j = 0; j < cols; ++j) 
					cells[i][j] = new cell(i, j);
			}

			for (int i = 0; i < rows; ++i) 
				for (int j = 0; j < cols; ++j) {
					cell*& c = cells[i][j];
					c->east = at(i, j + 1);
					c->south = at(i + 1, j);
					c->west = at(i, j - 1);
					c->north = at(i - 1, j);
					c->update_neighbors();
				}
		}

		~grid()
		{
			for (int i = 0; i < rows; ++i) {
				for (int j = 0; j < cols; ++j)
					delete cells[i][j];
				delete[] cells[i];
			}
			delete[] cells;
		}

		cell* at(int _row, int _col)
		{
			if (_row < 0 || _row >= rows
				|| _col < 0 || _col >= cols)
				return 0;
			return cells[_row][_col];
		}

		cell* rand_cell()
		{
			return cells[rand_int(0, rows)][rand_int(0, cols)];
			return cells[rand() % rows][rand() % cols];
		}

		virtual std::string cell_str(cell* _cell) const { return "   "; }

		void reset()
		{

		}
	};

	struct distances
	{
		cell* root;
		std::unordered_map<cell*, int> distmap;

		distances(cell* _root)
			: root(_root)
		{
			distmap[root] = 0;
		}
	};

	struct grid_dist : public grid
	{
		distances* dists;

		grid_dist(int _rows = 4, int _cols = 4)
			: grid(_rows, _cols), dists(0)
		{}

		~grid_dist() { if (dists) delete dists; }

		virtual std::string cell_str(cell* _cell) const 
		{ 
			if (dists && dists->distmap.count(_cell)) {
				int dist = dists->distmap[_cell];
				char* dist_str = 0;
				
				if (dist >= 0 && dist < 10) {
					return std::string(" " + std::to_string(dist) + " ");
				}
				else if (dist >= 10 && dist < 100) {
					return std::string(std::to_string(dist) + " ");
				}
				
				delete[] dist_str;
			}
			return std::string("   ");
		}

		void update_dists(cell* _cell)
		{
			if (dists) delete dists;
			dists = new distances(_cell);

			queue<cell*> queue;
			queue.push(_cell);
			cell* current;

			while (!queue.empty()) {
				current = queue.front();
				queue.pop();
				
				for (auto iter = current->neighbors.begin();
					iter != current->neighbors.end(); ++iter) {
					if (!dists->distmap.count(*iter)
						&& current->linked(*iter)) {
						dists->distmap[*iter] = dists->distmap[current] + 1;
						queue.push(*iter);
					}
				}	
			}
		}

		void calc_path(cell* _goal)
		{
			if (dists) {
				if (!dists->distmap.count(_goal)) return;

				cell* current = _goal;
				distances* path = new distances(_goal);
				path->distmap[_goal] = dists->distmap[_goal];
				while (current != dists->root) {
					for (auto iter = current->links.begin();
						iter != current->links.end(); ++iter) {
						if (dists->distmap[*iter] < dists->distmap[current]) {
							path->distmap[*iter] = dists->distmap[*iter];
							current = *iter;
							break;
						}
					}
				}

				delete dists;
				dists = path;
			}
		}
	};

	std::ostream& operator <<(std::ostream& _os, const grid& _grid)
	{
		_os << "+";
		for (int i = 0; i < _grid.cols; ++i) { _os << "---+"; }
		_os << endl;

		for (int i = 0; i < _grid.rows; ++i) {
			std::string top = "|", bottom = "+";
			for (int j = 0; j < _grid.cols; ++j) {
				cell* c = _grid.cells[i][j];

				std::string body = _grid.cell_str(c);
				const char* east_bound = c->linked(c->east) ? " " : "|";
				top += body;
				top += east_bound;

				const char* south_bound = c->linked(c->south) ? "   " : "---";
				bottom += south_bound;
				bottom += "+";
			}
			_os << top << endl << bottom << endl;
		}

		return _os;
	}

	void bintree_mazegen(grid& _grid)
	{
		for (int i = 0; i < _grid.rows; ++i)
			for (int j = 0; j < _grid.cols; ++j) {
				cell* c = _grid.cells[i][j];

				std::vector<cell*> neighbors;
				if (c->east) { neighbors.push_back(c->east); }
				if (c->south) { neighbors.push_back(c->south); }

				if (!neighbors.empty()) {
					int ndx = rand() % neighbors.size();
					c->link(neighbors[ndx]);
				}
			}
	}

	void rec_backtrack_mazegen(grid& _grid)
	{
		cell* current;
		stack<cell*> stack;

		stack.push(_grid.rand_cell());
		do {
			current = stack.top();
			std::vector<cell*> unvisited_neighbors = current->unvisited_neighbors();

			if (unvisited_neighbors.empty())
				stack.pop();
			else {
				cell* unvisited_neighbor = sample(unvisited_neighbors);
				current->link(unvisited_neighbor);
				stack.push(unvisited_neighbor);
			}
		} while (!stack.empty());
	}

	void rec_backtrack_mazegen_2(grid& _grid, cell* _cell)
	{
		std::vector<cell*> unvisited_neighbors = _cell->unvisited_neighbors();
		if (!unvisited_neighbors.empty()) {
			cell* unvisited_neighbor = sample(unvisited_neighbors);
			_cell->link(unvisited_neighbor);
			rec_backtrack_mazegen_2(_grid, unvisited_neighbor);
			rec_backtrack_mazegen_2(_grid, _cell);
		}
	}

	void huntnkill_mazegen(grid& _grid)
	{
		cell* current = _grid.rand_cell();
		while (current) {
			std::vector<cell*> unvisited_neighbors = current->unvisited_neighbors();

			if (!unvisited_neighbors.empty()) {
				cell* unvisited_neighbor = sample(unvisited_neighbors);
				current->link(unvisited_neighbor);
				current = unvisited_neighbor;
			}
			else {
				current = nullptr;

				for (int i = 0; i < _grid.rows; ++i) {
					for (int j = 0; j < _grid.cols; ++j) {
						cell* c = _grid.cells[i][j];
						std::vector<cell*> visited_neighbors = c->visited_neighbors();

						if (c->links.empty() && !visited_neighbors.empty()) {
							cell* visited_neighbor = sample(visited_neighbors);
							c->link(visited_neighbor);
							current = c;
							goto Pt0;
						}
					}
				}	
			Pt0: {}
			}//! else
		}
	}

	template<typename T>
	auto sample(const std::vector<T>& _vector)
		-> decltype(_vector[-1])
	{
		return _vector[rand_int(0, _vector.size())];
	}
}