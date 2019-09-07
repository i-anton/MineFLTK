#include "Minesweeper.h"
#include <queue>
#include "Vector2i.h"
Cell::Cell() :
	marked(false),
	opened(false),
	neighbours(0)
{}

Minesweeper::Minesweeper() :
	current_state(GameState::Playing),
	unmarked_mines(0),
	not_mined(TABLE_ROWS*TABLE_ROWS),
	total_cells(not_mined)
{}

Minesweeper::~Minesweeper()
{}

bool Minesweeper::reveal(unsigned int x, unsigned int y)
{
	if (cells[y][x].marked)
	{
		return true;
	}
	if (cells[y][x].neighbours == MINE_VALUE)
	{
		//fail
		for (size_t y_inn = 0; y_inn < TABLE_ROWS; ++y_inn)
		{
			for (size_t x_inn = 0; x_inn < TABLE_ROWS; x_inn++)
			{
				cells[y_inn][x_inn].opened = true;
			}
		}
		current_state = GameState::Lose;
		return false;
	}
	reveal_wave(x, y);
	if (not_mined == 0)
	{
		for (size_t y_inn = 0; y_inn < TABLE_ROWS; ++y_inn)
		{
			for (size_t x_inn = 0; x_inn < TABLE_ROWS; x_inn++)
			{
				cells[y_inn][x_inn].opened = true;
			}
		}
		current_state = GameState::Win;
	}
	return true;
}

void Minesweeper::mark(unsigned int x, unsigned int y)
{
	cells[y][x].marked = !cells[y][x].marked;
}

void Minesweeper::generate(unsigned int x, unsigned int y, unsigned int mines_count)
{
	unmarked_mines = mines_count;
	not_mined = total_cells - mines_count;
	assert(unmarked_mines > 0);
	assert(not_mined + mines_count > not_mined);
	std::vector<unsigned int> v_not_mined;
	v_not_mined.reserve(total_cells);
	unsigned int exclude_id = x + y * TABLE_ROWS;
	for (size_t i = 0; i < total_cells; ++i)
	{
		if (i != exclude_id)
			v_not_mined.push_back(i);
	}
	for (size_t i = 0; i < mines_count; ++i)
	{
		int idx = rand() % v_not_mined.size();
		int id = v_not_mined[idx];
		int y = id / TABLE_ROWS;
		int x = id - y * TABLE_ROWS;
		assert(x >= 0);
		assert(x < TABLE_ROWS);
		v_not_mined.erase(v_not_mined.begin() + idx);
		cells[y][x].neighbours = MINE_VALUE;
		mark_neighbours(x, y);
	}
}

void Minesweeper::reveal_wave(int x, int y)
{
	std::queue<Vector2i> q;
	q.push(Vector2i(x, y));
	while (!q.empty())
	{
		auto vect = q.front();
		q.pop();
		if (cells[vect.y][vect.x].opened)
			continue;
		not_mined--;
		cells[vect.y][vect.x].opened = true;
		cells[vect.y][vect.x].marked = false;

		if (cells[vect.y][vect.x].neighbours > 0)
			continue;
		int ymax = MIN(vect.y + 1, TABLE_ROWS - 1);
		int ymin = MAX(vect.y - 1, 0);
		int xmin = MAX(vect.x - 1, 0);
		int xmax = MIN(vect.x + 1, TABLE_ROWS - 1);
		for (int y_i = ymin; y_i <= ymax; ++y_i)
		{
			for (int x_i = xmin; x_i <= xmax; ++x_i)
			{
				auto cell = cells[y_i][x_i];
				if (y_i == vect.y && x_i == vect.x) continue;
				if (!cell.opened && cell.neighbours != MINE_VALUE)
					q.push(Vector2i(x_i, y_i));
			}
		}
	}
}

void Minesweeper::mark_neighbours(unsigned int x, unsigned int y)
{
	mark_neighbour_one(x - 1, y - 1);
	mark_neighbour_one(x, y - 1);
	mark_neighbour_one(x + 1, y - 1);
	mark_neighbour_one(x - 1, y);
	mark_neighbour_one(x + 1, y);
	mark_neighbour_one(x - 1, y + 1);
	mark_neighbour_one(x, y + 1);
	mark_neighbour_one(x + 1, y + 1);
}

void Minesweeper::mark_neighbour_one(unsigned int x, unsigned int y)
{
	if (x < 0 || x >= TABLE_ROWS ||
		y < 0 || y >= TABLE_ROWS) return;
	if (cells[y][x].neighbours != MINE_VALUE)
	{
		cells[y][x].neighbours++;
	}
}
