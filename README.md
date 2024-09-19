# Simple Shell

## Overview
This project is a simple Unix shell implementation in C. It supports basic shell functionalities such as executing commands, handling pipes, and maintaining a command history. It also includes a feature to transform a tree-structured command input into a series of piped commands.

## Features
- **Command Execution**: Run commands in the shell using `execvp`.
- **Pipes**: Handle multiple commands connected by pipes (`|`).
- **Command History**: Maintain a history of executed commands and support retrieval of the last command.
- **Tree Transformation**: Convert a tree-structured command input into a series of piped commands.
- **Basic Input Handling**: Handle standard input and remove trailing newlines.

## Usage

### Compile the Shell
To compile the shell, use `gcc`:
```bash
gcc -o simple_shell simple_shell.c


### Run the Shell
Execute the compiled shell:

```bash
./simple_shell