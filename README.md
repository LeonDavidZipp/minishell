# Minishell
Using preprogrammed shells is boring, how about we create our own?

## Good to know
- !!!one global variable!!!

- The readline() function can cause memory leaks. You don’t have to fix them. But
that doesn’t mean your own code, yes the code you wrote, can have memory
leaks.

https://reactive.so/post/42-a-comprehensive-guide-to-pipex
https://achrafbelarif.medium.com/42cursus-minishell-29cd25f972e6
https://github.com/zstenger93/42_minishell_tester

## Prerequisites
### Display Prompts
### Parsing
- operators: no surrounding spaces needed
- commands: spaces needed
- flags: no spaces needed, BUT if two flags are together and not immediately after (e.g. -la -> -l -a, NOT -l-a), space is needed between flags
### Pipes & Redirections
### Signal Handling
#### - Handle ctrl-C, ctrl-D and ctrl-\
  - ctrl-C displays a new prompt on a new line.
  - ctrl-D exits the shell.
  - ctrl-\ does nothing.
### Command Execution (includes file path handling)
- NOT Handle single ", ', ;, \
### Built in Commands
- echo with option -n
- cd with only a relative or absolute path
- pwd with no options
- export with no options
- unset with no options
- env with no options or arguments
- exit with no options
### Env Vars
### History

## Bonus
### && and ||
### * for currend working dir

