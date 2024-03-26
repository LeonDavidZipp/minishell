/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:24:10 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/26 11:07:36 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_pwd(char *args)
{
	char	*pwd;

	if (args)
	{
		ft_fprintf(2, "pwd: too many arguments\n");
		return (1);
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
	{
		perror(NAME);
		return (1);
	}
	else
	{
		printf("%s\n", pwd);
		free(pwd);
	}
	return (0);
}
