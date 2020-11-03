#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#define STR_LENGTH 1024
enum token {NUMBER = 1, OPERATOR = 2, LEFTPAREN = 3, RIGHTPAREN = 4};

struct count
{
	int leftparen;
	int number;
	int operator;
	int sign;
	int rightparen;
};

void strrep(char str[], const char *bef, const char *aft);
int isoperator(char c);
char *getoperator(char *str);
double calc(char *str, int i);
double eval(char *str);

int isnumber(char c)
{
	if (isdigit(c) || c == '.') return 1;
	return 0;
}

int isoperator(char c)
{
	if (c == '+' || c == '-' || c == '*' || c == '/') return 1;
	return 0;
}

char *getoperator(char *str)
{
	int i, count = 0;

	for (i = 0; (count != 0 || str[i] != ')') && str[i] != '\0'; i++)
	{
		if (count == 0 && isoperator(str[i])) return &str[i];
		else if (str[i] == '(') count++;
		else if (str[i] == ')') count--;
	}
	return NULL;
}

double calc(char *str, int i)
{
	int j, k;
	double tmp = 0, a, b;
	printf("calc: str = %s\n", str);

	char str2[STR_LENGTH], val[50];

	char *pl, *pr, *next;

	// 左辺
	j = i - 1;
	if (str[j] == ')')
	{
		while (str[j] != '(')
		{
			j--;
			if (j == -1) exit(EXIT_FAILURE);
		}
		pl = str + j;
		a = eval(pl);
	}
	else if (isnumber(str[j]))
	{
		while (isnumber(str[j - 1]))
		{
			j--;
			if (j == -1) exit(EXIT_FAILURE);
		}
		pl = str + j;
		a = atof(pl);
	}
	else if (str[j] == '(')
	{
		pl = str + j + 1;
		a = 0;
	}

	// 右辺
	j = i + 1;
	if (str[j] == '(')
	{
		pr = str + j;
		if (str[i] == '+' || str[i] == '-')
		{
			if ((next = getoperator(pr)) != NULL)
			{
				switch (*next)
				{
					case '*':
					case '/': b = calc(pr, next - pr); break;
					case '+':
					case '-': b = eval(pr); break;
					default: break;
				}
			}
			else b = eval(pr);
		}
		else b = eval(pr);
		if (str[j] == '(')
		{
			while (str[j] != ')')
			{
				j++;
				if (str[j] == '\0') exit(EXIT_FAILURE);
			}
		}
		else if (isnumber(str[j])) while (isnumber(str[j + 1])) j++;
		else exit(EXIT_FAILURE);
	}
	else if (isnumber(str[j]))
	{
		pr = str + j;
		if (str[i] == '+' || str[i] == '-')
		{
			if ((next = getoperator(pr)) != NULL)
			{
				switch (*next)
				{
					case '*':
					case '/': b = calc(pr, next - pr); break;
					case '+':
					case '-': b = atof(pr); break;
					default: break;
				}
			}
			else b = atof(pr);
		}
		else b = atof(pr);
		if (str[j] == '(')
		{
			while (str[j] != ')')
			{
				j++;
				if (str[j] == '\0') exit(EXIT_FAILURE);
			}
		}
		else if (isnumber(str[j])) while (isnumber(str[j + 1])) j++;
		else exit(EXIT_FAILURE);
	}

	switch (str[i])
	{
		case '+': tmp = a + b; break;
		case '-': tmp = a - b; break;
		case '*': tmp = a * b; break;
		case '/': if (b == 0) exit(EXIT_FAILURE); else tmp = a / b; break;
		default: break;
	}

	for (k = 0; pl + k <= str + j; k++) str2[k] = pl[k];
	str2[k] = '\0';

	if (tmp >= 0) sprintf(val, "%f", tmp);
	else sprintf(val, "(%f)", tmp);
	strrep(str, str2, val);

	return tmp;
}

double eval(char *str)
{
	int i, count;
	double tmp;
	char *p;

	count = 0;
	p = str + 1;
	tmp = atof(p);
	while (1)
	{
		if (*p == '(') if ((tmp = eval(p)) < 0) count++;
		else if (*p == ')') count--;
		else if (*p == '\0') exit(EXIT_FAILURE);
		p++;
		if (count < 0) exit(EXIT_FAILURE);
		if (count == 0 && *p == ')') break;
	}

	while ((p = getoperator(str + 1)) != NULL) tmp = calc(str, p - str);

	return tmp;
}

void strrep(char str[], const char *bef, const char *aft)
{
	char tmp[STR_LENGTH], *p;

	p = str;
	if (p = strstr(p, bef))
	{
		strcpy(tmp, p + strlen(bef));
		*p = '\0';
		strcat(str, aft);
		strcat(str, tmp);
	}
}

void init(const char in[], char out[])
{
	sprintf(out, "(%s)", in);
	while (strchr(out, ' ')) strrep(out, " ", "");
}

int main(int argc, char **argv)
{
	char str[STR_LENGTH];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s \"func\"\n", argv[0]);
		return 0;
	}

	init(argv[1], str);
	printf("expression:%s\nanswer:%f\n", argv[1], eval(str));

	return 0;
}
