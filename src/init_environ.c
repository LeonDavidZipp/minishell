/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 20:16:01 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/15 14:18:23 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static t_env_var	*create_env_var(char **temp, t_env_var *prev);
static void			free_env_vars(t_env_var *env_var);

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
		env_var = new_env_var(temp[0], temp[1], prev);
		if (!env_var)
			return (free_env_vars(first), NULL);
		ft_free_2d_arr((void **)temp);
		if (!prev)
			first = env_var;
		prev = env_var;
		environ++;
	}
	return (first);
}

void	add_to_environ(t_env_var *new, t_env_var **env_vars)
{
	t_list	*successor;
	t_list	*current;

	current = *env_vars;
	if (*env_vars == NULL)
		*env_vars = new;
	else
	{
		while (current->next != NULL)
		{
			successor = current->next;
			current = successor;
		}
		current->next = new;
	}
}

t_env_var	*new_env_var(char *key, char *value, t_env_var *prev)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	env_var->key = ft_strdup(key);
	env_var->value = ft_strdup(value);
	if (!env_var->key || !env_var->value)
	{
		free(env_var->key);
		free(env_var->value);
		free(env_var);
		return (NULL);
	}
	env_var->next = NULL;
	if (prev)
		prev->next = env_var;
	return (env_var);
}

void	update_env_var(t_env_var *env_var, char *value)
{
	free(env_var->value);
	env_var->value = ft_strdup(value);
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

int	main(int argc, char **argv, char **environ)
{
	t_env_var	*env_var;

	(void)argc;
	(void)argv;
	env_var = init_environ(environ);
	if (!env_var)
		return (1);
	t_env_var	*temp = new_env_var("test", "test", env_var);
	free_env_vars(env_var);
	return (0);
}
