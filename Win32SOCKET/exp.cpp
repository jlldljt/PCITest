
/****************************************************************************


本工程实现了四则运算、关系运算、逻辑运算
1.其中支持输入
四则运算+ - * / %取余
关系运算> == < != >= <=
逻辑运算^ || &&
    注意：^会把左右操作数转换成整数再进行运算，故对于有小数的情况，请慎用
值      float,保留了v字段，可以自定义成任何外部内存中值

2.程序运算时只能单字节运算符，故内部对两个字节的运算符进行了转换，更改#define ReadVar(no)
规则:
== =
!= !
>= }
<= {
|| |
&& &

****************************************************************************/



#include "stdafx.h"
#include "exp.h"
#include <stdio.h>  //计算器
#include <malloc.h>
#include <stdlib.h>

#define NEWCH(A,len) NewMemory((void **)&(A) , (len))
#define DELETE(A) DelMemory((void **)&(A))

void NewMemory(void **p, size_t size) {
	
	if (0 == size)
		return;
	
	if (*p != NULL) {
		free(*p);
		*p = NULL;
	}

	*p = (malloc(size));
	
	while (NULL == *p)
	{
		;
	}

}

void DelMemory(void **p) {
	
	if (*p != NULL) {
		free(*p);
		*p = NULL;
	}
	
}


#define ReadVar(no)  (no)
#define STACK_SIZE		100	//max size of the stack
#define STACK_INCREMENT	10	//realloc size

typedef struct FStack  //stack of int
{
	float* base;
	float* top;
	int stacksize;
}FStack;

void Init(FStack* s)
{
	//	s->base = (float*)malloc(STACK_SIZE * sizeof(FStack));
	//	if (!s->base)
	//	{
	//		//printf("overflow!\n");
	//		return;
	//	}
	NEWCH(s->base, STACK_SIZE * sizeof(float));
	s->top = s->base;
	s->stacksize = STACK_SIZE;
}

void DeInit(FStack* s)
{
	DELETE(s->base);
	s->top = s->base;
	s->stacksize = 0;
}

bool isEmpty(FStack* s)
{
	if (s->top == s->base)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Push(FStack* s, float e)
{
	if (s->top - s->base >= s->stacksize)
	{
		//printf("stack is full!\nrealloc %d\n", STACK_INCREMENT);
		s->base = (float*)realloc(s->base, (s->stacksize + sizeof(FStack)));
		if (!s->base)
		{
			//printf("overflow!\n");
			return;
		}
		s->top = s->base + s->stacksize;
		s->stacksize += STACK_INCREMENT;
	}
	*(s->top) = e;
	(s->top)++;
}

float GetTop(FStack* s)
{
	float e;
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		return 0;
	}
	e = *(s->top - 1);
	return e;
}

void Pop(FStack* s)
{
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		//DELETE(s->base);
		return;
	}
	s->top--;
}

typedef struct CStack  //stack of char
{
	char* base;
	char* top;
	int stacksize;
}CStack;

void InitC(CStack* s)
{
	//	s->base = (char*)malloc(STACK_SIZE * sizeof(CStack));
	//	if (!s->base)
	//	{
	//		//printf("overflow!\n");
	//		return;
	//	}
	NEWCH(s->base, STACK_SIZE * sizeof(char));
	s->top = s->base;
	s->stacksize = STACK_SIZE;
}

void DeInitC(CStack* s)
{
	DELETE(s->base);
	s->top = s->base;
	s->stacksize = 0;
}

bool isEmptyC(CStack* s)
{
	if (s->top == s->base)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void PushC(CStack* s, int e)
{
	if (s->top - s->base >= s->stacksize)
	{
		//printf("stack is full!\nrealloc %d\n", STACK_INCREMENT);
		s->base = (char*)realloc(s->base, (s->stacksize + sizeof(CStack)));
		if (!s->base)
		{
			//printf("overflow!\n");
			return;
		}
		s->top = s->base + s->stacksize;
		s->stacksize += STACK_INCREMENT;
	}
	*(s->top) = e;
	(s->top)++;
}

char GetTopC(CStack* s)
{
	char e;
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		return 0;
	}
	e = *(s->top - 1);
	return e;
}

void PopC(CStack* s)
{
	if (s->top == s->base)
	{
		//printf("stack is empty!\n");
		//DELETE(s->base);
		return;
	}
	s->top--;
}

bool isArithmeticOperators(char ch)
{
	if (ch == '+' || ch == '-' || ch == '*' || ch == '/' || ch == '%')
	{
		return true;
	}
	else
	{
		return false;
	}
}


bool IsRelationalOperators(char ch)
{
	if (ch == '>' || ch == '=' || ch == '<' || ch == '!' || ch == '{' || ch == '}')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool IsLogicalOperators(char ch)
{
	if (ch == '&' || ch == '|' || ch == '^')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isNumeric(char ch)
{
	if ((ch >= '0' && ch <= '9') || ch == '.')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isLBracket(char ch)
{
	if (ch == '(')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isRBracket(char ch)
{
	if (ch == ')')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isVar(char ch)
{
	if (ch == 'V' || ch == 'v')
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool isIgnorable(char ch)
{
	if (ch == ' ' || ch == '	')
	{
		return true;
	}
	else
	{
		return false;
	}
}

int Priority(char ch)
{
	int p;
	switch (ch)
	{
	case '(':/*为了防止有运算符低于它*/
		p = 0;
		break;
	case '&':
	case '|':
	case '^':
		p = 1;
		break;
	case '>':
	case '<':
	case '{':
	case '}':
	case '!':
	case '=':
		p = 2;
		break;
	case '+':
	case '-':
		p = 3;
		break;
	case '*':
	case '/':
	case '%':
		p = 4;
		break;
	}
	return p;
}

float Calculate(float f1, float f2, char oper)
{
	float f3;
	switch (oper)
	{
	case '+':
		f3 = f1 + f2;
		break;
	case '-':
		f3 = f1 - f2;
		break;
	case '*':
		f3 = f1 * f2;
		break;
	case '%':
		f3 = (float)((int)f1 % (int)f2);
		break;
	case '/':
		if (f2 == 0)
		{
			//printf("\nDevided by zero!");
			f3 = 0;
		}
		else
		{
			f3 = f1 / f2;
		}
		break;
	case '{':
		f3 = f1 <= f2;
		break;
	case '}':
		f3 = f1 >= f2;
		break;
	case '=':
		f3 = f1 == f2;
		break;
	case '!':
		f3 = f1 != f2;
		break;
	case '&':
		f3 = f1 && f2;
		break;
	case '|':
		f3 = f1 || f2;
		break;
	case '^':
		f3 = float((short)f1 ^ (short)f2);
		break;
	case '>':
		f3 = f1 > f2;
		break;
	case '<':
		f3 = f1 < f2;
		break;
	}
	return f3;
}

float StrtoFloat(char* str, int* pos)
{
	float fRes;
	int i = *pos;
	int k;
	char n[50] = { 0 };
	for (k = 0; (str[i] >= '0' && str[i] <= '9') || str[i] == '.'; i++, k++)
	{
		n[k] = str[i];
	}
	n[k] = '\0';
	*pos = i - 1;
	fRes = atof(n);
	return fRes;
}

char StrtoSpecialRelationalOperator(char* str, int* pos)
{
	int i = *pos;
	int k;
	char result = 0;
	for (k = 0; k < 4 && IsRelationalOperators(str[i + k]); k++)
	{
		;
	}
		
	if (1 == k) {
		if('>' == str[i] || '<' == str[i])
			result = str[i];
	}
	else if (2 == k) {
		if ('=' == str[i + 1]) {
			switch (str[i])
			{
			case '>':
				result = '}';
				break;
			case '<':
				result = '{';
				break;
			case '=':
				result = '=';
				break;
			case '!':
				result = '!';
				break;
			default:
				break;
			}
		}
	}

	*pos = *pos + k - 1;
	
	return result;
}

char StrtoSpecialLogicalOperator(char* str, int* pos)
{
	int i = *pos;
	int k;
	char result = 0;
	for (k = 0; k < 4 && IsLogicalOperators(str[i + k]); k++)
	{
		;
	}

	if (1 == k) {
		if ('^' == str[i])
			result = str[i];
	}
	else if (2 == k) {
		if ('&' == str[i + 1] && '&' == str[i]) {
			result =  '&';
		}
		else if ('|' == str[i + 1] && '|' == str[i]) {
			result = '|';
		}
	}

	*pos = *pos + k - 1;

	return result;
}

bool Check(char* str)
{
	int i = 0;
	while (str[i] != '\0')
	{
		if (str[i] != '+' && str[i] != '-' && str[i] != '*' && str[i] != '/' && str[i] != '%' && str[i] != '.' && str[i] != '(' && str[i] != ')' && (str[i] < '0' || str[i] > '9'))
		{
			return false;
		}
		i++;
	}
	return true;
}

/*
左结合规则：
不带空格
数字：数字、操作符、左括号、变量 起始
运算操作符：数字、右括号
关系符：数字、右括号，but关系符本身可以有两个宽
逻辑：数字、右括号，but逻辑符本身可以有两个宽
左括号：操作符、左括号  起始
右括号：左括号、数字
变量：操作符、左括号  起始

带空格
数字：操作符、左括号  起始
运算操作符：数字、右括号
关系符：数字、右括号
逻辑：数字、右括号
左括号：操作符、左括号  起始
右括号：左括号、数字
变量：操作符、左括号  起始
*/
bool CheckEx(char* str)
{
	int i = 0;
	char elm = 0;//0:起始 1:左括 2:操作符 3:数字 4:变量 5:右括 高位1表示被空格阻隔
	while (str[i] != '\0')
	{
		if (isLBracket(str[i]))
		{

			if ((elm & 0x0F) > 2)
				return false;

			elm = 1;
		}
		else if (isArithmeticOperators(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return false;

			elm = 2;
		}
		else if (isNumeric(str[i]))
		{
			if (elm & 0x80)
			{
				if ((elm & 0x0F) > 2)
					return false;
			}
			else
			{
				if ((elm & 0x0F) > 4)
					return false;
			}
			elm = 3;
		}
		else if (isVar(str[i]))
		{
			if ((elm & 0x0F) > 2)
				return false;

			elm = 4;
		}
		else if (isRBracket(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 1)
				return false;

			elm = 5;
		}
		else if (isIgnorable(str[i]))
		{
			elm |= 0x80;//标记有空格
		}
		else
		{
			return false;
		}

		i++;
	}
	return true;
}

/*
左结合规则：
不带空格
数字：数字、操作符、左括号、变量 起始
运算操作符：数字、右括号
关系符：数字、右括号，but关系符本身可以有两个宽
逻辑：数字、右括号，but逻辑符本身可以有两个宽
左括号：操作符、左括号  起始
右括号：左括号、数字、右括号
变量：操作符、左括号  起始

带空格
数字：操作符、左括号  起始
运算操作符：数字、右括号
关系符：数字、右括号
逻辑：数字、右括号
左括号：操作符、左括号  起始
右括号：左括号、数字、右括号
变量：操作符、左括号  起始
*/
bool CheckExOut(char* str , char *out)
{
	int i = 0;
	int j = 0;
	int left_brackets = 0, right_brackets = 0;
	char elm = 0;//0:起始 1:左括 2:操作符 3:数字 4:变量 5:右括 高位1表示被空格阻隔
	char current_char;
	while ((current_char = str[i]) != '\0')
	{
		if (isLBracket(str[i]))
		{

			if ((elm & 0x0F) > 2)
				return false;
			left_brackets++;
			elm = 1;
		}
		else if (isArithmeticOperators(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return false;

			elm = 2;
		}
		else if (IsRelationalOperators(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return false;

			if (!(current_char = StrtoSpecialRelationalOperator(str, &i)))
				return false;

			elm = 2;
		}
		else if (IsLogicalOperators(str[i]))
		{
			if ((elm & 0x0F) != 3 && (elm & 0x0F) != 5)
				return false;

			if (!(current_char = StrtoSpecialLogicalOperator(str, &i)))
				return false;

			elm = 2;
		}
		else if (isNumeric(str[i]))
		{
			if (elm & 0x80)
			{
				if ((elm & 0x0F) > 2)
					return false;
			}
			else
			{
				if ((elm & 0x0F) > 4)
					return false;
			}
			elm = 3;
		}
		else if (isVar(str[i]))
		{
			if ((elm & 0x0F) > 2)
				return false;

			elm = 4;
		}
		else if (isRBracket(str[i]))
		{
			if ((elm & 0x0F) != 5 && (elm & 0x0F) != 3 && (elm & 0x0F) != 1)
				return false;
			right_brackets++;
			elm = 5;
		}
		else if (isIgnorable(str[i]))
		{
			elm |= 0x80;//标记有空格
			i++;
			continue;
		}
		else
		{
			return false;
		}
		out[j] = current_char;// str[i];
		j++;
		i++;
	}
	out[j] = '\0';

	if(right_brackets == left_brackets)
	  return true;
	
	return false;
}

void TestExp()
{
	char exp[100] = { 0 };
	char org_exp[100] = { " 1 + 2 < 3 + 9 >= 9 / 3 && (5 + 9 + (5>2)) && 1 " };
	int i;
	float f, f1, f2;
	char oper;
	FStack fstack = { 0 };
	CStack cstack = { 0 };
	Init(&fstack);
	InitC(&cstack);
	printf("The expression is:");
	gets_s(org_exp,100);
	if (!CheckExOut(org_exp,exp))
	{
		printf("input error! exit now!\n");
		return;
	}
	puts(exp);
	for (i = 0; exp[i] != '\0' && exp[i] != -52/*0xCC*/; i++)
	{
		if (isArithmeticOperators(exp[i]) || IsLogicalOperators(exp[i]) || IsRelationalOperators(exp[i]))
		{
			if (!isEmptyC(&cstack))
			{
				while (!isEmptyC(&cstack) && Priority(exp[i]) <= Priority(GetTopC(&cstack)))
				{
					oper = GetTopC(&cstack);
					PopC(&cstack);
					f2 = GetTop(&fstack);
					Pop(&fstack);
					f1 = GetTop(&fstack);
					Pop(&fstack);
					f = Calculate(f1, f2, oper);
					Push(&fstack, f);
				}
				PushC(&cstack, exp[i]);
			}
			else
			{
				PushC(&cstack, exp[i]);
			}
		}
		else if (isNumeric(exp[i]))
		{
			f = StrtoFloat(exp, &i);
			Push(&fstack, f);
		}
		else if (exp[i] == '(')
		{
			PushC(&cstack, exp[i]);
		}
		else if (exp[i] == ')')
		{
			while (GetTopC(&cstack) != '(' && !isEmptyC(&cstack))
			{
				oper = GetTopC(&cstack);
				PopC(&cstack);
				f2 = GetTop(&fstack);
				Pop(&fstack);
				f1 = GetTop(&fstack);
				Pop(&fstack);
				f = Calculate(f1, f2, oper);
				Push(&fstack, f);
			}
			PopC(&cstack);
		}
		else if (isVar(exp[i]))
		{
			i++;
			f = StrtoFloat(exp, &i);
			f = ReadVar(f);
			Push(&fstack, f);
		}

	}
	//exp  analyse  over
	while (!isEmptyC(&cstack))
	{
		oper = GetTopC(&cstack);
		PopC(&cstack);
		f2 = GetTop(&fstack);
		Pop(&fstack);
		f1 = GetTop(&fstack);
		Pop(&fstack);
		f = Calculate(f1, f2, oper);
		Push(&fstack, f);
	}
	printf("\nThe result is:%f\n", GetTop(&fstack));
	Pop(&fstack);

	DeInit(&fstack);
	DeInitC(&cstack);
}
