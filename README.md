Simple Shell
Overview
This project is a simple Unix shell implementation in C. It supports basic shell functionalities such as executing commands, handling pipes, and maintaining a command history. It also includes a feature to transform a tree-structured command input into a series of piped commands.

Features
Command Execution: Run commands in the shell using execvp.
Pipes: Handle multiple commands connected by pipes (|).
Command History: Maintain a history of executed commands and support retrieval of the last command.
Tree Transformation: Convert a tree-structured command input into a series of piped commands.
Basic Input Handling: Handle standard input and remove trailing newlines.
Usage
Compile the Shell

To compile the shell, use gcc:

bash
gcc -o simple_shell simple_shell.c
Run the Shell

Execute the compiled shell:

bash
./simple_shell

Basic Commands
Execute a Command: Type the command and press Enter.
Exit the Shell: Type exit and press Enter.
Show History: Type hist and press Enter to display the history of executed commands.
Repeat Last Command: Type !! to repeat the last command executed.
Tree Command Transformation: Prefix a command with Tree to transform a tree-structured input into piped commands.
Pipes

To execute a series of commands connected by pipes:

bash
Copy code
command1 | command2 | command3
For example:

bash
Copy code
ls | grep "txt" | sort
Tree Commands

To use the tree command transformation:

bash
Copy code
Tree (cmd1, (cmd2, (cmd3, cmd4)))
This command structure is parsed into a series of piped commands.

Code Structure
simple_shell.c: The main source file containing the shell implementation.
README.md: This documentation file.
Functions
create_node(const char* data): Creates a new tree node with given data.
free_tree(Node* root): Frees the memory allocated for the tree.
parse_subtree(const char* arr, int start): Parses a subtree from the input string.
build_tree(const char* args): Builds a tree structure from the input string.
inorder_traversal(Node* root, char* result): Performs an inorder traversal of the tree to generate a piped command string.
reformat(const char* input): Reformats the tree command input into a piped command string.
execute_command(char* command): Executes a single command.
handle_pipes(char* input): Handles multiple commands connected by pipes.
main(void): The main function, which reads user input and processes commands.
