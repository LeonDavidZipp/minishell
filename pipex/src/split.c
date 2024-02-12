/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   split.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/01/25 16:55:37 by cgerling          #+#    #+#             */
/*   Updated: 2024/02/06 12:27:21 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/pipex.h"

void	init_struct(t_split *var)
{
	var->flag_1 = 0;
	var->flag_2 = 0;
	var->count = 0;
	var->i = 0;
	var->j = 0;
	var->str_index = 0;
	var->str = NULL;
}

int	count_word(char const *s, char c)
{
	t_split	var;

	init_struct(&var);
	while (s[var.i])
	{
		if (s[var.i] == '\'')
			var.flag_1 = !var.flag_1;
		else if (s[var.i] == '"')
			var.flag_2 = !var.flag_2;
		else if (s[var.i] == c && !var.flag_1 && !var.flag_2)
		{
			if (var.j)
				var.j = 0;
		}
		else
		{
			if (!var.j)
			{
				var.count++;
				var.j = 1;
			}
		}
		var.i++;
	}
	return (var.count);
}

char	*process_substring(t_split *var, char const *s, char c)
{
	while ((s[var->j] && (s[var->j] != c || var->flag_1 || var->flag_2)))
	{
		if (s[var->j] == '\'')
			var->flag_1 = !var->flag_1;
		else if (s[var->j] == '"')
			var->flag_2 = !var->flag_2;
		var->j++;
	}
	if ((s[var->i] == '\'' && s[var->j - 1] == '\'')
		|| (s[var->i] == '"' && s[var->j - 1] == '"'))
		return (ft_substr(s, var->i + 1, var->j - var->i - 2));
	else
		return (ft_substr(s, var->i, var->j - var->i));
}

char	**split(char const *s, char c)
{
	t_split	var;

	init_struct(&var);
	var.str = (char **)malloc(((count_word(s, c)) + 1) * sizeof(char *));
	if (!var.str || !s)
		return (free(var.str), NULL);
	while (var.str_index < (count_word(s, c)))
	{
		while (s[var.i] == c && !var.flag_1 && !var.flag_2)
			var.i++;
		var.j = var.i;
		var.str[var.str_index] = process_substring(&var, s, c);
		if (!var.str[var.str_index])
			return (ft_free_tab(var.str), NULL);
		var.i = var.j;
		var.str_index++;
	}
	var.str[var.str_index] = NULL;
	return (var.str);
}
