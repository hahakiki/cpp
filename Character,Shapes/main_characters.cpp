#include "Graph.h"
#include "Simple_window.h"
#include "Character.h"

using namespace Graph_lib;

int main()
{
	srand(time(nullptr));

	Point tl(100, 100);
	Simple_window win(tl, 800, 800, "Chasing");

	vector<Character*> characters;

	vector<Character*> pursuers;
	vector<Character*> evaders;

	Pursuer* p1 = new Pursuer(&win, 3, 0);
	Pursuer* p2 = new Pursuer(&win, 6, 0);
	Evader* e1 = new Evader(&win, 4, 7);
	Evader* e2 = new Evader(&win, 1, 7);

	p1->setEnemy(e1); p1->setEnemy(e2); //적 지정
	p2->setEnemy(e1); p2->setEnemy(e2);
	e1->setEnemy(p1); e1->setEnemy(p2);
	e2->setEnemy(p1); e2->setEnemy(p2);

	pursuers.push_back(p1); pursuers.push_back(p2); //추적자와 도망자 vector에 각각 지정
	evaders.push_back(e1); evaders.push_back(e2);

	characters.push_back(p1); //캐릭터 지정
	characters.push_back(p2);
	characters.push_back(e1);
	characters.push_back(e2);

	bool Bool = true;

	do
	{
		win.wait_for_button();

		for (Character* ch : characters)
		{
			ch->move();
			
		}
		for (int i = 0; i < pursuers.size(); i++)
		{
			for (int j = 0; j < evaders.size(); j++)
			{
				if (pursuers[i]->row() == evaders[j]->row() && pursuers[i]->col() == evaders[j]->col())
				{
					Graph_lib::Text t{ Point(250,400), "THE END" };
					t.set_font_size(80);
					t.set_color(Color::black);
					win.attach(t);
					win.wait_for_button();
					Bool = false; //break로는 종료되지 않아 bool을 변경하는 방식으로 종료하였다.
				}
			}
		}
	} while (Bool);
}
