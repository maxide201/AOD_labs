#include <iostream>
#include <stack>
using namespace std;


void Calc(stack<int>* operands, stack<char>* operators);

void main()
{
	stack<int> operands; // стек операндов
	stack<char> operators; // стек операторов
	string inp = "10*(8+2)-2*3*(4+2*3)"; // входная строка
	string n = ""; //переменная для считывания числа

	for (int i = 0; i < inp.length(); i++)
	{
		//Считывание числа и запись его в стек операндов
		if (isdigit(inp[i]))
		{
			n += inp[i];
		}
		else if (n != "")
		{
			operands.push(atoi(n.c_str()));
			n = "";
		}

		//Считывание операторов + и -
		//Проверка на приоритетность нового оператора по сравнению с оператором в стеке
		if (inp[i] == '+' || inp[i] == '-')
		{
			if (!operators.empty())
			{
				if (operators.top() == '*' || operators.top() == '/')
				{
					Calc(&operands, &operators);
				}
			}
			operators.push(inp[i]);
		}
		//Считывание операторов * / (
		//Добавление в стек операторов
		if (inp[i] == '*' || inp[i] == '/' || inp[i] == '(')
			operators.push(inp[i]);

		//Идём по стеку операторов до оператора ')'. Попутно выталкиваем всё из стеков
		if (inp[i] == ')')
		{
			while (operators.top()!='(')
				Calc(&operands, &operators);

			operators.pop();
		}
	}
	//Если до этого считывали число, то добавляем его в стек операндов
	if(n!="") operands.push(atoi(n.c_str()));
	//Используем все операторы из стека
	while (!operators.empty())
		Calc(&operands, &operators);

	cout << operands.top();
}

void Calc(stack<int>* operands, stack<char>* operators)
{
	//Выталкиваем два верхних операнда из стека
	int a = operands->top();
	operands->pop();
	int b = operands->top();
	operands->pop();
	//Выталкиваем оператор из стека, проводим вычисления и добавляем резкльтат в стек операндов
	switch (operators->top())
	{
	case '+':
		operands->push(a + b);
		break;
	case '-':
		operands->push(b - a);
		break;
	case '*':
		operands->push(a * b);
		break;
	case '/':
		operands->push(b / a);
		break;
	}
	operators->pop();
}