/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_envp.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:04:21 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/19 18:14:27 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_links(t_env_var **first, t_env_var **prev,
				t_env_var *env_var);
char		**split_envp(char *envp);

t_env_var	*init_envp(char **envp)
{
	char			**temp;
	t_env_var		*env_var;
	t_env_var		*prev;
	t_env_var		*first;

	first = NULL;
	prev = NULL;
	while (*envp)
	{
		temp = split_envp(*envp);
		if (!temp)
			return (free_env_vars(first), NULL);
		if (ft_null_terminated_arr_len((void **)temp) != 2)
			env_var = new_env_var(temp[0], "");
		else
			env_var = new_env_var(temp[0], temp[1]);
		if (!env_var)
			return (free_env_vars(first), NULL);
		ft_free_2d_arr((void **)temp);
		update_links(&first, &prev, env_var);
		envp++;
	}
	return (first);
}

char	**split_envp(char *envp)
{
	char		**key_value;
	int			len;

	key_value = ft_calloc(3, sizeof(char *));
	if (!key_value)
		return (NULL);
	len = 0;
	while (envp[len] && envp[len] != '=')
	{
		if (ft_isspace(envp[len]))
		{
			free(key_value);
			return (NULL);
		}
		len++;
	}
	if (ft_isspace(envp[len + 1]))
	{
		free(key_value);
		return (NULL);
	}
	key_value[0] = ft_substr(envp, 0, len);
	key_value[1] = ft_substr(envp, len + 1, ft_strlen(envp) - len);
	return (key_value);
}

static void	update_links(t_env_var **first, t_env_var **prev,
				t_env_var *env_var)
{
	if (!*prev)
		*first = env_var;
	else
		(*prev)->next = env_var;
	*prev = env_var;
}

// int	main(int argc, char **argv, char **envp)
// {
// 	t_app_data	app_data;

// 	(void)argc;
// 	(void)argv;
// 	app_data.env_vars = init_envp(envp);
// 	while (app_data.env_vars)
// 	{
// 		printf("key: %s | value: %s\n", app_data.env_vars->key,
// 			app_data.env_vars->value);
// 		app_data.env_vars = app_data.env_vars->next;
// 	}
// 	if (!app_data.env_vars)
// 		return (1);
// 	// app_data.input = NULL;
// 	// while (1)
// 	// {
// 	// 	printf("minishell$ ");
// 	// 	get_next_line(0, &app_data.input);
// 	// 	if (!app_data.input)
// 	// 		break ;
// 	// 	lexer(&app_data);
// 	// }
// 	return (0);
// }
