#include "main.h"

/**
 * get_flags - Determines the active flags
 * @format: Formatted string for argument printing
 * @i: Parameter index
 * Return: Calculated flags
 */
int get_flags(const char *format, int *i)
{
    /* - + 0 # ' ' */
    /* 1 2 4 8 16 */
    int j, curr_i;
    int flags = 0;
    const char FLAGS_CHARS[] = {'-', '+', '0', '#', ' ', '\0'};
    const int FLAGS_ARRAY[] = {F_MINUS, F_PLUS, F_ZERO, F_HASH, F_SPACE, 0};

    for (curr_i = *i + 1; format[curr_i] != '\0'; curr_i++)
    {
        for (j = 0; FLAGS_CHARS[j] != '\0'; j++)
        {
            if (format[curr_i] == FLAGS_CHARS[j])
            {
                flags |= FLAGS_ARRAY[j];
                break;
            }
        }

        if (FLAGS_CHARS[j] == 0)
            break;
    }

    *i = curr_i - 1;

    return (flags);
}
