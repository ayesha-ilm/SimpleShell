#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <sys/wait.h>

#define MAX_LINE 80
#define MAX_TOKENS 40
#define MAX_INPUT 80
#define MAX_PIPE 10

typedef struct Node 
{
    char *data;
    struct Node *left_child;
    struct Node *right_child;
} Node;

Node* create_node(const char* data) 
{
    Node *node = malloc(sizeof(Node));
    if (!node) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    node->data = strdup(data);
    if (!node->data) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    node->left_child = NULL;
    node->right_child = NULL;
    return node;
}

void free_tree(Node* root) 
{
    if (root) {
        free_tree(root->left_child);
        free_tree(root->right_child);
        free(root->data);
        free(root);
    }
}

char* parse_subtree(const char* arr, int start) 
{
    char* subtree = malloc(30 * sizeof(char));
    if (!subtree) {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    int i = 0, count = 1;
    subtree[i++] = '(';
    for (int j = start + 1; count != 0; j++) 
    {
        if (arr[j] == '(') count++;
        if (arr[j] == ')') count--;
        subtree[i++] = arr[j];
    }
    subtree[i] = '\0';
    return subtree;
}

Node* build_tree(const char* args) 
{
    Node* root = create_node("");

    int start = 1;
    while (args[start] != ',' && args[start] != '\0') start++;
    int length = start - 1;
    
    if (length > 0) {
        root->data = strndup(&args[1], length);
    }
    
    if (args[start] == ',') 
    {
        start++;
        char* right_args = parse_subtree(args, start);
        int right_length = strlen(right_args);
        if (right_length > 2) 
        {
            root->right_child = build_tree(right_args);
        }
        free(right_args);

        char* left_args = parse_subtree(args, start + right_length);
        int left_length = strlen(left_args);
        if (left_length > 2) 
        {
            root->left_child = build_tree(left_args);
        }
        free(left_args);
    }

    return root;
}

void inorder_traversal(Node* root, char* result) 
{
    if (root) 
    {
        inorder_traversal(root->left_child, result);
        strcat(result, root->data);
        strcat(result, " | ");
        inorder_traversal(root->right_child, result);
    }
}

char* reformat(const char* input) 
{
    char* formatted = malloc(MAX_INPUT * sizeof(char));
    if (!formatted) 
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }
    formatted[0] = '\0';

    char* trimmed_input = strdup(input + 5);
    if (!trimmed_input) 
    {
        perror("Failed to allocate memory");
        exit(EXIT_FAILURE);
    }

    Node* tree_head = build_tree(trimmed_input);
    inorder_traversal(tree_head, formatted);

    size_t len = strlen(formatted);
    if (len > 3) formatted[len - 3] = '\0';  // Remove trailing " | "

    free(trimmed_input);
    free_tree(tree_head);
    return formatted;
}

void execute_command(char* command) 
{
    char* args[MAX_TOKENS];
    char* token = strtok(command, " ");
    int i = 0;
    while (token != NULL) 
    {
        args[i++] = token;
        token = strtok(NULL, " ");
    }
    args[i] = NULL;

    if (execvp(args[0], args) == -1) 
    {
        perror("execvp failed");
        exit(EXIT_FAILURE);
    }
}

void handle_pipes(char* input) 
{
    char* commands[MAX_PIPE];
    int num_commands = 0;

    char* token = strtok(input, "|");
    while (token != NULL && num_commands < MAX_PIPE) 
    {
        commands[num_commands++] = token;
        token = strtok(NULL, "|");
    }

    int pipefds[2 * (num_commands - 1)];

    for (int i = 0; i < num_commands - 1; i++) 
    {
        if (pipe(pipefds + i * 2) == -1) {
            perror("pipe failed");
            exit(EXIT_FAILURE);
        }
    }

    for (int i = 0; i < num_commands; i++) 
    {
        pid_t pid = fork();
        if (pid == -1) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }

        if (pid == 0) {
            if (i > 0) {
                if (dup2(pipefds[(i - 1) * 2], STDIN_FILENO) == -1) 
                {
                    perror("dup2 failed");
                    exit(EXIT_FAILURE);
                }
            }

            if (i < num_commands - 1) {
                if (dup2(pipefds[i * 2 + 1], STDOUT_FILENO) == -1) 
                {
                    perror("dup2 failed");
                    exit(EXIT_FAILURE);
                }
            }

            for (int j = 0; j < 2 * (num_commands - 1); j++) 
            {
                close(pipefds[j]);
            }

            execute_command(commands[i]);
        }
    }

    for (int i = 0; i < 2 * (num_commands - 1); i++) 
    {
        close(pipefds[i]);
    }

    for (int i = 0; i < num_commands; i++) 
    {
        if (wait(NULL) == -1) {
            perror("wait failed");
        }
    }
}

int main(void) {
    char *args[MAX_TOKENS];
    Node *tail = NULL;
    int len_linked_list = 0;

    while (1) {
        printf("\n%s", "osh>");
        fflush(stdout);

        char input[MAX_INPUT];
        char input_maker[MAX_INPUT] = "";
        fgets(input, MAX_INPUT, stdin);

        char *newline = strchr(input, '\n');
        if (newline) *newline = '\0';

        if (strcmp(input, "exit") == 0) 
        {
            break;
        }

        if (strncmp(input, "Tree ", 5) == 0) 
        {
            char* new_input = reformat(input);
            strcpy(input, new_input);
            free(new_input);
        }

        if (strcmp(input, "!!") == 0) 
        {
            if (tail == NULL) 
            {
                printf("No command found\n");
            } else {
                strcpy(input, tail->data);
            }
        }

        if (strcmp(input, "hist") == 0) {
            if (tail == NULL) 
            {
                printf("No history exists\n");
            } 
            else 
            {
                Node* temp = tail;
                do 
                {
                    printf("%s\n", temp->data);
                    temp = temp->right_child;
                } while (temp != tail);
            }
            continue;
        }

        if (strcmp(input, "!!") != 0 && strcmp(input, "hist") != 0) {
            Node* new_node = create_node(input);
            if (tail == NULL) 
            {
                tail = new_node;
                tail->right_child = tail;
            } else 
            {
                new_node->right_child = tail->right_child;
                tail->right_child = new_node;
            }
            len_linked_list++;
        }

        char* pipe_check = strchr(input, '|');
        if (pipe_check) 
        {
            handle_pipes(input);
        } else {
            char* token = strtok(input, " ");
            int i = 0;
            while (token != NULL) 
            {
                args[i++] = token;
                token = strtok(NULL, " ");
            }
            args[i] = NULL;

            pid_t pid = fork();
            if (pid < 0) 
            {
                perror("fork failed");
                exit(EXIT_FAILURE);
            }

            if (pid == 0) 
            {  
                //child process
                execvp(args[0], args);
                perror("execvp failed");
                exit(EXIT_FAILURE);
            } 
            else 
            {  
                //parent process
                if (wait(NULL) == -1) {
                    perror("wait failed");
                }
            }
        }
    }

    return 0;
}
