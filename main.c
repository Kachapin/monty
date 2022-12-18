Copy code
#include "monty.h"

glob_t global;

/**
*main - entry of the program
*
*@argc: number of arguments
*@argv: argument vectors
*
*Return: 0 on success, 1 on failure
*/
int main(int argc, char **argv)
{
    FILE *f;
    char *line, **command;
    size_t buffersize = 0;
    unsigned int line_number = 1;
    stack_t *head = NULL;
    int ret = 0;

    /* Check for correct number of arguments */
    if (argc != 2)
    {
        fprintf(stderr, "USAGE: monty file\n");
        return 1;
    }

    /* Open file */
    f = fopen(argv[1], "r");
    global.fd = f;
    if (f == NULL)
    {
        fprintf(stderr, "Error: Can't open file %s\n", argv[1]);
        return 1;
    }

    /* Read and execute commands from file */
    while (getline(&line, &buffersize, f) != -1)
    {
        global.line = line;
        command = parser(line);
        if (command == NULL)
            continue;
        if (command[0][0] == '#')
            continue;
        if (check_line(command) == -1)
        {
            fprintf(stderr, "L%u: unknown instruction %s\n", line_number, command[0]);
            ret = 1;
            break;
        }
        exec_cmd(command, line_number, &head);
        line_number++;
    }

    /* Clean up and return */
    free(command);
    _free(&head);
    fclose(f);
    return ret;
}

/**
*parser- parses the commmand input
*
*@buffer: string containing command
*
*Return: Parsed command
*/
char **parser(char *buffer)
{
    char **cmd, *token;
    int i;

    /* Check for empty input */
    if (buffer == NULL)
        return NULL;

    /* Tokenize input */
    token = strtok(buffer, " \t\n\r");
    if (token == NULL)
        return NULL;

    /* Allocate memory for command array */
    cmd = malloc(sizeof(char *) * 1024);
    if (cmd == NULL)
    {
        fprintf(stderr, "Error: malloc failed");
        fclose(global.fd);
        return NULL;
    }

    /* Parse command */
    i = 0;
    while (token)
    {
        cmd[i++] = token;
        token = strtok(NULL, " \t\r\n");
    }
    cmd[i] = NULL;
    return cmd;
}
