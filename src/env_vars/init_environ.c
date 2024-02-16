/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   init_environ.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/16 15:04:21 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/16 15:11:25 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

static void	update_links(t_env_var **first, t_env_var **prev,
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

static void	update_links(t_env_var **first, t_env_var **prev,
				t_env_var *env_var)
{
	if (!*prev)
		*first = env_var;
	else
		(*prev)->next = env_var;
	*prev = env_var;
}
