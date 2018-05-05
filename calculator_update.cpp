/*
	calculator_buggy.cpp
*/

#include "std_lib_facilities.h"

struct Token {
	char kind;
	double value;
	string name;
	Token(char ch) :kind(ch), value(0) { }
	Token(char ch, double val) :kind(ch), value(val) { }
	Token(char ch, string n) :kind(ch), name(n) { }
};

class Token_stream {
	bool full;
	Token buffer;
public:
	Token_stream() :full(0), buffer(0) { }

	Token get();
	void putback(Token t) { buffer=t; full=true; }

	void ignore(char);
};

const char let = 'L';
const char quit = 'Q';
const char print = ';';
const char number = '8';
const char name = 'a';

const char user_sqrt = 'r'; //sqrt,sin,cos,tan �� �Է� �� ��� ������ ���� ����ϱ� ���� 
const char user_sin = 's'; //switch���� ����� ���� ����.
const char user_cos = 'c'; //user_�� ��� �̸��� ����ϱ⿡ ���� �Լ� �̸��� ���� �ٿ���.
const char user_tan = 't';

const char salary_time = 'p'; //���� ����� ���� �ð�
const int basic_salary = 7530; //���� �ñ�

Token Token_stream::get()
{
	if (full) { full=false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '!': // !���ڸ� �ν�
		return Token(ch);
	case '(':
	case ')':
	case '+':
	case '-':
	case '*':
	case '/':
	case '%':
	case ';':
	case '=':
	case '^': //n������ ���� '^'�߰�
		return Token(ch);
	case '.':
	case '0':
	case '1':
	case '2':
	case '3':
	case '4':
	case '5':
	case '6':
	case '7':
	case '8':
	case '9':
	{	
		cin.unget();
		double val;
		cin >> val;
		return Token(number,val);
	}
	default:
		if (isalpha(ch)) {
			string s;
			s += ch;
			while(cin.get(ch) && (isalpha(ch) || isdigit(ch))) s += ch;
			cin.unget();
			if (s == "let") return Token(let);	
			if (s == "quit") return Token(quit);

			if (s == "sqrt") return Token(user_sqrt); //sqrt, sin, cos, tan ������ ���� �Է��� �� ��
			if (s == "sin") return Token(user_sin); //�ش��ϴ� ��ū���� �����Ͽ���.
			if (s == "cos") return Token(user_cos);
			if (s == "tan") return Token(user_tan);

			if (s == "salarytime") return Token(salary_time); //salarytime�� �Էµ� ��� �ش��ϴ� ��ū���� �����Ѵ�.

			return Token(name,s);
		}
		error("Bad token");
	}
}

void Token_stream::ignore(char c)
{
	if (full && c==buffer.kind) {
		full = false;
		return;
	}
	full = false;

	char ch;
	while (cin>>ch)
		if (ch==c) return;
}

struct Variable {
	string name;
	double value;
	Variable(string n, double v) :name(n), value(v) { }
};

vector<Variable> names;

double get_value(string s)
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return names[i].value;
	error("get: undefined name ",s);
}

void set_value(string s, double d)
{
	for (int i = 0; i<=names.size(); ++i)
		if (names[i].name == s) {
			names[i].value = d;
			return;
		}
	error("set: undefined name ",s);
}

bool is_declared(string s)
{
	for (int i = 0; i<names.size(); ++i)
		if (names[i].name == s) return true;
	return false;
}

Token_stream ts;

double expression();

int Factorial(int value) //factorial�� ����ϴ� �Լ��̴�.
{
	if (value == 0)
	{
		return 1;
	}
	return value*Factorial(value - 1);
}

double primary()
{
	Token t = ts.get();
	Token t2 = ts.get(); //factorial�� �ν��ϱ����� Token t ���� �ϳ��� ��ū�� �� ����

	if (t2.kind == '!') //�ι�° Token�� !(factorial)�� ��� Factorial() ȣ���� ���� t�� ���� �����Ѵ�.
	{
		t.value = Factorial(t.value);
	}
	else ts.putback(t2); //!�� �ƴҰ�� �ٽ� �ִ´�.

	switch (t.kind) {
	case '(':
	{	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("'(' expected");
		else return d; //��ȣ �� ��� �� ���� ���� ��ȯ�Ѵ�.(logical error) 
	}
	case '-':
		return - primary();
	case '+':
		return primary(); //+���� �� ��츦 �ݿ��ϱ� ���� primary()�� return�Ѵ�. (extend unary plus)
	case number:
		return t.value;
	case name:
		return get_value(t.name);

	case user_sqrt: //������ ���� expression()�Լ��� ȣ���� ��ȣ ���� ���� �˾Ƴ��� �˸°� ����� ��ȯ�Ͽ���.
	{
		double d = expression();
		if (d < 0) //������ �Էµ� ��� error()�� ����Ͽ���.
			error("error: negative integer"); //�����޽���
		return sqrt(d);
	}
	case user_sin:
	{
		double d = expression();
		return sin(d);
	}
	case user_cos:
	{
		double d = expression();
		return cos(d);
	}
	case user_tan:
	{
		double d = expression();
		return tan(d);
	}

	case salary_time: //�����ñ� ���
	{
		double d = expression();
		if (d < 1) error("0 Ȥ�� �������� �Է��ϼ̽��ϴ�.");
		return d * basic_salary;
	}

	default:
		error("primary expected");
	}
}


double mod(int left, int primary) //������ ������ �ϴ� ��� ���Ǵ� �Լ� (optional)
{
	if (left < primary)
	{
		error("������ ��Ȯ���� Ȯ�����ּ���."); //���̹� ����� ���� ȿ���� �´�.
	}
	return left % primary;
}

double n_ary_square(double num, int n) // n���� ��ȯ �Լ�
{
	double result = 1;
	for (int i = 0; i < n; i++)
	{
		result *= num;
	}
	return result;
}

double term()
{
	double left = primary();
	while(true) {
		Token t = ts.get();

		switch(t.kind) {
		case '^': //n ������ �ϱ� ���� case
			left = n_ary_square(left, primary());
			break;
		case '%': //������ ����(optional)
			left = mod(left, primary());
			cout << "������ ������ ��� �Ǽ��� �Է��Ͻø� int�� �ڵ� ��ȯ�˴ϴ�." << endl; //�ڵ� ���� ��ȯ�� �˷���
			break;
		case '*':
			left *= primary();
			break;
		case '/':
		{	double d = primary();
			if (d == 0) error("divide by zero");
			left /= d;
			break;
		}
		default:
			ts.putback(t);
			return left;
		}
	}
}

double expression()
{
	double left = term();
	while(true) {
		Token t = ts.get();
		switch(t.kind) {
		case '+':
			left += term();
			break;
		case '-':
			left -= term();
			break;
		default:
			ts.putback(t);
			return left;
		}
	}
}

double declaration()
{
	Token t = ts.get();
	if (t.kind != name) error ("name expected in declaration");
	string name = t.name;
	if (is_declared(name)) error(name, " declared twice");
	Token t2 = ts.get();
	if (t2.kind != '=') error("= missing in declaration of " ,name);
	double d = expression();
	names.push_back(Variable(name,d));
	return d;
}

double statement()
{
	Token t = ts.get();
	switch(t.kind) {
	case let:
		return declaration();
	default:
		ts.putback(t);
		return expression();
	}
}

void clean_up_mess()
{
	ts.ignore(print);
}

const string prompt = "> ";
const string result = "= ";

void calculate()
{
	names.push_back(Variable("pi", 3.14159)); //����� ������ �� names�� pi�� e�� �̸� �־�д�.
	names.push_back(Variable("e", 2.71828));
	while(true) try {
		cout << prompt;
		Token t = ts.get();
		while (t.kind == print) t=ts.get();
		if (t.kind == quit) return;
		ts.putback(t);
		cout << result << statement() << endl;
	}
	catch(runtime_error& e) {
		cerr << e.what() << endl;
		clean_up_mess();
	}
}

int main()
try {
	calculate();
	return 0;
}
catch (exception& e) {
	cerr << "exception: " << e.what() << endl;
	return 1;
}
catch (...) {
	cerr << "exception\n";
	return 2;
}
