#include "Character.h"

using namespace Graph_lib;

const int grid_width = 8;
const int grid_height = 8;

Character::Character(Graph_lib::Window* win, int r, int c)
	: my_window{ win }, pos_row{ r }, pos_col{ c }
{
}

void Character::move()
{
	pair<int, int> dir = nextDirection();

	int dr = dir.first;
	int dc = dir.second;

	pos_row += dr;
	pos_col += dc;

	int w = my_window->x_max() / grid_width;
	int h = my_window->y_max() / grid_height;

	for (Shape* s : body_shapes)
	{
		s->Shape::move(w*dc, h*dr);
	}
}

pair<int, int> Character::nextDirection()
{
	return make_pair(0, 0);
}

pair<int, int> Pursuer::nextDirection()
{
	vector<vector<pair<int, int>>> directions; 
	vector<pair<int, int>> helper; //directions에 바로 추가가 안되서 helper를 사용
	vector<pair<int, int>> direction;

	for (int i = 0; i < numOfEnemies(); i++)
	{
		if (row() >= enemies[i]->row() && col() >= enemies[i]->col())
		{
			helper = { {-1,0}, {0,-1} };
			directions.push_back(helper);
		}
		else if (row() >= enemies[i]->row() && col() <= enemies[i]->col())
		{
			helper = { { 0,1 },{ -1,0 } };
			directions.push_back(helper);
		}

		else if (row() <= enemies[i]->row() && col() >= enemies[i]->col())
		{
			helper = { { 0,-1 },{ 1,0 } };
			directions.push_back(helper);
		}
		else
		{
			helper = { { 1,0 },{ 0,1 } };
			directions.push_back(helper);
		}
	}

	if (directions.size() == 1) //enemies가 1개일 경우
	{
		direction = directions[0];
	}
	else
	{
		for (int i = 0; i < directions.size()-1; i++)
		{
			for (int j = i + 1; j < directions.size(); j++)
			{
				for (int p = 0; p < directions[i].size(); p++)
				{
					for (int q = 0; q < directions[j].size(); q++)
					{
						if (directions[i][p] == directions[j][q])
							direction.push_back(directions[i][p]);
					}
				}
			}
		}
	}
	if (direction.size() == 0)
		direction = { {1,0}, {0,1}, {-1,0}, {0,-1} };
	std::random_shuffle(direction.begin(), direction.end());
	int dr = 0, dc = 0, new_r = 0, new_c = 0;
	for (int i = 0; i < direction.size(); i++)
	{
		dr = direction[i].first;
		dc = direction[i].second;
		new_r = pos_row + dr;
		new_c = pos_col + dc;

		if (new_r >= 0 && new_r < grid_height &&
			new_c >= 0 && new_c < grid_width)
			return direction[i];
	}
}

pair<int, int> Evader::nextDirection()
{
	vector<vector<pair<int, int>>> directions;
	vector<pair<int, int>> helper; //directions에 바로 추가가 안되서 helper를 사용
	vector<pair<int, int>> direction;

	for (int i = 0; i < numOfEnemies(); i++)
	{
		if (row() >= enemies[i]->row() && col() >= enemies[i]->col())
		{
			helper = { { 1,0 },{ 0,1 }, {-1,0}, {0,-1}, { 1,0 },{ 0,1 } }; //도망갈 확률을 높임
			directions.push_back(helper);
		}
		else if (row() >= enemies[i]->row() && col() <= enemies[i]->col())
		{
			helper = { { 0,-1 },{ 1,0 }, {-1,0}, {0,1},{ 0,-1 },{ 1,0 } };
			directions.push_back(helper);
		}

		else if (row() <= enemies[i]->row() && col() >= enemies[i]->col())
		{
			helper = { { 0,1 },{ -1,0 }, {1,0}, {0,-1},{ 0,1 },{ -1,0 } };
			directions.push_back(helper);
		}
		else
		{
			helper = { { -1,0 },{ 0,-1 },{1,0}, {0,1},{ -1,0 },{ 0,-1 } };
			directions.push_back(helper);
		}
	}

	if (directions.size() == 0) //enemies가 1개일 경우
	{
		direction = directions[0];
	}
	else
	{
		for (int i = 0; i < directions.size(); i++) 
		{
			for (int j = i+1; j < directions.size(); j++)
			{
				for (int p = 0; p < directions[i].size(); p++)
				{
					for (int q = 0; q < directions[j].size(); q++)
					{
						if (directions[i][p] == directions[j][q])
							direction.push_back(directions[i][p]);
					}
				}
			}
		}
	}

	if (direction.size() == 0)
		direction = { { 1,0 },{ 0,1 },{ -1,0 },{ 0,-1 } };
	std::random_shuffle(direction.begin(), direction.end());
	int dr = 0, dc = 0, new_r = 0, new_c = 0;
	for (int i = 0; i < direction.size(); i++)
	{
		dr = direction[i].first;
		dc = direction[i].second;
		new_r = pos_row + dr;
		new_c = pos_col + dc;

		if (new_r >= 0 && new_r < grid_height &&
			new_c >= 0 && new_c < grid_width)
			return direction[i];
	}
}

Pursuer::Pursuer(Graph_lib::Window * win, int r, int c)
	: Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;

	Graph_lib::Ellipse* head = new Graph_lib::Ellipse(Point(c*w + w / 2, r*h + h / 2), 20, 20);
	head->set_color(Color::dark_magenta);
	head->set_fill_color(Color::dark_magenta);
	body_shapes.push_back(head);
	win->attach(*head);

	Graph_lib::Star* eye1 = new Graph_lib::Star(Point(c*w + w / 2 - 7, r*h + h / 2), 5, 2, 5);
	eye1->set_color(Color::red);
	eye1->set_fill_color(Color::red);
	body_shapes.push_back(eye1);
	win->attach(*eye1);
	
	Graph_lib::Star* eye2 = new Graph_lib::Star(Point(c*w + w / 2 + 7, r*h + h / 2), 5, 2, 5);
	eye2->set_color(Color::red);
	eye2->set_fill_color(Color::red);
	body_shapes.push_back(eye2);
	win->attach(*eye2);

	Graph_lib::Rectangle* body = new Graph_lib::Rectangle(Point(c*w + w / 2 - 20, r*h + h / 2 + 20), 40, 50);
	body->set_color(Color::black);
	body->set_fill_color(Color::black);
	body_shapes.push_back(body);
	win->attach(*body);

	Graph_lib::Closed_polyline* weapon1 = new Graph_lib::Closed_polyline();
	weapon1->add(Point(c*w + w / 2 + 20, r*h + h / 2 + 40)); weapon1->add(Point(c*w + w / 2 + 30, r*h + h / 2 + 20));
	weapon1->add(Point(c*w + w / 2 + 40, r*h + h / 2 + 10)); weapon1->add(Point(c*w + w / 2 + 23, r*h + h / 2 + 45));
	weapon1->set_color(Color::red); 
	weapon1->set_fill_color(Color::red);
	body_shapes.push_back(weapon1);
	win->attach(*weapon1);

	Graph_lib::Closed_polyline* weapon2 = new Graph_lib::Closed_polyline();
	weapon2->add(Point(c*w + w / 2 - 22, r*h + h / 2 + 40)); weapon2->add(Point(c*w + w / 2 - 35, r*h + h / 2 + 40));
	weapon2->add(Point(c*w + w / 2 - 48, r*h + h / 2 + 44)); weapon2->add(Point(c*w + w / 2 - 22, r*h + h / 2 + 44));
	weapon2->set_color(Color::red);
	weapon2->set_fill_color(Color::red);
	body_shapes.push_back(weapon2);
	win->attach(*weapon2);

	Graph_lib::Ellipse* leg = new Graph_lib::Ellipse(Point(c*w + w / 2, r*h + h / 2 + 70), 10, 10);
	leg->set_color(Color::black);
	leg->set_fill_color(Color::black);
	body_shapes.push_back(leg);
	win->attach(*leg);
}


Evader::Evader(Graph_lib::Window * win, int r, int c)
	: Character(win, r, c)
{
	int w = win->x_max() / grid_width;
	int h = win->y_max() / grid_height;

	Graph_lib::Ellipse* head = new Graph_lib::Ellipse(Point(c*w + w / 2, r*h + h / 2), 20, 20);
	head->set_color(Color::dark_yellow);
	head->set_fill_color(Color::dark_yellow);
	body_shapes.push_back(head);
	win->attach(*head);

	Graph_lib::Star* eye1 = new Graph_lib::Star(Point(c*w + w / 2 - 7, r*h + h / 2), 5, 2, 5);
	eye1->set_color(Color::green);
	eye1->set_fill_color(Color::green);
	body_shapes.push_back(eye1);
	win->attach(*eye1);

	Graph_lib::Star* eye2 = new Graph_lib::Star(Point(c*w + w / 2 + 7, r*h + h / 2), 5, 2, 5);
	eye2->set_color(Color::green);
	eye2->set_fill_color(Color::green);
	body_shapes.push_back(eye2);
	win->attach(*eye2);

	Graph_lib::Rectangle* body = new Graph_lib::Rectangle(Point(c*w + w / 2 - 20, r*h + h / 2 + 20), 40, 50);
	body->set_color(Color::dark_blue);
	body->set_fill_color(Color::dark_blue);
	body_shapes.push_back(body);
	win->attach(*body);
}