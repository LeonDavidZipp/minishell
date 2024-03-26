/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   need_name.c                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/27 15:52:00 by cgerling          #+#    #+#             */
/*   Updated: 2024/03/26 11:17:18 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

static void	init(int *i, bool *flags)
{
	i[0] = 0;
	i[1] = 0;
	flags[0] = false;
	flags[1] = false;
}

char	*remove_quotes(char *str)
{
	char	*output;
	bool	flags[2];
	int		i[2];

	init(i, flags);
	output = malloc(ft_strlen(str) + 1);
	if (!output)
		return (NULL);
	while (str[i[0]])
	{
		handle_quotes(str[i[0]], &flags[0], &flags[1]);
		if (str[i[0]] == '\'' && !flags[1])
		{
			i[0]++;
			continue ;
		}
		if (str[i[0]] == '\"' && !flags[0])
		{
			i[0]++;
			continue ;
		}
		output[i[1]++] = str[i[0]++];
	}
	output[i[1]] = '\0';
	return (output);
}

// char	**fill_new_str(char **str, char **new_str, int exit_code)
// {
// 	char	*temp;
// 	char	*expanded;
// 	int		is_heredoc;
// 	int		i;

// 	i = 0;
// 	is_heredoc = 0;
// 	while (str[i])
// 	{
// 		expanded = expand(str[i], exit_code, 0);
// 		if (!expanded)
// 			return (ft_free_2d_arr((void **)str), \
// 			ft_free_2d_arr((void **)new_str), NULL);
// 		if (i > 0 && ft_strcmp(str[i - 1], "<<") == 0)
// 			is_heredoc = 1;
// 		if (is_heredoc == 1)
// 		{
// 			new_str[i] = expanded;
// 			is_heredoc = 0;
// 		}
// 		else
// 		{
// 			temp = remove_quotes(expanded);
// 			if (!temp)
// 				return (ft_free_2d_arr((void **)str), \
// 				ft_free_2d_arr((void **)new_str), free(expanded), NULL);
// 			new_str[i] = temp;
// 			free(expanded);
// 		}
// 		i++;
// 	}
// 	ft_free_2d_arr((void **)str);
// 	return (new_str);
// }

// char	**expand_and_remove(char **str, int exit_code)
// {
// 	char	**new_str;
// 	int		i;

// 	i = 0;
// 	while (str[i])
// 		i++;
// 	new_str = malloc(sizeof(char *) * (i + 1));
// 	if (!new_str)
// 		return (ft_free_2d_arr((void **)str), NULL);
// 	new_str[i] = NULL;
// 	return (fill_new_str(str, new_str, exit_code));
// }

char	*expand_and_remove(char *str, int exit_code)
{
	char	*expanded;
	char	*result;
	
	expanded = expand(str, exit_code, 0);
	if (!expanded)
		return (NULL);
	result = remove_quotes(expanded);
	if (!result)
		return (free(expanded), NULL);
	free(expanded);
	return (result);
}

// int main()
// {
// 	char *str = "'\"'\"'\"hello\"'\"'\"'";
// 	printf("%s\n", remove_quotes(str));
// 	return 0;
// }