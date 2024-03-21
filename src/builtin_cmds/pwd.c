/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   pwd.c                                              :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: cgerling <cgerling@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:24:10 by lzipp             #+#    #+#             */
/*   Updated: 2024/03/20 11:53:17 by cgerling         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

int	builtin_pwd(char *args)
{
	char	*pwd;

	if (args)
	{
		printf("pwd: too many arguments\n");
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
