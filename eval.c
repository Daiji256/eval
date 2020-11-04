#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

double eval(const char *org)
{
	int i, j, cnt, f;
	double tmp;
	char str[STR_LENGTH], str2[STR_LENGTH], val[16];
	strinit(str, org);

	for (i = 1; str[i] != '\0'; i++)
	{
		if (str[i] != '(') continue;
		if (!isfunc(str[i - 1]))
		{
			if (str[i + 1] == '-')
			{
				f = 1;
				for (j = 2; str[i + j] != ')'; j++)
				{
					if (str[i + j] == '\0') exit(EXIT_FAILURE);
					if (!isnumber(str[i + j]))
					{
						f = 0;
						break;
					}
				}
				if (f)
				{
					i += j;
					continue;
				}
			}
			else
			{
				for (j = 0, cnt = 1; cnt != 0; j++)
				{
					if (str[i + 1 + j] == '(') cnt++;
					else if (str[i + 1 + j] == ')') cnt--;
					str2[j] = str[i + 1 + j];
				}
				str2[j - 1] = '\0';
				tmp = eval(str2);
				str2[strlen(str2) + 1] = '\0';
				str2[strlen(str2)] = ')';
				for (j = strlen(str2); j >= 0; j--) str2[j + 1] = str2[j];
				str2[0] = '(';
			}
		}
		else
		{
			for (; isfunc(str[i - 1]); i--);
			for (j = 0; str[i + j] != '('; j++) str2[j] = str[i + j];
			str2[j] = '(';
			for (j++, cnt = 1; cnt != 0; j++)
			{
				if (str[i + j] == '(') cnt++;
				else if (str[i + j] == ')') cnt--;
				str2[j] = str[i + j];
			}
			str2[j] = '\0';
			tmp = calcfunc(str2);
		}
		num2str(val, tmp);
		strrep(str, str2, val);
	}

	for (i = 1; str[i] != '\0'; i++)
	{
		if (str[i] != '*' && str[i] != '/') continue;
		tmp = calc(str, str2, i);
		num2str(val, tmp);
		strrep(str, str2, val);
		i = 0;
	}

	for (i = 1; str[i] != '\0'; i++)
	{
		if (str[i] != '+' && str[i] != '-') continue;
		if (str[i] == '-' && str[i - 1] == '(') continue;
		tmp = calc(str, str2, i);
		num2str(val, tmp);
		strrep(str, str2, val);
		i = 0;
	}

	while (strchr(str, '(')) strrep(str, "(", "");
	while (strchr(str, ')')) strrep(str, ")", "");

	return atof(str);
}

double calc(const char *org, char *str, int op)
{
	int l, r, i;
	double tmp;
	char a[STR_LENGTH], b[STR_LENGTH];

	l = op - 1;
	if (org[l] == ')')
	{
		while (org[l] != '(')
		{
			l--;
			if (l < 0) exit(EXIT_FAILURE);
		}
		for (i = 0; l + 1 + i < op - 1; i++) a[i] = org[l + 1 + i];
		a[i] = '\0';
	}
	else if (isnumber(org[l]))
	{
		while (isnumber(org[l - 1]))
		{
			l--;
			if (l < 0) exit(EXIT_FAILURE);
		}
		if (org[l - 1] == '-' && org[l - 2] == '(') l--;
		for (i = 0; l + i < op; i++) a[i] = org[l + i];
		a[i] = '\0';
	}
	else if (org[l] == '(')
	{
		l = op;
		a[0] = '0';
		a[1] = '\0';
	}

	r = op + 1;
	if (org[r] == '(')
	{
		while (org[r] != ')')
		{
			r++;
			if (org[r] == '\0') exit(EXIT_FAILURE);
		}
		for (i = 0; op + 2 + i <= r - 1; i++) b[i] = org[op + 2 + i];
		b[i] = '\0';
	}
	else if (isnumber(org[r]))
	{
		while (isnumber(org[r + 1]))
		{
			r++;
			if (org[r] == '\0') exit(EXIT_FAILURE);
		}
		for (i = 0; op + 1 + i <= r; i++) b[i] = org[op + 1 + i];
		b[i] = '\0';
	}

	for (i = 0; l + i <= r; i++) str[i] = org[l + i];
	str[i] = '\0';

	switch (org[op])
	{
		case '+': tmp = atof(a) + atof(b); break;
		case '-': tmp = atof(a) - atof(b); break;
		case '*': tmp = atof(a) * atof(b); break;
		case '/': tmp = atof(a) / atof(b); break;
		default: tmp = 0; break;
	}

	return tmp;
}

double calcfunc(const char *org)
{
	char func[10], x[STR_LENGTH], y[STR_LENGTH];
	int i, j, cnt;

	for (i = 0; isfunc(org[i]); i++) func[i] = org[i];
	func[i] = '\0';

	if (strcmp("abs", func) == 0 || strcmp("fabs", func) == 0 || strcmp("sqrt", func) == 0 || strcmp("sin", func) == 0 || strcmp("cos", func) == 0 || strcmp("tan", func) == 0 || strcmp("asin", func) == 0 || strcmp("acos", func) == 0 || strcmp("atan", func) == 0 || strcmp("sinh", func) == 0 || strcmp("cosh", func) == 0 || strcmp("tanh", func) == 0 || strcmp("asinh", func) == 0 || strcmp("acosh", func) == 0 || strcmp("atanh", func) == 0 || strcmp("exp", func) == 0 || strcmp("log", func) == 0 || strcmp("log10", func) == 0 || strcmp("ceil", func) == 0 || strcmp("floor", func) == 0 || strcmp("round", func) == 0)
	{
		for (i++, j = cnt = 0; cnt > 0 || org[i + j] != ')'; j++)
		{
			if (org[i + j] == '(') cnt++;
			else if (org[i + j] == ')') cnt--;
			else if (org[i + j] == '\0') exit(EXIT_FAILURE);
			x[j] = org[i + j];
		}
		x[j] = '\0';

		if (strcmp("abs", func) == 0) return fabs(eval(x));
		else if (strcmp("fabs", func) == 0) return fabs(eval(x));
		else if (strcmp("sqrt", func) == 0) return sqrt(eval(x));
		else if (strcmp("sin", func) == 0) return sin(eval(x));
		else if (strcmp("cos", func) == 0) return cos(eval(x));
		else if (strcmp("tan", func) == 0) return tan(eval(x));
		else if (strcmp("asin", func) == 0) return asin(eval(x));
		else if (strcmp("acos", func) == 0) return acos(eval(x));
		else if (strcmp("atan", func) == 0) return atan(eval(x));
		else if (strcmp("sinh", func) == 0) return sinh(eval(x));
		else if (strcmp("cosh", func) == 0) return cosh(eval(x));
		else if (strcmp("tanh", func) == 0) return tanh(eval(x));
		else if (strcmp("asinh", func) == 0) return asinh(eval(x));
		else if (strcmp("acosh", func) == 0) return acosh(eval(x));
		else if (strcmp("atanh", func) == 0) return atanh(eval(x));
		else if (strcmp("exp", func) == 0) return exp(eval(x));
		else if (strcmp("log", func) == 0) return log(eval(x));
		else if (strcmp("log10", func) == 0) return log10(eval(x));
		else if (strcmp("ceil", func) == 0) return ceil(eval(x));
		else if (strcmp("floor", func) == 0) return floor(eval(x));
		else if (strcmp("round", func) == 0) return round(eval(x));
	}

	if (strcmp("pow", func) == 0 || strcmp("atan2", func) == 0 || strcmp("hypot", func) == 0 || strcmp("mod", func) == 0 || strcmp("fmod", func) == 0)
	{
		for (i++, j = cnt = 0; cnt > 0 || org[i + j] != ','; j++)
		{
			if (org[i + j] == '(') cnt++;
			else if (org[i + j] == ')') cnt--;
			else if (org[i + j] == '\0') exit(EXIT_FAILURE);
			x[j] = org[i + j];
		}
		x[j] = '\0';
		for (i += j + 1, j = cnt = 0; cnt > 0 || org[i + j] != ')'; j++)
		{
			if (org[i + j] == '(') cnt++;
			else if (org[i + j] == ')') cnt--;
			else if (org[i + j] == '\0') exit(EXIT_FAILURE);
			y[j] = org[i + j];
		}
		y[j] = '\0';

		if (strcmp("pow", func) == 0) return pow(eval(x), eval(y));
		else if (strcmp("atan2", func) == 0) return atan2(eval(x), eval(y));
		else if (strcmp("hypot", func) == 0) return hypot(eval(x), eval(y));
		else if (strcmp("mod", func) == 0) return fmod(eval(x), eval(y));
		else if (strcmp("fmod", func) == 0) return fmod(eval(x), eval(y));
	}

	return 0;
}

void strinit(char *str, const char *org)
{
	sprintf(str, "(%s)", org);
	while (strchr(str, ' ')) strrep(str, " ", "");
}

void strrep(char *str, const char *bef, const char *aft)
{
	char tmp[STR_LENGTH], *p;

	p = str;
	if ((p = strstr(p, bef)))
	{
		strcpy(tmp, p + strlen(bef));
		*p = '\0';
		strcat(str, aft);
		strcat(str, tmp);
	}
}

void num2str(char *str, double num)
{
	if (num < 0) sprintf(str, "(%f)", num);
	else sprintf(str, "%f", num);
}

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

int isfunc(char c)
{
	if (!isoperator(c) && c != '(' && c != ')') return 1;
	return 0;
}
