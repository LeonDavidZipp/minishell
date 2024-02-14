/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 20:16:01 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 20:24:09 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

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
		temp = ft_split(*environ, '=');
		if (!temp)
			return (free_env_vars(first), NULL);
		env_var = create_env_var(temp, prev);
		if (!env_var)
			return (free_env_vars(first), NULL);
		free(temp);
		if (!prev)
			first = env_var;
		prev = env_var;
		environ++;
	}
	return (first);
}

static t_env_var	*create_env_var(char **temp, t_env_var *prev)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	env_var->key = temp[0];
	env_var->value = temp[1];
	env_var->next = NULL;
	if (prev)
		prev->next = env_var;
	return (env_var);
}

static void	free_env_vars(t_env_var *env_var)
{
	t_env_var	*temp;

	while (env_var)
	{
		temp = env_var->next;
		free(env_var->key);
		free(env_var->value);
		free(env_var);
		env_var = temp;
	}
}
