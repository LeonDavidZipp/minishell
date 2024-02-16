#include <stdio.h>
#include <stdlib.h>

int	check_input(char *str)
{
	int	quote_1;
	int	quote_2;
	int	bracket_1;
	int	bracket_2;
	int	i;

	quote_1 = 0;
	quote_2 = 0;
	bracket_1 = 0;
	bracket_2 = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			quote_1++;
		if (str[i] == '\"')
			quote_2++;
		if (str[i] == '(')
			bracket_1++;
		if (str[i] == ')')
			bracket_2++;
		i++;
	}
	if (quote_1 % 2 != 0)
		return (printf("Error: invalid input - single quotes not closed\n"), 1);
	if (quote_2 % 2 != 0)
		return (printf("Error: invalid input - double quotes not closed\n"), 1);
	if (bracket_1 != bracket_2)
		return (printf("Error: invalid input - brackets not balanced\n"), 1);
	return (0);
}

int	main(int argc, char **argv)
{
	if (check_input(argv[1]) == 0)
		printf("Input is valid\n");
	return (0);
}