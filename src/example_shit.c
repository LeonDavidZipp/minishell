/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   example_shit.c                                     :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: lzipp <lzipp@student.42.fr>                +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/02/14 15:04:54 by lzipp             #+#    #+#             */
/*   Updated: 2024/02/14 15:06:25 by lzipp            ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../inc/minishell.h"

// examples on how to convert parsed input into tokens, commands, etc.

char* token = strtok(command, " ");
while (token != NULL) {
	// process the token
	token = strtok(NULL, " ");
}

t_treenode* parse(char** tokens) {
	char* token = next_token(tokens);
	if (is_operator(token)) {
		t_treenode* node = new_node(token);
		node->left = parse(tokens);
		node->right = parse(tokens);
		return node;
	} else {
		return new_node(token);
	}
}

void execute(t_treenode* node) {
	if (is_operator(node->content)) {
		// execute the operator with its operands
		execute(node->left);
		execute(node->right);
	} else {
		// execute the command
	}
}
