#include <stdio.h>
#include <stdlib.h>

int	check_input(char *str)
{
	int	s_quote;
	int	d_quote;
	int	bracket_1;
	int	bracket_2;
	int	i;

	s_quote = 0;
	d_quote = 0;
	bracket_1 = 0;
	bracket_2 = 0;
	i = 0;
	while (str[i])
	{
		if (str[i] == '\'')
			s_quote++;
		if (str[i] == '\"')
			d_quote++;
		if (str[i] == '(')
			bracket_1++;
		if (str[i] == ')')
			bracket_2++;
		i++;
	}
	if (s_quote % 2 != 0)
		return (printf("Error: invalid input - single quotes not closed\n"), 1);
	if (d_quote % 2 != 0)
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