/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:54:39 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/06 19:52:33 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// static void	unset_vars(char **key_arr, t_env_var **env_vars);
// static void	remove_env_var(t_env_var **env_vars, t_env_var *prev,
// 				t_env_var *current);

// void	builtin_unset(char *keys, t_env_var **env_vars)
// {
// 	char		**key_arr;

// 	if (!keys || ft_strcmp(keys, "") == 0)
// 	{
// 		printf("unset: not enough arguments\n");
// 		return ;
// 	}
// 	key_arr = ft_split(keys, ' ');
// 	if (!key_arr)
// 		return ;
// 	unset_vars(key_arr, env_vars);
// 	ft_free_2d_arr((void **)key_arr);
// }

// static void	unset_vars(char **key_arr, t_env_var **env_vars)
// {
// 	t_env_var	*prev;
// 	t_env_var	*current;
// 	t_env_var	*next;
// 	int			i;

// 	i = -1;
// 	while (key_arr[++i])
// 	{
// 		current = *env_vars;
// 		prev = NULL;
// 		while (current)
// 		{
// 			next = current->next;
// 			if (ft_strcmp(current->key, key_arr[i]) == 0)
// 			{
// 				remove_env_var(env_vars, prev, current);
// 				current = next;
// 			}
// 			else
// 			{
// 				prev = current;
// 				current = next;
// 			}
// 		}
// 	}
// }

// static void	remove_env_var(t_env_var **env_vars, t_env_var *prev,
// 				t_env_var *current)
// {
// 	t_env_var	*next;

// 	next = current->next;
// 	if (prev)
// 		prev->next = next;
// 	else
// 		*env_vars = next;
// 	free(current->key);
// 	free(current->value);
// 	free(current);
// }

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
//     builtin_unset("TEST1 TEST2", &env_vars);
//     temp = env_vars;
//     while (temp)
//     {
//         printf("%s=%s\n", temp->key, temp->value);
//         temp = temp->next;
//     }
//     printf("----------------\n");
//     return (0);
// }
