/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   env_var_helpers2.c                                 :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/04/19 10:30:05 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/28 14:53:24 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	var_name_valid(char *key, bool includes_equal)
{
	int		i;
	int		len;

	if (!key)
		return (1);
	if (key[0] == '-')
		return (2);
	if (!ft_isalpha(key[0]) && key[0] != '_')
		return (1);
	i = 0;
	len = ft_strlen(key);
	while (key[++i])
	{
		if (i == len - 1 && len > 1 && includes_equal && key[i] == '+')
			break ;
		if (!ft_isalnum(key[i]) && key[i] != '_')
			return (1);
		else if (key[i] == '=')
			return (1);
	}
	return (0);
}

void	handle_non_zero(char **key, int *exit_code)
{
	if (*exit_code == 1)
		ft_fprintf(2, "%s: export: `%s': %s\n", NAME, *key, INVALID_ID);
	else if ((*key)[0] && (*key)[1] && *exit_code == 2)
	{
		ft_fprintf(2, "%s: export: `%c%c': %s\n%s%s", NAME, (*key)[0],
			(*key)[1], INVALID_OP, EXPORT_USG, EXPORT_USG2);
	}
}
