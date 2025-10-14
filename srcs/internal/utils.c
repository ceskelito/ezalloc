#include "ezalloc_internal.h"

static char *error_handler(int action, const char *str)
{
    static char *last_error = NULL;

    if (action == SET)
    {
        free(last_error);
        if (!str)
            break ;
        last_error = strdup(str);
    }
    else if (action == GET)
        return last_error;
    else if (action == CLEAN)
        free (last_error);
    return NULL;
}

void    set_error(char *str)
{
    error_handler(SET, str);
}

char    *get_error(void)
{
    return (error_handler(GET, NULL));
}

void    free_error(void)
{
    error_handler(CLEAN, NULL);
}