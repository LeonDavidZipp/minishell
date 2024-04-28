# Babash
By Charlotte Gerlinger & Leon David Zipp

## Description
The **Minishell Project** at **42 Heilbronn** is a fundamental step in a 42 students' programming journey.
It challenges students to create their own bash-inspired kernel from scratch, capable of interpreting and executing commands entered by the user.
The project is written in C. Many students consider Minishell to be one of the two most important, if not the most important project during the
42 curriculum.

<img width="510" alt="Screen Shot 2024-04-28 at 2 12 35 PM" src="https://github.com/LeonDavidZipp/minishell/assets/117377515/1c7bb2ca-0dc1-4958-a719-b30bd926831e">

## The Main Learnings
### 1. Working as a Team
Minishell is a 2-person project. Since both members have different skills and interests as well as overlapping fields of expertise, the main challenges are:
- fairly distributing the workload between both members
- correctly estimating the skills and interests of both and assigning the correct tasks
- finding compromises if both members dislike a task, especially 42-norminette-fixing :')
- helping each other
- keeping track of each others' progress

### 2. Using git and Github professionally
Our main learnings considering git and Github can be described as follows:
- creating issues for new features or bugs
- working on seperate branches and coordinating progress
- merging into branches when new features work as expected
- resetting faulty changes
- using submodules

### 3. Testing
Since bash handles a myriad commands and arrangements of operators, rigorous testing was necessary to handle as many cases as possible. To do this, we wrote our own tests and used a Minishell tester you can find [here](https://github.com/zstenger93/42_minishell_tester).

<img width="580" alt="Screen Shot 2024-04-28 at 2 24 33 PM" src="https://github.com/LeonDavidZipp/minishell/assets/117377515/df74d95a-d620-4ae2-ba86-588d3cfa5d57">

### 4. Dockerization and Devcontainers

## The 3 Pillars of Minishell
For an **overview of all structs and non-static functions** used in the project, click [here](https://github.com/LeonDavidZipp/minishell/blob/main/inc/minishell.h)
### [1. Parsing & Tokenization](https://github.com/LeonDavidZipp/minishell/tree/main/src/parsing)
The first important step in creating our project:
- **parsing and splitting** the input into **tokens** (an arrangement of characters that has an actual meaning), such as...
  - commands, e.g. ls and exit
  - operators, e.g && (AND) and || (OR)
  - redirections, e.g. > and <
  - etc.
- **rearranging** these tokens according to rules defined by the original bash
- **validating the syntax** of these tokens
- **expanding** hidden values such as **environment variables** or **wildcards**

### [2. Creating an Abstract Syntax Tree (AST)](https://github.com/LeonDavidZipp/minishell/tree/main/src/build_ast)
The second step is to construct an **AST** from the previously generated tokens:

![AST](https://github.com/LeonDavidZipp/minishell/assets/117377515/c2161f98-7bf5-4d80-b052-1055108e4f48)

- the AST serves as an internal representation of the user's input commands.
- it is built and executed using **recursion**.
- the tokens are inserted according to their **priority as defined in bash**.
- because of having to handle brackets, our tree handles the insertion of as many subtrees as required into the main tree.
- the AST facilitates the interpretation and execution of commands entered by the user within the shell environment.

### [3. Execution](https://github.com/LeonDavidZipp/minishell/tree/main/src/execution)
The execution of the command line input is the final integral part of the **Minishell Project**.
The main challenges of this part:
- correctly opening and closing **file descriptors**
- handling **redirections**
- executing **builtin** as well as **other commands**
- executing **commands hidden in environment variables**

### (Inofficial 4th Pillar: Memory Leakage)
Preventing all possible leaks was a requirement as well. To do this, we **deallocated variables as soon as they were no longer needed**.
Another valid approach would be writing a **garbage collector**, but we decided against it.
