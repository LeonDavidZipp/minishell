/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_vars_2.c                                       :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/03/01 14:38:06 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 15:51:44 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

char	**env_vars_to_char_arr(t_env_var *env_vars)
{
	t_env_var	*temp;
	char		**envp;
	int			i;

	temp = env_vars;
	i = 0;
	while (temp)
	{
		i++;
		temp = temp->next;
	}
	envp = (char **)ft_calloc(i + 1, sizeof(char *));
	if (!envp)
		return (NULL);
	temp = env_vars;
	i = 0;
	while (temp)
	{
		envp[i] = ft_strjoin(temp->key, "=");
		envp[i] = ft_join_in_place(envp[i], temp->value);
		i++;
		temp = temp->next;
	}
	return (envp);
}
