#pragma once
#include "stdafx.h"
#include <vector>
enum class GameState
{
	Playing,
	Win,
	Lose
};
constexpr short MINE_VALUE = -1;
struct Cell
{
	bool marked;
	bool opened;
	short neighbours;
	Cell();
};
class Minesweeper
{
public:
	Minesweeper();
	~Minesweeper();
	bool reveal(unsigned int x, unsigned int y);
	void mark(unsigned int x, unsigned int y);
	void generate(unsigned int x, unsigned int y, unsigned int mines_count);
	GameState getState() const { return current_state; }
	Cell getCell(unsigned int x, unsigned int y) const { return cells[y][x]; }
private:
	void reveal_wave(int x, int y);
	void mark_neighbours(unsigned int x, unsigned int y);
	void mark_neighbour_one(unsigned int x, unsigned int y);
private:
	GameState current_state;
	Cell cells[TABLE_ROWS][TABLE_ROWS];
	unsigned int unmarked_mines;
	unsigned int not_mined;
	const unsigned int total_cells;
};

