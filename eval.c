#include "eval.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void init(const char in[], char out[])
{
	sprintf(out, "(%s)", in);
	while (strchr(out, ' ')) strrep(out, " ", "");
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

double eval(char *str)
{
	char str2[STR_LENGTH];
	init(str, str2);
	return eval2(str2);
}

double eval2(char *str)
{
	int i, count;
	double tmp;
	char *p, str2[STR_LENGTH], val[50];

	printf("eval: %s\n", str);

	count = 0;
	p = str + 1;
	tmp = atof(p);
	while (1)
	{
		if (*p == '(')
		{
			if (!isoperator(*(p - 1)) && *(p - 1) != '(' && *(p - 1) != ')')
			{
				while (!isoperator(*(p - 1)) && *(p - 1) != '(' && *(p - 1) != ')') p--;
				tmp = calcfunc(p);
			}
			else if ((tmp = eval2(p)) < 0) count++;
		}
		else if (*p == ')') count--;
		else if (*p == '\0') exit(EXIT_FAILURE);
		p++;
		if (count < 0) exit(EXIT_FAILURE);
		if (count == 0 && *p == ')') break;
	}

	while ((p = getoperator(str + 1)) != NULL) tmp = calc(str, p - str);

	count = 0;
	i = 0;
	while (1)
	{
		if (str[i] == '(') count++;
		else if (str[i] == ')') count--;
		else if (str[i] == '\0') exit(EXIT_FAILURE);
		str2[i] = str[i];
		i++;
		if (count == 0) break;
	}
	str2[i] = '\0';
	if (tmp >= 0) sprintf(val, "%f", tmp);
	else sprintf(val, "(%f)", tmp);
	strrep(str, str2, val);

	return tmp;
}

double calc(char *str, int i)
{
	int j, k;
	double tmp = 0, a, b;

	printf("calc: %s\n", str);

	char str2[STR_LENGTH], val[50];

	char *pl, *pr, *next;

	j = i - 1;
	if (str[j] == ')')
	{
		while (str[j] != '(')
		{
			j--;
			if (j == -1) exit(EXIT_FAILURE);
		}
		pl = str + j;
		a = eval2(pl);
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

	for (j = i + 1; str[j] != '\0'; j++)
	{
		if (str[j] == '(')
		{
			if (!isoperator(str[j - 1]) && str[j - 1] != '(' && str[j - 1] != ')')
			{
				while (!isoperator(str[j - 1]) && str[j - 1] != '(' && str[j - 1] != ')') j--;
				tmp = calcfunc(str + j);
			}
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
						case '-': b = eval2(pr); break;
						default: break;
					}
				}
				else b = eval2(pr);
			}
			else b = eval2(pr);
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
			break;
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
			break;
		}
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

double calcfunc(char *str)
{
	char func[10], x[STR_LENGTH], y[STR_LENGTH], str2[STR_LENGTH], val[50];
	int i, j, k, count;;
	double tmp = 0;
	for (i = 0; !isoperator(str[i]) && str[i] != '(' && str[i] != ')'; i++) func[i] = str2[i] = str[i];
	func[i] = '\0';
	if (strcmp("abs", func) == 0 || \
		strcmp("fabs", func) == 0 || \
		strcmp("sqrt", func) == 0 || \
		strcmp("cbrt", func) == 0 || \
		strcmp("sin", func) == 0 || \
		strcmp("cos", func) == 0 || \
		strcmp("tan", func) == 0 || \
		strcmp("asin", func) == 0 || \
		strcmp("acos", func) == 0 || \
		strcmp("atan", func) == 0 || \
		strcmp("sinh", func) == 0 || \
		strcmp("cosh", func) == 0 || \
		strcmp("tanh", func) == 0 || \
		strcmp("asinh", func) == 0 || \
		strcmp("acosh", func) == 0 || \
		strcmp("atanh", func) == 0 || \
		strcmp("exp", func) == 0 || \
		strcmp("log", func) == 0 || \
		strcmp("log10", func) == 0 || \
		strcmp("ceil", func) == 0 || \
		strcmp("floor", func) == 0 || \
		strcmp("round", func) == 0)
	{
		str2[i] = str[i];
		x[0] = '(';
		count = 0;
		for (j = 1; count != 0 || str[i + j] != ')'; j++)
		{
			if (str[i + j] == '(') count++;
			else if (str[i + j] == ')') count--;
			else if (str[i + j] == '\0') exit(EXIT_FAILURE);
			str2[i + j] = str[i + j];
			x[j] = str[i + j];
		}
		str2[i + j] = str[i + j];
		str2[i + j + 1] = '\0';
		x[j] = ')';
		x[j + 1] = '\0';

		if (strcmp("abs", func) == 0) tmp = fabs(eval2(x));
		else if (strcmp("fabs", func) == 0) tmp = fabs(eval2(x));
		else if (strcmp("sqrt", func) == 0) tmp = sqrt(eval2(x));
		else if (strcmp("cbrt", func) == 0) tmp = cbrt(eval2(x));
		else if (strcmp("sin", func) == 0) tmp = sin(eval2(x));
		else if (strcmp("cos", func) == 0) tmp = cos(eval2(x));
		else if (strcmp("tan", func) == 0) tmp = tan(eval2(x));
		else if (strcmp("asin", func) == 0) tmp = asin(eval2(x));
		else if (strcmp("acos", func) == 0) tmp = acos(eval2(x));
		else if (strcmp("atan", func) == 0) tmp = atan(eval2(x));
		else if (strcmp("sinh", func) == 0) tmp = sinh(eval2(x));
		else if (strcmp("cosh", func) == 0) tmp = cosh(eval2(x));
		else if (strcmp("tanh", func) == 0) tmp = tanh(eval2(x));
		else if (strcmp("asinh", func) == 0) tmp = asinh(eval2(x));
		else if (strcmp("acosh", func) == 0) tmp = acosh(eval2(x));
		else if (strcmp("atanh", func) == 0) tmp = atanh(eval2(x));
		else if (strcmp("exp", func) == 0) tmp = exp(eval2(x));
		else if (strcmp("log", func) == 0) tmp = log(eval2(x));
		else if (strcmp("log10", func) == 0) tmp = log10(eval2(x));
		else if (strcmp("ceil", func) == 0) tmp = ceil(eval2(x));
		else if (strcmp("floor", func) == 0) tmp = floor(eval2(x));
		else if (strcmp("round", func) == 0) tmp = round(eval2(x));
	}

	if (strcmp("pow", func) == 0 || \
		strcmp("atan2", func) == 0 || \
		strcmp("hypot", func) == 0 || \
		strcmp("mod", func) == 0 || \
		strcmp("fmod", func) == 0 || \
		strcmp("dim", func) == 0 || \
		strcmp("fdim", func) == 0 || \
		strcmp("max", func) == 0 || \
		strcmp("fmax", func) == 0 || \
		strcmp("min", func) == 0 || \
		strcmp("fmin", func) == 0)
	{
		str2[i] = str[i];
		x[0] = '(';
		count = 0;
		for (j = 1; count != 0 || str[i + j] != ','; j++)
		{
			if (str[i + j] == '(') count++;
			else if (str[i + j] == ')') count--;
			else if (str[i + j] == '\0') exit(EXIT_FAILURE);
			str2[i + j] = str[i + j];
			x[j] = str[i + j];
		}
		str2[i + j] = str[i + j];
		x[j] = ')';
		x[j + 1] = '\0';
		y[0] = '(';
		count = 0;
		for (k = 1; count != 0 || str[i + j + k] != ')'; k++)
		{
			if (str[i + j + k] == '(') count++;
			else if (str[i + j + k] == ')') count--;
			else if (str[i + j + k] == '\0') exit(EXIT_FAILURE);
			y[k] = str[i + j + k];
			str2[i + j + k] = str[i + j + k];
		}
		str2[i + j + k] = str[i + j + k];
		str2[i + j + k + 1] = '\0';
		y[k] = ')';
		y[k + 1] = '\0';

		if (strcmp("pow", func) == 0) tmp = pow(eval2(x), eval2(y));
		else if (strcmp("atan2", func) == 0) tmp = atan2(eval2(x), eval2(y));
		else if (strcmp("hypot", func) == 0) tmp = hypot(eval2(x), eval2(y));
		else if (strcmp("mod", func) == 0) tmp = fmod(eval2(x), eval2(y));
		else if (strcmp("fmod", func) == 0) tmp = fmod(eval2(x), eval2(y));
		else if (strcmp("dim", func) == 0) tmp = fdim(eval2(x), eval2(y));
		else if (strcmp("fdim", func) == 0) tmp = fdim(eval2(x), eval2(y));
		else if (strcmp("max", func) == 0) tmp = fmax(eval2(x), eval2(y));
		else if (strcmp("fmax", func) == 0) tmp = fmax(eval2(x), eval2(y));
		else if (strcmp("min", func) == 0) tmp = fmin(eval2(x), eval2(y));
		else if (strcmp("fmin", func) == 0) tmp = fmin(eval2(x), eval2(y));
	}

	if (tmp >= 0) sprintf(val, "%f", tmp);
	else sprintf(val, "(%f)", tmp);
	strrep(str, str2, val);
	printf("func: %s\n", str);

	return tmp;
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
