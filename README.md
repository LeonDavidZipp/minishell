# Babash
By Charlotte Gerlinger & Leon David Zipp

<img width="369" alt="Screen Shot 2024-04-28 at 1 25 16 PM" src="https://github.com/LeonDavidZipp/minishell/assets/117377515/25d633c9-eecc-4cd9-98d6-10ad5784a834">

## Description
The Minishell project at École 42 is a fundamental step in learning about operating systems and shell scripting.
It challenges students to create a simple bash-inspired shell from scratch, capable of interpreting and executing commands entered by the user.
The project is written in C. Many students consider Minishell to be one of the two most important, if not the most important project during the
42 curriculum.
### Main Learnings
#### [1. Parsing & Tokenization](https://github.com/LeonDavidZipp/minishell/tree/main/src/parsing)
The first important steps in creating our project:
- parsing and splitting the input into tokens, such as...
  - commands, e.g. ls and exit
  - operators, e.g && (AND) and || (OR)
  - redirections, e.g. > and <
  - etc.
- rearranging these tokens according to rules defined by the original bash
- validating the syntax of these tokens

#### 2. Creating the Abstract Syntax Tree (AST)
