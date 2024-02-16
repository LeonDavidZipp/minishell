/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 13:19:56 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/16 15:13:12 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

void			update_env_vars(char *key, char *value, t_env_var **env_vars);
t_env_var		*new_env_var(char *key, char *value);
void			free_env_vars(t_env_var *env_var);

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

void	unset_env_var(char *key, t_env_var **env_vars)
{
	t_env_var	*temp;
	t_env_var	*prev;

	temp = *env_vars;
	prev = NULL;
	while (temp)
	{
		if (ft_strncmp(temp->key, key, ft_strlen(key)) == 0)
		{
			if (!prev)
				*env_vars = temp->next;
			else
				prev->next = temp->next;
			free(temp->key);
			free(temp->value);
			free(temp);
			return ;
		}
		prev = temp;
		temp = temp->next;
	}
}

void	free_env_vars(t_env_var *env_var)
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
