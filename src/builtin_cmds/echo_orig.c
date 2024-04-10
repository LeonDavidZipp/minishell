/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   echo.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 16:17:23 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/10 16:54:30 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static char	*remove_n_flags(char *args);
static void	process_n_flags(char **args, int *i, int *start, bool *n_flag);
static int	handle_n_flag(char *args, int out_fd, t_app_data *app);
static bool	n_in_string_with_others(char *args);

int	builtin_echo(char *args, int out_fd, t_app_data *app)
{
	char	*expanded;
	bool	n_in_string;

	if (!args )
		return (ft_fprintf(out_fd, "\n"), 0);
	else if (ft_strlen(args) == 2 && ft_strncmp(args, "-n", 2) == 0)
		return (0);
	n_in_string = n_in_string_with_others(args);
	printf("args = %s\n", args);
	// if (args[0] == '\"' || args[0] == '\'')
	// {
	// 	n_in_string = true;
	// 	expanded = expand_and_remove(args, app->last_exit_code, app->env_vars, 0);
	// }
	if (ft_strlen(args) >= 3 && ft_strncmp(args, "-n", 2) == 0 && !n_in_string)
		return (handle_n_flag(args, out_fd, app));
	else if (n_in_string)
		return (ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	else
	{
		expanded = expand_and_remove(args, app->last_exit_code, app->env_vars, 0);
		return (ft_fprintf(out_fd, "%s\n", expanded), free(expanded), 0);
	}
}

int	handle_n_flag(char *args, int out_fd, t_app_data *app)
{
	char		*temp;
	char		*expanded;

	temp = remove_n_flags(args);
	expanded = expand_and_remove(temp, app->last_exit_code, app->env_vars, 0);
	if (ft_strcmp(args, temp) != 0)
		ft_fprintf(out_fd, "%s", expanded);
	else
		ft_fprintf(out_fd, "%s\n", expanded);
	free(temp);
	free(expanded);
	return (0);
}

int	builtin_echo(char *args, int out_fd, t_app_data *app)
{
	
	char **args_split;

	args_split = split(args);
	if (!args_split)
		return (1);
	int i = -1;
	while (args_split[++i])
	{
		if (i > 0)
			ft_fprintf(out_fd, " ");
		ft_fprintf(out_fd, "%s", args_split[i]);
	}
}

static char	*remove_n_flags(char *args)
{
	int		i;
	int		start;
	int		len;
	bool	n_flag;

	i = 0;
	start = 0;
	len = ft_strlen(args);
	n_flag = false;
	while (args[i])
	{
		process_n_flags(&args, &i, &start, &n_flag);
		if (n_flag && i == len)
			return (ft_strdup(""));
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
	bool 	dquote;
	bool 	quote;

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
	while (*n_flag && (*args)[*i] == 'n')
	(*i)++;
}

static bool	n_in_string_with_others(char *args)
{
	int		i;
	bool	dquote;
	bool	quote;

	i = 0;
	dquote = false;
	quote = false;
	while (args[i])
	{
		if (args[i] == '"' && !quote)
			dquote = !dquote;
		else if (args[i] == '\'' && !dquote)
			quote = !quote;
		i++;
	}
	return (false);
}

static bool	n_in_string_with_others(char *args, int *i)
{
	bool	dquote;
	bool	quote;
	bool	n_found;
	bool	other_found;

	dquote = false;
	quote = false;
	n_found = false;
	other_found = false;
	while (args[*i])
	{
		if (args[*i] == '"' && !quote)
			dquote = !dquote;
		else if (args[*i] == '\'' && !dquote)
			quote = !quote;
		if ((dquote || quote || !(dquote || quote)) && ft_strncmp(args + *i, "-n", 2) == 0)
			n_found = true;
		else if ((dquote || quote) && ft_strncmp(args + *i, other, ft_strlen(other)) == 0)
			other_found = true;
		(*i)++;
	}
	return (n_found && other_found);
}
