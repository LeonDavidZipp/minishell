/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   minishell.h                                        :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/04 10:58:02 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 16:02:37 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef MINISHELL_H
# define MINISHELL_H

# include "../lib/libft/libft.h"
# include <stdio.h>
# include <stdlib.h>
# include <unistd.h>
# include <sys/wait.h>
# include <signal.h>
# include <sys/stat.h>
# include <dirent.h>
# include <string.h>
# include <errno.h>
# include <termios.h>
# include <term.h>
# include <fcntl.h>
# include <sys/ioctl.h>
# include <readline/readline.h>
# include <readline/history.h>

# define PROMPT "\033[0;36mchl#>	\033[0m"

typedef enum e_token
{
	SEMICOLON,
	ARG,
	FLAG,
	BUILTIN_CMD,
	OTHER_CMD,
	SINGLE_QUOTE,
	DOUBLE_QUOTE,
	PIPE,
	AND,
	OR,
	REDIR_OUT,
	REDIR_IN,
	REDIR_APPEND,
	REDIR_INPUT,
	HEREDOC,
	ENV_VAR,
	WILDCARD
}			t_token;

typedef struct s_input
{
	char	*content;
	int		type;
}			t_input;

typedef struct s_treenode
{
	char				*content;
	int					type;
	struct s_treenode	*left;
	struct s_treenode	*right;
}					t_treenode;

// signal handling
void		signal_handler(void);

#endif
