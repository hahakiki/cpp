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

const char user_sqrt = 'r'; //sqrt,sin,cos,tan 를 입력 할 경우 각각에 대해 계산하기 위해 
const char user_sin = 's'; //switch문에 사용할 문자 지정.
const char user_cos = 'c'; //user_의 경우 이름을 사용하기에 각각 함수 이름과 같아 붙였다.
const char user_tan = 't';

const char salary_time = 'p'; //봉급 계산을 위한 시간
const int basic_salary = 7530; //최저 시급

Token Token_stream::get()
{
	if (full) { full=false; return buffer; }
	char ch;
	cin >> ch;
	switch (ch) {
	case '!': // !문자를 인식
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
	case '^': //n제곱을 위해 '^'추가
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

			if (s == "sqrt") return Token(user_sqrt); //sqrt, sin, cos, tan 각각에 대해 입력이 될 때
			if (s == "sin") return Token(user_sin); //해당하는 토큰값을 리턴하였다.
			if (s == "cos") return Token(user_cos);
			if (s == "tan") return Token(user_tan);

			if (s == "salarytime") return Token(salary_time); //salarytime이 입력될 경우 해당하는 토큰값을 리턴한다.

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

int Factorial(int value) //factorial을 계산하는 함수이다.
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
	Token t2 = ts.get(); //factorial을 인식하기위해 Token t 이후 하나의 토큰을 더 구함

	if (t2.kind == '!') //두번째 Token이 !(factorial)인 경우 Factorial() 호출을 통해 t의 값을 변경한다.
	{
		t.value = Factorial(t.value);
	}
	else ts.putback(t2); //!가 아닐경우 다시 넣는다.

	switch (t.kind) {
	case '(':
	{	double d = expression();
		t = ts.get();
		if (t.kind != ')') error("'(' expected");
		else return d; //괄호 값 계산 후 계산된 값을 반환한다.(logical error) 
	}
	case '-':
		return - primary();
	case '+':
		return primary(); //+숫자 의 경우를 반영하기 위해 primary()를 return한다. (extend unary plus)
	case number:
		return t.value;
	case name:
		return get_value(t.name);

	case user_sqrt: //각각에 대해 expression()함수를 호출해 괄호 안의 값을 알아내고 알맞게 계산해 반환하였다.
	{
		double d = expression();
		if (d < 0) //음수가 입력될 경우 error()를 사용하였다.
			error("error: negative integer"); //에러메시지
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

	case salary_time: //최저시급 계산
	{
		double d = expression();
		if (d < 1) error("0 혹은 음수값을 입력하셨습니다.");
		return d * basic_salary;
	}

	default:
		error("primary expected");
	}
}


double mod(int left, int primary) //나머지 연산을 하는 경우 사용되는 함수 (optional)
{
	if (left < primary)
	{
		error("수식이 정확한지 확인해주세요."); //네이버 계산기와 같은 효과를 냈다.
	}
	return left % primary;
}

double n_ary_square(double num, int n) // n제곱 반환 함수
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
		case '^': //n 제곱을 하기 위한 case
			left = n_ary_square(left, primary());
			break;
		case '%': //나머지 연산(optional)
			left = mod(left, primary());
			cout << "나머지 연산의 경우 실수를 입력하시면 int로 자동 변환됩니다." << endl; //자동 정수 변환을 알려줌
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
	names.push_back(Variable("pi", 3.14159)); //계산이 시작할 때 names에 pi와 e를 미리 넣어둔다.
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
