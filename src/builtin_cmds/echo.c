/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/05 17:52:45 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*remove_n_flags(char *args);
static void	process_n_flags(char **args, int *i, int *start, bool *n_flag);
static int	handle_n_flag(char *args, int out_fd, t_app_data *app);

int	builtin_echo(char *args, int out_fd, t_app_data *app)
{
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
	if (ft_strlen(args) >= 3 && ft_strncmp(args, "-n", 2) == 0 && !n_in_string)
		return (handle_n_flag(args, out_fd, app));
	else if (n_in_string)
		return (ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	else
	{
		expanded = expand_and_remove(args, app->last_exit_code, app->env_vars);
		return (ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	}
}

int	handle_n_flag(char *args, int out_fd, t_app_data *app)
{
	char		*temp;
	char		*expanded;

	temp = remove_n_flags(args);
	expanded = expand_and_remove(temp, app->last_exit_code, app->env_vars);
	if (ft_strcmp(args, temp) != 0)
		ft_fprintf(out_fd, "%s", expanded);
	else
		ft_fprintf(out_fd, "%s\n", expanded);
	free(temp);
	free(expanded);
	return (0);
}

static char	*remove_n_flags(char *args)
{
	int		i;
	int		start;
	bool	n_flag;

	i = 0;
	start = 0;
	n_flag = false;
	while (args[i])
	{
		process_n_flags(&args, &i, &start, &n_flag);
		while (n_flag && args[i] == 'n')
			i++;
		if (n_flag && args[i] == ' ')
		{
			n_flag = false;
			start = i + 1;
		}
		else if (n_flag && args[i] != 'n')
			break ;
		i++;
	}
	return (ft_substr(args, start, ft_strlen(args) - start));
}

static void	process_n_flags(char **args, int *i, int *start, bool *n_flag)
{
	while (ft_strlen(*args + *i) >= 3 && ft_strncmp(*args + *i, "-n ", 3) == 0)
	{
		*i += 3;
		*start = *i;
	}
	if (ft_strncmp(*args + *i, "-n", 2) == 0)
	{
		*n_flag = true;
		*i += 2;
	}
}
