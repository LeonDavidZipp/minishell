# Babash
By Charlotte Gerlinger & Leon David Zipp

<img width="369" alt="Screen Shot 2024-04-28 at 1 25 16 PM" src="https://github.com/LeonDavidZipp/minishell/assets/117377515/25d633c9-eecc-4cd9-98d6-10ad5784a834">

## Description
The Minishell project at École 42 is a fundamental step in learning about operating systems and shell scripting.
It challenges students to create a simple bash-inspired shell from scratch, capable of interpreting and executing commands entered by the user.
The project is written in C. Many students consider Minishell to be one of the two most important, if not the most important project during the
42 curriculum.
### Main Learnings
For an **overview of all structs and non-static functions** used in the projects, click [here](https://github.com/LeonDavidZipp/minishell/blob/main/inc/minishell.h)
#### [1. Parsing & Tokenization](https://github.com/LeonDavidZipp/minishell/tree/main/src/parsing)
The first important step in creating our project:
- parsing and splitting the input into **tokens** (an arrangement of characters that has an actual meaning), such as...
  - commands, e.g. ls and exit
  - operators, e.g && (AND) and || (OR)
  - redirections, e.g. > and <
  - etc.
- rearranging these tokens according to rules defined by the original bash
- validating the **syntax** of these tokens

#### [2. Creating the Abstract Syntax Tree (AST)](https://github.com/LeonDavidZipp/minishell/tree/main/src/build_ast)
The second step was to now build an **AST** from the previously generated tokens.

![image](https://github.com/LeonDavidZipp/minishell/assets/117377515/8571c9ba-3fd7-48db-98d9-446858b97891)

- the AST serves as an internal representation of the user's input commands.
- it is built and executed using recursion
- the tokens are inserted according to their priority as defined in bash
- because of having to handle brackets, our tree handles the insertion of as many subtrees as required into the main tree
- the AST facilitates the interpretation and execution of commands entered by the user within the shell environment
