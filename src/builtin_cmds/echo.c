/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/05 14:43:08 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*remove_n_flags(char *args);

int	builtin_echo(char *args, int out_fd, t_app_data *app)
{
	char	*temp;
	char	*expanded;
	bool	n_in_string;

	if (!args || (ft_strlen(args) == 2 && ft_strncmp(args, "-n", 2) == 0))
		return (ft_fprintf(out_fd, "\n"), 0);
	n_in_string = false;
	if (args[0] == '\"' || args[0] == '\'')
	{
		expanded = expand_and_remove(args, app->last_exit_code, app->env_vars);
		n_in_string = true;
	}
	if (ft_strlen(args) >= 3 && ft_strncmp(args, "-n ", 3) == 0 && !n_in_string)
	{
		temp = remove_n_flags(args);
		expanded = expand_and_remove(temp, app->last_exit_code, app->env_vars);
		return(ft_fprintf(out_fd, "\n"), free(temp), free(expanded), 0);
	}
	else if (n_in_string)
		return(ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	else
	{
		expanded = expand_and_remove(args, app->last_exit_code, app->env_vars);
		return (ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	}
}

static char	*remove_n_flags(char *args)
{
	int		i;
	bool	n_flag;

	i = -1;
	while (args[++i])
	{
		while (ft_strlen(args + i) >= 3 && ft_strncmp(args + i, "-n ", 3) == 0)
			i += 3;
		if (ft_strncmp(args + i, "-n", 2) == 0 && args[i + 2] && (args[i + 2] == ' ' || args[i + 2] == 'n'))
		{
			n_flag = true;
			i += 2;
		}
		while (n_flag && args[i] == 'n')
			i++;
	}
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
