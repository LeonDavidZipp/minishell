/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:24:10 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/01 06:14:27 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

void	builtin_pwd(char *args)
{
	char	*pwd;

	if (args)
	{
		printf("pwd: too many arguments\n");
		return ;
	}
	pwd = getcwd(NULL, 0);
	if (!pwd)
		perror(NAME);
	else
	{
		printf("%s\n", pwd);
		free(pwd);
	}
}
