/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   cd.c                                               :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:56:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/04/06 14:43:19 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../../inc/minishell.h"

// implement cd ~ and cd -
static int	handle_minus(char ***env_vars, int *last_exit_code);
static int	update_pwds(char ***env_vars, int *last_exit_code);

int	builtin_cd(char *path, char ***env_vars, int *last_exit_code)
{
	if (!path)
	{
		path = getenv("HOME");
		if (!path)
		{
			ft_fprintf(2, "%s: cd: HOME not set\n", NAME);
			return (1);
		}
	}
	if (path[0] == '-' && ft_strlen(path) == 1)
		return (handle_minus(env_vars, last_exit_code));
	if (chdir(path) == -1)
	{
		ft_fprintf(2, "%s: cd: %s: %s\n", NAME, path, strerror(errno));
		return (1);
	}
	return (update_pwds(env_vars, last_exit_code));
}


static int	handle_minus(char ***env_vars, int *last_exit_code)
{
	char	*oldpwd;
	char	*pwd;

	oldpwd = ft_getenv("OLDPWD", *env_vars);
	if (!oldpwd)
	{
		ft_fprintf(2, "%s: cd: OLDPWD not set\n", NAME);
		return (1);
	}
	pwd = ft_getenv("PWD", *env_vars);
	if (!pwd)
	{
		ft_fprintf(2, "%s: cd: PWD not set\n", NAME);
		return (1);
	}
	if (chdir(oldpwd) == -1)
	{
		ft_fprintf(2, "%s: cd: %s: %s\n", NAME, oldpwd, strerror(errno));
		return (1);
	}
	update_pwds(env_vars, last_exit_code);
	return (0);
}

static int	update_pwds(char ***env_vars, int *last_exit_code)
{
	char		*oldpwd_string;
	char		*pwd_string;
	char		*cwd_string;
	t_envvar	*new_pwd;
	t_envvar	*new_oldpwd;

	cwd_string = getcwd(NULL, 0);
	pwd_string = ft_getenv("PWD", *env_vars);
	oldpwd_string = ft_strdup("OLDPWD=");
	oldpwd_string = ft_join_in_place(oldpwd_string, pwd_string);
	free(pwd_string);
	pwd_string = ft_sprintf("PWD=%s", cwd_string);
	free(cwd_string);
	new_pwd = split_env_var(pwd_string);
	free(pwd_string);
	new_oldpwd = split_env_var(oldpwd_string);
	free(oldpwd_string);
	*env_vars = update_env_vars(&new_pwd, last_exit_code, *env_vars);
	*env_vars = update_env_vars(&new_oldpwd, last_exit_code, *env_vars);
	return (*last_exit_code != 0);
}
