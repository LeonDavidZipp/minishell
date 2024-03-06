/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/06 18:43:01 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/06 18:43:06 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_links(t_env_var **first, t_env_var **prev,
				t_env_var *env_var);
char		**split_envp(char *envp);

char	**init_envp(char **env_vars)
{
	int			i;
	char		**envp;

	envp = ft_calloc(ft_null_terminated_arr_len((void **)env_vars) + 1,
			sizeof(char *));
	if (!envp)
		return (NULL);
	while (env_vars[++i])
		envp[i] = ft_strdup(env_vars[i]);
	return (envp);
}


char	**split_path(char *path)
{
	char	**result;
	char	*temp;
	int		i;

	result = ft_split(path, ':');
	if (!result)
		return (NULL);
	i = -1;
	while (result[++i])
	{
		temp = ft_strjoin(result[i], "/");
		free(result[i]);
		result[i] = temp;
	}
	return (result);
}

char	*get_path(t_env_var *env_vars)
{
	t_env_var	*temp;

	temp = env_vars;
	while (temp)
	{
		if (ft_strcmp(temp->key, "PATH") == 0)
			return (ft_strdup(temp->value));
		temp = temp->next;
	}
	return (NULL);
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
