/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/05 12:05:59 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*remove_n_flags(char *args);

int	builtin_echo(char *args, int out_fd)
{
	char	*temp;
	char	*expanded;
	bool	n_in_string;

	if (!args || (ft_strlen(args) == 2 && ft_strncmp(args, "-n", 2) == 0))
		return (ft_fprintf(out_fd, "\n"), 0);
	n_in_string = false;
	if (args[0] == '\"' || args[0] == '\'')
	{
		expanded = expand_and_remove(args, 0, NULL);
		n_in_string = true;
	}
	if (ft_strlen(args) >= 3 && ft_strncmp(args, "-n ", 3) == 0 && !n_in_string)
	{
		temp = remove_n_flags(args);
		expanded = expand_and_remove(temp, 0, NULL);
		ft_fprintf(out_fd, "%s", expanded);
		free(temp);
		free(expanded);
	}
	else if (n_in_string)
		return(ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	else if (!n_in_string)
		return (ft_fprintf(out_fd, "%s\n", args), 0);
	return (0);
}

static char	*remove_n_flags(char *args)
{
	int i = 0;

	while (ft_strlen(args + i) >= 3 && ft_strncmp(args + i, "-n ", 3) == 0)
		i += 3;
	return ft_substr(args, i, ft_strlen(args) - i);
}

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
