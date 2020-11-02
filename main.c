#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

int check(char *str)
{
	int n, n0 = -1;
	char token[10];
	struct count c;
	memset((void *)&c, 0, sizeof(struct count));

	while (*str != '\0')
	{
		switch(n = gettoken(str, token))
		{
			case NUMBER:
			{
				if (n0 == -1 || n0 == OPERATOR || n0 == LEFTPAREN) c.number++;
				else return -1;
				break;
			}
			case OPERATOR:
			{
				if (n0 == RIGHTPAREN || n0 == NUMBER) c.operator++;
				else if ((token[0] == '+' || token[0] == '-') && (n0 == -1 || n0 == LEFTPAREN)) c.sign++;
				else return -1;
				break;
			}
			case LEFTPAREN:
			{
				if (n0 == -1 || n0 == OPERATOR || n0 == LEFTPAREN) c.leftparen++;
				else return -1;
				break;
			}
			case RIGHTPAREN:
			{
				if (n0 == NUMBER || n0 == RIGHTPAREN) c.rightparen++;
				else return -1;
				break;
			}
			case '\0':
			{
				break;
			}
			default:
			{
				return -1;
			}
		}
		n0 = n;
		if (c.leftparen - c.rightparen < 0)
		{
			return -1;
		}
		str = str + strlen(token);
	}
	if (n != NUMBER && n != RIGHTPAREN)
	{
		if (c.leftparen - c.rightparen != 0) return -1;
	}

	return 0;
}

void init(const char in[], char out[])
{
	sprintf(out, "(%s)", in);
	while (strchr(out, " ")) strrep(out, " ", "");
}

int main(int argc, char **argv)
{
	char str[STR_LENGTH];

	if (argc != 2)
	{
		fprintf(stderr, "Usage: %s \"func\"\n", argv[0]);
		return EXIT_FAILURE;
	}

	init(argv[1], str);

	return 0;
}
