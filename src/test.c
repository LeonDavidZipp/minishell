#include "../inc/minishell.h"

static t_env_var	*create_env_var(char **temp, t_env_var *prev);
static void			free_env_vars(t_env_var *env_var);

t_env_var	*init_environ(char **environ)
{
	char			**temp;
	t_env_var		*env_var;
	t_env_var		*prev;
	t_env_var		*first;

	first = NULL;
	prev = NULL;
	while (*environ)
	{
		temp = ft_split(*environ, '=');
		if (!temp)
			return (free_env_vars(first), NULL);
		env_var = create_env_var(temp, prev);
		if (!env_var)
			return (free_env_vars(first), NULL);
		free(temp);
		if (!prev)
			first = env_var;
		prev = env_var;
		environ++;
	}
	return (first);
}

static t_env_var	*create_env_var(char **temp, t_env_var *prev)
{
	t_env_var	*env_var;

	env_var = (t_env_var *)malloc(sizeof(t_env_var));
	if (!env_var)
		return (NULL);
	env_var->key = temp[0];
	env_var->value = temp[1];
	env_var->next = NULL;
	if (prev)
		prev->next = env_var;
	return (env_var);
}

static void	free_env_vars(t_env_var *env_var)
{
	t_env_var	*temp;

	while (env_var)
	{
		temp = env_var->next;
		free(env_var->key);
		free(env_var->value);
		free(env_var);
		env_var = temp;
	}
}

// int main(int argc, char **argv, char **environ) {
//     printf("Number of command-line arguments: %d\n", argc);
//     for (int i = 0; i < argc; i++) {
//         printf("argv[%d] = %s\n", i, argv[i]);
//     }
//     for (char **env = environ; *env != NULL; env++) {
//         printf("%s\n", *env);
//     }
//     return 0;
// }

int main(int argc, char **argv, char **environ) {
    printf("Number of command-line arguments: %d\n", argc);
    for (int i = 0; i < argc; i++) {
        printf("argv[%d] = %s\n", i, argv[i]);
    }

    // Call init_environ
    t_env_var *env_var = init_environ(environ);
	t_env_var *start = env_var;
	printf("environ initialized\n");
    // Print out the processed environment variables
    while (env_var) {
		printf("key = %s || value = %s\n", env_var->key, env_var->value);
		env_var = env_var->next;
	}
	free_env_vars(start);
    // TODO: Free the env_vars linked list when you're done with it
    return 0;
}
// 