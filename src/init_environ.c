/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 20:16:01 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/16 12:35:03 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// static t_env_var	*create_env_var(char **temp, t_env_var *prev);
void				free_env_vars(t_env_var *env_var);
void				add_or_update_env_var(t_env_var *new, t_env_var **env_vars);
t_env_var			*new_env_var(char *key, char *value, t_env_var *prev);

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
			env_var = new_env_var(temp[0], "", prev);
		else
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

void	add_to_environ(t_env_var *new, t_env_var **env_vars)
{
	t_env_var	*successor;
	t_env_var	*current;

	current = *env_vars;
	if (*env_vars == NULL)
		*env_vars = new;
	else
	{
		while (current->next)
		{
			successor = current->next;
			current = successor;
		}
		current->next = new;
	}
}

void	add_or_update_env_var(t_env_var *new, t_env_var **env_vars)
{
	t_env_var	*successor;
	t_env_var	*current;

	current = *env_vars;
	if (*env_vars == NULL)
		*env_vars = new;
	else
	{
		while (current->next && ft_strncmp(current->key, new->key,
				ft_strlen(new->key)) != 0)
		{
			successor = current->next;
			current = successor;
		}
		if (ft_strncmp(current->key, new->key, ft_strlen(new->key)) == 0)
		{
			free(current->value);
			current->value = ft_strdup(new->value);
			free(new->key);
			free(new->value);
			free(new);
		}
		else
			current->next = new;
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

int	main(int argc, char **argv, char **environ)
{
    t_env_var	*env_var;
    t_env_var	*temp;

    (void)argc;
    (void)argv;
    env_var = init_environ(environ);
	temp = env_var;
    while (temp)
    {
        printf("key: %s, value: %s\n", temp->key, temp->value);
        temp = temp->next;
    }
    printf("\n-----------\n\n");
    t_env_var	*new_var = new_env_var("test", "test", env_var);
	printf("\n\nnew_var->key: %s, new_var->value: %s\n\n", new_var->key, new_var->value);
    add_or_update_env_var(new_var, &env_var);
    temp = env_var;
    while (temp)
    {
        printf("key: %s, value: %s\n", temp->key, temp->value);
        temp = temp->next;
    }
    // free_env_vars(env_var);
    return (0);
}
