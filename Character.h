#pragma once

#include "Window.h"
#include "Graph.h"

class Character
{
public:
	Character(Graph_lib::Window* win, int r, int c);
	void move();

	void setEnemy(Character* e) { enemies.push_back(e); }
	vector<Character*> enemy() { return enemies; }
	int numOfEnemies() const { return enemies.size(); }

	int row() { return pos_row; }
	int col() { return pos_col; }

protected:
	virtual pair<int, int> nextDirection();

	int pos_row;
	int pos_col;

	Graph_lib::Window* my_window;

	vector<Character*> enemies;
	vector<Graph_lib::Shape*> body_shapes;
};

class Pursuer : public Character
{
public:
	Pursuer(Graph_lib::Window* win, int r, int c);

protected:
	pair<int, int> nextDirection() override;
};

class Evader : public Character
{
public:
	Evader(Graph_lib::Window* win, int r, int c);

protected:
	pair<int, int> nextDirection() override;
};
