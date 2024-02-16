/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:19:56 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/16 14:22:56 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void			update_env_vars(char *key, char *value, t_env_var **env_vars);
t_env_var		*new_env_var(char *key, char *value);
static void		free_env_vars(t_env_var *env_var);
static void		update_links(t_env_var **first, t_env_var **prev,
					t_env_var *env_var);

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

void	update_env_vars(char *key, char *value, t_env_var **env_vars)
{
	t_env_var	*temp;
	t_env_var	*new_var;

	temp = *env_vars;
	if (!temp)
	{
		*env_vars = new_env_var(key, value);
		return ;
	}
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
		{
			free(temp->value);
			temp->value = ft_strdup(value);
			return ;
		}
		if (temp->next == NULL)
			break ;
		temp = temp->next;
	}
	new_var = new_env_var(key, value);
	temp->next = new_var;
}

t_env_var	*new_env_var(char *key, char *value)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	env_var->key = ft_strdup(key);
	if (!env_var->key)
	{
		free(env_var);
		return (NULL);
	}
	if (!value)
		env_var->value = ft_strdup("");
	else
		env_var->value = ft_strdup(value);
	env_var->next = NULL;
	return (env_var);
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
