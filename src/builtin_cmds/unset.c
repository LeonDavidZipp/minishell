/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:54:39 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 06:44:34 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	remove_env_var(t_env_var **env_vars, t_env_var *prev,
				t_env_var *current);

void	builtin_unset(char *key, t_env_var **env_vars)
{
	t_env_var	*prev;
	t_env_var	*current;
	t_env_var	*next;

	if (!key || ft_strcmp(key, "") == 0)
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	current = *env_vars;
	prev = NULL;
	while (current)
	{
		next = current->next;
		if (ft_strncmp(current->key, key, ft_strlen(key)) == 0)
		{
			remove_env_var(env_vars, prev, current);
			current = next;
		}
		else
		{
			prev = current;
			current = next;
		}
	}
}

static void	remove_env_var(t_env_var **env_vars, t_env_var *prev,
				t_env_var *current)
{
	t_env_var	*next;

	next = current->next;
	if (prev)
		prev->next = next;
	else
		*env_vars = next;
	free(current->key);
	free(current->value);
	free(current);
}

// int	main(void)
// {
//     t_env_var	*env_vars;

//     env_vars = NULL;
//     update_env_vars("TEST1", "test1", &env_vars);
//     update_env_vars("TEST2", "test2", &env_vars);
//     update_env_vars("TEST3", "test3", &env_vars);
//     t_env_var	*temp = env_vars;
//     while (temp)
//     {
//         printf("%s=%s\n", temp->key, temp->value);
//         temp = temp->next;
//     }
//     printf("----------------\n");
//     builtin_unset("TEST", &env_vars);
//     temp = env_vars;
//     while (temp)
//     {
//         printf("%s=%s\n", temp->key, temp->value);
//         temp = temp->next;
//     }
//     printf("----------------\n");
//     return (0);
// }
