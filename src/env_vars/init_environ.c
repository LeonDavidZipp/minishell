/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:04:21 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/19 10:20:18 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	update_links(t_env_var **first, t_env_var **prev,
				t_env_var *env_var);
char	**split_environ(char *environ);

t_env_var	*init_environ(char **environ)
{
	char			**temp;
	t_env_var		*env_var;
	t_env_var		*prev;
	t_env_var		*first;

	first = NULL;
	prev = NULL;
	while (*environ)
	{
		// temp = ft_split(*environ, '=');
		temp = split_environ(*environ);
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
		environ++;
	}
	return (first);
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

char	**split_environ(char *environ)
{
	char		**key_value;
	int			len;

	key_value = ft_calloc(3, sizeof(char *));
	if (!key_value)
		return (NULL);
	len = 0;
	while (environ[len] && environ[len] != '=')
	{
		if (ft_isspace(environ[len]))
		{
			free(key_value);
			return (NULL);
		}
		len++;
	}
	key_value[0] = ft_substr(environ, 0, len);
	key_value[1] = ft_substr(environ, len + 1, ft_strlen(environ) - len);
	return (key_value);
}

// int	main(void)
// {
// 	char *environ = "USER====1234==6";
// 	char **result = split_environ(environ);
// 	int i = 0;
// 	while (result[i])
// 	{
// 		printf("val: |%s|\n", result[i]);
// 		i++;
// 	}
// }

int	main(int argc, char **argv, char **envp)
{
	t_env_var	*env_vars;
	t_env_var	*copy;

	(void)argc;
	(void)argv;
	env_vars = init_environ(envp);
	update_env_vars("key1", "value1", &env_vars);
	while (env_vars)
	{
		printf("key: %s, value: %s\n", env_vars->key, env_vars->value);
		env_vars = env_vars->next;
	}
	printf("----------\n");
	copy = copy_env_vars(env_vars);
	while (copy)
	{
		printf("key: %s, value: %s\n", copy->key, copy->value);
		copy = copy->next;
	}
	free_env_vars(env_vars);
	free_env_vars(copy);
	return (0);
}
