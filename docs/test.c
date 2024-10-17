#include <stdio.h>
#include <stdlib.h>
#include <readline/readline.h>
#include <readline/history.h>

static void userprompt(void)
{
    char *input;

    while (1)
    {
        input = readline("minishell> ");
        if (input == NULL)
        {
            printf("Exit \n");
            break ;
        }
        printf("You entered: %s\n", input);
        free(input);
    }
}

int main(void)
{
    userprompt();
    return (0);
}