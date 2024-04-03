/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/03 18:17:28 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// doesn't function properly regarding the -n flag (can be for example: echo -nnn -n -nnnnnn) and if -n flag is in quotes it is seen as literal characters
// tried to fix it but it's not working properly and I got annoyed and did something else

int	builtin_echo(char *args, int out_fd)
{
	char	*temp;

	if (!args)
	{
		ft_fprintf(out_fd, "\n");
		return (0);
	}
	if (ft_strlen(args) >= 3 && ft_strncmp(args, "-n ", 3) == 0)
	{
		temp = ft_substr(args, 3, ft_strlen(args) - 3);
		ft_putstr_fd(temp, out_fd);
		free(temp);
	}
	else if (ft_strlen(args) == 2 && ft_strncmp(args, "-n", 2) == 0)
		ft_fprintf(out_fd, "\n");
	else
		ft_fprintf(out_fd, "%s\n", args);
	return (0);
}

// int	is_only_n(char *args)
// {
// 	int	i;

// 	i = 0;
// 	while (args[i] != '\0')
// 	{
// 		if (args[i] != 'n' && args[i] != '-' && args[i] != ' ')
// 			return (0);
// 		i++;
// 	}
// 	return (1);
// }

// int builtin_echo(char *args, int out_fd)
// {
// 	char	*temp;
// 	int		n_count;
// 	if (!args)
// 	{
// 		ft_fprintf(out_fd, "\n");
// 		return (0);
// 	}
// 	// printf("args: %s\n", args);
// 	if (is_only_n(args))
// 		return (0);
// 	else if (ft_strlen(args) >= 2 && ft_strncmp(args, "-n", 2) == 0)
// 	{
// 		n_count = 2;
// 		while (args[n_count] && (args[n_count] == 'n' || (args[n_count] == '-'
// 			&& args[n_count + 1] == 'n') || args[n_count] == ' '))
// 			n_count++;
// 		temp = ft_substr(args, n_count, ft_strlen(args) - n_count);
// 		ft_putstr_fd(temp, out_fd);
// 		free(temp);
// 	}
// 	else
// 		ft_fprintf(out_fd, "%s\n", args);
// 	return (0);
// }

// int	main(int argc, char **argv)
// {
// 	// int	i;

// 	// i = 1;
// 	if (argc == 1)
// 		printf("\n");
// 	else
// 		builtin_echo(argv[2]);
// 	return (0);
// }
