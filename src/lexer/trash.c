#include "../../inc/minishell.h"

// static t_token	*switch_first_arg_for_redir(t_token *token);

// t_token	*switch_args_for_redir(t_token *token)
// {
// 	t_token			*current;
// 	t_token			*temp[4];

// 	token = switch_first_arg_for_redir(token);
// 	current = token;
// 	while (current && current->next && current->next->next
// 		&& current->next->next->next)
// 	{
// 		if ((current->next->type >= REDIR_OUT && current->next->type <= HEREDOC)
// 			&& current->next->next->type == ARG
// 			&& current->next->next->next->type == ARG)
// 		{
// 			temp[0] = current->next;
// 			temp[1] = current->next->next;
// 			temp[2] = current->next->next->next;
// 			temp[3] = current->next->next->next->next;
// 			current->next = temp[2];
// 			current->next->next = temp[0];
// 			current->next->next->next = temp[1];
// 			current->next->next->next->next = temp[3];
// 		}
// 		current = current->next;
// 	}
// 	return (token);
// }

t_token	*switch_args_for_redir(t_token *token)
{
	t_token			*current;
	t_token			*before_first_redir;
	t_token			*temp;

	current = token;
	before_first_redir = NULL;
	while (current)
	{
		if (!before_first_redir && current->type >= REDIR_OUT && current->type <= HEREDOC
			&& current->next && (current->next->type == ARG || current->next->type == CMD)
			&& current->next->next && (current->next->next->type == ARG || current->next->next->type == CMD))
		{
			before_first_redir = current->next->next;
			before_first_redir->next = current;
			before_first_redir->next->next = current->next;
			before_first_redir->next->next->next = current->next->next->next;
			token = before_first_redir;
			current = before_first_redir->next;
		}
		else if (!before_first_redir && current->next && current->next->type >= REDIR_OUT
			&& current->next->type <= HEREDOC)
		{
			before_first_redir = current;
			current = current->next;
		}
		else if (before_first_redir && current->type >= REDIR_OUT && current->type <= HEREDOC
			&& current->next && (current->next->type == ARG || current->next->type == CMD)
			&& current->next->next && (current->next->next->type == ARG || current->next->next->type == CMD))
		{
			temp = current->next->next; // Save the second argument
			current->next->next = temp->next; // Link the first argument to the rest of the list
			temp->next = current; // Link the second argument to the redirection operator
			before_first_redir->next = temp;
			current = current->next;
			before_first_redir = before_first_redir->next;
			printf("before_first_redir->content: %s\n", before_first_redir->content);
		}
		else
			current = current->next;
		printf("still going\n");
	}
	return (token);
}

// static t_token	*switch_first_arg_for_redir(t_token *token)
// {
// 	t_token			*current;
// 	t_token			*temp[4];

// 	current = token;
// 	if (current && (current->type >= REDIR_OUT && current->type <= HEREDOC)
// 		&& current->next && current->next->type == ARG
// 		&& current->next->next && current->next->next->type == ARG)
// 	{
// 		temp[0] = current;
// 		temp[1] = current->next;
// 		temp[2] = current->next->next;
// 		temp[3] = current->next->next->next;
// 		current = temp[2];
// 		current->type = CMD;
// 		current->next = temp[0];
// 		current->next->next = temp[1];
// 		current->next->next->next = temp[3];
// 		token = current;
// 	}
// 	return (token);
// }