/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:04:21 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/19 17:30:32 by lzipp            ###   ########.fr       */
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
	if (ft_isspace(environ[len]))
	{
		free(key_value);
		return (NULL);
	}
	key_value[0] = ft_substr(environ, 0, len);
	key_value[1] = ft_substr(environ, len + 1, ft_strlen(environ) - len);
	return (key_value);
}

int	ft_isspace(int c)
{
	return (c == ' ' || c == '\t' || c == '\n'
		|| c == '\v' || c == '\f' || c == '\r');
}

int	main(void)
{
	char *env_vars[] = {
		"USER= ===1234==6",
		"USER=",
		"USER=abc=def",
		"USER=abc$def&ghi",
		"USER=abc def",
		"USER=1234",
		"",
		NULL
	};

	int i, j;
	char **result;
	for (i = 0; env_vars[i]; i++)
	{
		printf("Testing: |%s|\n", env_vars[i]);
		result = split_environ(env_vars[i]);
		j = 0;
		while (result[j])
		{
			printf("val: |%s|\n", result[j]);
			j++;
		}
		printf("\n");
	}
}


// int	main(int argc, char **argv, char **envp)
// {
// 	t_env_var	*env_vars;
// 	t_env_var	*copy;

// 	(void)argc;
// 	(void)argv;
// 	env_vars = init_environ(envp);
// 	update_env_vars("key1", "value1", &env_vars);
// 	update_env_vars()
// 	t_env_var	*temp = env_vars;
// 	while (temp)
// 	{
// 		printf("key: %s, value: %s\n", temp->key, temp->value);
// 		temp = temp->next;
// 	}
// 	printf("----------\n");
// 	copy = copy_env_vars(env_vars);
// 	temp = copy;
// 	while (temp)
// 	{
// 		printf("key: %s, value: %s\n", temp->key, temp->value);
// 		temp = temp->next;
// 	}
// 	free_env_vars(env_vars);
// 	free_env_vars(copy);
// 	return (0);
// }
