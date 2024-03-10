/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   unset.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/22 11:54:39 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/10 14:00:53 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_unset(char *keys, char **env_vars)
{
	if (!keys || ft_strcmp(keys, "") == 0)
	{
		printf("unset: not enough arguments\n");
		return ;
	}
	unset_env_vars(keys, &env_vars);
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
