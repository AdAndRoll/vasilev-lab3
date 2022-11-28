

#include <string>
#include <iostream>
#include <stack> // сделать свой
#include <queue> // сделать свой
//вместо switch можно сделать список(массив) правил
//базовый класс rool, три метода: перейти, чек и действие
//сделать отдельными классами лексический, синтаксический анализы и транслятор

using namespace std;

enum TypeElement {
	Operation,
	Value
};

class Lexema {
	string str;
	TypeElement type;
public:
	Lexema(string _str="", TypeElement _type = Operation) : str(_str), type(_type) {};
	int getPriority() {
		if (type == Operation) {
			if (str == "(") return 0;
			if (str == ")") return 1;
			if (str == "+" || str == "-") return 2;
			if (str == "*" || str == "/") return 3;
		}
		return -1;
	}
	string getStr() { return str; }
	TypeElement getType() { return type; }
	friend ostream& operator << (ostream& out, Lexema& p) {
		out << "{" << p.str << ", ";
		if (p.type == Operation) {
			out << "operation";
		}
		else if (p.type == Value) {
			out << "value";
		};
		out << "}";
		return out;
	}
};

queue <Lexema> lex(string input) {
	queue<Lexema>res;
	input += ' ';
	int i = 0;
	string tmp = "";
	string op = "+-*/()";
	string sep = " \n\t";
	int state = 0;
	for (i = 0; i < input.size(); i++) {
		char c = input[i];
		int fres;
		switch (state)
		{
		case 0: // операция
			if (c >= '0' && c <= '9') {
				tmp = c;
				state = 1;
				break;
			}
			fres = op.find(c);
			if (fres >= 0) {
				tmp = c;
				Lexema l(tmp, Operation);
				res.push(l);
				state = 0;
				break;
			}
			break;
		case 1: // число
			if (c >= '0' && c <= '9') {
				tmp += c;
				state = 1;
				break;
			}
			fres = op.find(c);
			if (fres >= 0) {
				Lexema l1(tmp, Value);
				res.push(l1);
				tmp = c;
				Lexema l2(tmp, Operation);
				res.push(l2);
				state = 0;
				break;
			}
			fres = sep.find(c);
			if (fres >= 0) {
				Lexema l(tmp, Value);
				res.push(l);
				state = 0;
				break;
			}
			break;
		}
	}
	return res;
}

void print(queue <Lexema> t) {
	while (!t.empty()) {
		cout << t.front() << " ";
		t.pop();
	}
	cout << endl;
}
void prints(stack <Lexema> t) {
	while (!t.empty()) {
		cout << t.top() << " ";
		t.pop();
	}
	cout << endl;
}
queue<Lexema> toPostfix(queue<Lexema> inf) {
	queue<Lexema> res;
	stack<Lexema> st;
	Lexema lex, stLex;
	while (!inf.empty()) {
		lex = inf.front();
		if (lex.getType() == Value) {
			res.push(lex);
		}
		if (lex.getStr() == "(") {
			st.push(lex);


		}
		else if (lex.getStr() == ")") {
			stLex = st.top(); st.pop();
			while (stLex.getStr() != "(") {
				res.push(stLex);

				stLex = st.top(); st.pop();

			}
		}
		else if (lex.getType() == Operation) {
			while (!st.empty()) {
				stLex = st.top(); st.pop();
				if (lex.getPriority() <= stLex.getPriority()) {
					
					res.push(stLex);

				}
				else {
					
					
					st.push(stLex);

					break;
				}
			}
			
			st.push(lex);

		}
		inf.pop();
	}
	while (!st.empty()) {
		stLex = st.top(); st.pop();
		res.push(stLex);

	}
	return res;
}
double Calcucator(queue <Lexema> synt_res) {
	Lexema lex;
	stack<double> res;
	double roperand;
	double loperand;
	while (!synt_res.empty()) {
		lex = synt_res.front();
		switch (lex.getType()) {
		case Operation:
			roperand = res.top();
			res.pop();
			loperand = res.top();
			res.pop();
			if (lex.getStr() == "+") {
				res.push(loperand + roperand);
			}
			else if (lex.getStr() == "-") {
				res.push(loperand - roperand);
			}
			else if (lex.getStr() == "*") {
				res.push(loperand * roperand);
			}
			else if (lex.getStr() == "/") {
				res.push(loperand / roperand);
			}
			break;
		case Value:
			res.push(stoi(lex.getStr()));
			break;
		
		}
		synt_res.pop();
	}
	return res.top();
}

int main() {
	string str = "( 123 -10)/ 50 *	\t	30 \n";
	//string str = "(15-4)*3";
	cout << str<< endl;
	queue <Lexema> lex_res;
	lex_res = lex(str);
	print(lex_res);
	queue<Lexema> synt_res= toPostfix(lex_res);
	cout << "synt_res" << endl;
	print(synt_res);
	cout << Calcucator(synt_res);
	return 0;
}