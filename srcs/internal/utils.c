#include "ezalloc_internal.h"
#include <errno.h>

/* Manages the last error message (set, get, or clean) */
static char *error_handler(int action, const char *str)
{
    static char *last_error;

    if (action == SET)
    {
        free(last_error);
        last_error = NULL;
        if (!str)
            return (NULL);
        last_error = strdup(str);
        if (!last_error)
            errno = ENOMEM;
    }
    else if (action == GET)
        return (last_error);
    else if (action == CLEAN)
    {
        free (last_error);
        last_error = NULL;
    }
    return (NULL);
}

/* Sets the last error message */
void    set_error(char *str)
{
    error_handler(SET, str);
}

/* Gets the last error message */
char    *get_error(void)
{
    return (error_handler(GET, NULL));
}

/* Frees the error message and cleans up */
void    free_error(void)
{
    error_handler(CLEAN, NULL);
}