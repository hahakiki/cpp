#include "Graph.h"
#include "Simple_window.h"

using namespace Graph_lib;

int main(void)
{
	Simple_window win(Point(100, 100), 800, 800, "Canvas");

	Box b1{ Point(50,50), 100, 100, 20 };
	b1.set_color(Color::black);
	win.attach(b1);

	Box b2{ Point(200,50), 50, 200, 10 };
	b2.set_color(Color::red);
	b2.set_fill_color(Color::green);
	win.attach(b2);

	Box b3{ Point(300, 50), 400, 100, 20 };
	b3.set_color(Color::black);
	b3.set_fill_color(Color::red);
	b3.set_style(Line_style::dashdotdot);
	win.attach(b3);

	Box b4{ Point(50, 300), 100, 100, 50 };
	b4.set_color(Color::blue);
	win.attach(b4);

	Box b5{ Point(200,300), 300, 300, 100 };
	b5.set_color(Color::black);
	b5.set_fill_color(Color::cyan);
	b5.set_style(Line_style::dash);
	win.attach(b5);

	win.wait_for_button(); //Vector를 써도 되지만 간단하게 하였다.
	win.detach(b1); win.detach(b2); win.detach(b3); win.detach(b4); win.detach(b5);

	Regular_polygon p1{ Point(80,80), 3, 30 };
	p1.set_color(Color::black);
	win.attach(p1);

	Regular_polygon p2{ Point(200,150), 4, 60 };
	p2.set_color(Color::black);
	p2.set_style(Line_style::dashdotdot);
	win.attach(p2);

	Regular_polygon p3{ Point(200, 300), 5, 50 };
	p3.set_color(Color::black);
	p3.set_fill_color(Color::green);
	win.attach(p3);

	Regular_polygon p4{ Point(400, 100), 6, 70 };
	p4.set_color(Color::red);
	p4.set_style(Line_style::dot);
	win.attach(p4);

	Regular_polygon p5{ Point(500,500), 15, 150 };
	p5.set_color(Color::blue);
	p5.set_fill_color(Color::black);
	win.attach(p5);
	
	win.wait_for_button();
	win.detach(p1); win.detach(p2); win.detach(p3); win.detach(p4); win.detach(p5);

	Star s1{ Point(100,100), 3, 50, 10 };
	s1.set_color(Color::black);
	win.attach(s1);

	Star s2{ Point(200, 100), 6, 80, 60 };
	s2.set_color(Color::red);
	s2.set_style(Line_style::dash);
	win.attach(p2);

	Star s3{ Point(400, 100), 8, 100, 20 };
	s3.set_fill_color(Color::green);
	win.attach(s3);

	Star s4{ Point(200, 400), 20, 150, 10 };
	s4.set_color(Color::blue);
	s4.set_fill_color(Color::black);
	s4.set_style(Line_style::dot);
	win.attach(s4);

	Star s5{ Point(500,500), 10, 150, 50 };
	s5.set_color(Color::red);
	s5.set_fill_color(Color::dark_green);
	win.attach(s5);

	Star s6{ Point(500,300), 5, 100, 50 };
	s6.set_color(Color::black);
	win.attach(s6);

	win.wait_for_button();
}