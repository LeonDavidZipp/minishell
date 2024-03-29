/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42heilbronn.de>       +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:04:21 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/17 11:17:05 by lzipp            ###   ########.fr       */
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
