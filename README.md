# Simple Shell

## Overview

`simple_shell` is a basic shell implementation that supports command execution, history management, and tree-structured command transformations. This shell allows users to execute commands, view command history, repeat previous commands, and utilize pipes and tree structures to manage complex command sequences.

## Basic Commands

- **Execute a Command**: Type the command and press Enter.
- **Exit the Shell**: Type `exit` and press Enter.
- **Show History**: Type `hist` and press Enter to display the history of executed commands.
- **Repeat Last Command**: Type `!!` to repeat the last command executed.
- **Tree Command Transformation**: Prefix a command with `Tree` to transform a tree-structured input into piped commands.

## Pipes

To execute a series of commands connected by pipes, use the following syntax:

```bash
command1 | command2 | command3
```

**Example:**

```bash
ls | grep "txt" | sort
```

## Tree Commands

To use the tree command transformation, employ the following structure:

```bash
Tree (cmd1, (cmd2, (cmd3, cmd4)))
```

This structure is parsed into a series of piped commands.

## Code Structure

- `simple_shell.c`: The main source file containing the shell implementation.
- `README.md`: This documentation file.

## Functions

- **`create_node(const char *data)`**: Creates a new tree node with the given data.
- **`free_tree(Node root)`**: Frees the memory allocated for the tree.
- **`parse_subtree(const char *arr, int start)`**: Parses a subtree from the input string.
- **`build_tree(const char *args)`**: Builds a tree structure from the input string.
- **`inorder_traversal(Node root, char *result)`**: Performs an inorder traversal of the tree to generate a piped command string.
- **`reformat(const char *input)`**: Reformats the tree command input into a piped command string.
- **`execute_command(const char *command)`**: Executes a single command.
- **`handle_pipes(const char *input)`**: Handles multiple commands connected by pipes.
- **`main(void)`**: The main function, which reads user input and processes commands.
