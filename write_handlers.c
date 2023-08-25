#include "main.h"

/************************* WRITE HANDLE *************************/
/**
 * Prints a character.
 *
 * @param c - The character to print.
 * @param buffer - Buffer array to handle print.
 * @param flags - Active flags.
 * @param width - Width specifier.
 * @param precision - Precision specifier.
 * @param size - Size specifier.
 *
 * @return Number of characters printed.
 */
int handle_write_char(char c, char buffer[],
    int flags, int width, int precision, int size)
{
    int i = 0;
    char padding = ' ';

    UNUSED(precision);
    UNUSED(size);

    if (flags & F_ZERO)
        padding = '0';

    buffer[i++] = c;
    buffer[i] = '\0';

    if (width > 1)
    {
        buffer[BUFF_SIZE - 1] = '\0';
        for (i = 0; i < width - 1; i++)
            buffer[BUFF_SIZE - i - 2] = padding;

        if (flags & F_MINUS)
            return (write(1, &buffer[0], 1) +
                    write(1, &buffer[BUFF_SIZE - i - 1], width - 1));
        else
            return (write(1, &buffer[BUFF_SIZE - i - 1], width - 1) +
                    write(1, &buffer[0], 1));
    }

    return (write(1, &buffer[0], 1));
}

/************************* WRITE NUMBER *************************/
/**
 * Prints a number.
 *
 * @param is_negative - Flag indicating if the number is negative.
 * @param ind - Index where the number starts in the buffer.
 * @param buffer - Buffer array to handle print.
 * @param flags - Active flags.
 * @param width - Width specifier.
 * @param precision - Precision specifier.
 * @param size - Size specifier.
 *
 * @return Number of characters printed.
 */
int write_number(int is_negative, int ind, char buffer[],
    int flags, int width, int precision, int size)
{
    int length = BUFF_SIZE - ind - 1;
    char padding = ' ', extra_char = 0;

    UNUSED(size);

    if ((flags & F_ZERO) && !(flags & F_MINUS))
        padding = '0';
    if (is_negative)
        extra_char = '-';
    else if (flags & F_PLUS)
        extra_char = '+';
    else if (flags & F_SPACE)
        extra_char = ' ';

    return (write_num(ind, buffer, flags, width, precision,
        length, padding, extra_char));
}

/**
 * Writes a number using the buffer.
 *
 * @param ind - Index where the number starts in the buffer.
 * @param buffer - Buffer array to handle print.
 * @param flags - Active flags.
 * @param width - Width specifier.
 * @param prec - Precision specifier.
 * @param length - Number length.
 * @param padding - Padding character.
 * @param extra_char - Extra character.
 *
 * @return Number of printed characters.
 */
int write_num(int ind, char buffer[],
    int flags, int width, int prec,
    int length, char padding, char extra_char)
{
    int i, padding_start = 1;

    if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0' && width == 0)
        return 0;
    if (prec == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
        buffer[ind] = padding = ' ';
    if (prec > 0 && prec < length)
        padding = ' ';
    while (prec > length)
        buffer[--ind] = '0', length++;
    if (extra_char != 0)
        length++;
    if (width > length)
    {
        for (i = 1; i < width - length + 1; i++)
            buffer[i] = padding;
        buffer[i] = '\0';
        if (flags & F_MINUS && padding == ' ')
        {
            if (extra_char)
                buffer[--ind] = extra_char;
            return (write(1, &buffer[ind], length) + write(1, &buffer[1], i - 1));
        }
        else if (!(flags & F_MINUS) && padding == ' ')
        {
            if (extra_char)
                buffer[--ind] = extra_char;
            return (write(1, &buffer[1], i - 1) + write(1, &buffer[ind], length));
        }
        else if (!(flags & F_MINUS) && padding == '0')
        {
            if (extra_char)
                buffer[--padding_start] = extra_char;
            return (write(1, &buffer[padding_start], i - padding_start) +
                write(1, &buffer[ind], length - (1 - padding_start)));
        }
    }
    if (extra_char)
        buffer[--ind] = extra_char;
    return (write(1, &buffer[ind], length));
}

/**
 * Writes an unsigned number.
 *
 * @param is_negative - Flag indicating if the number is negative.
 * @param ind - Index where the number starts in the buffer.
 * @param buffer - Buffer array to handle print.
 * @param flags - Active flags.
 * @param width - Width specifier.
 * @param precision - Precision specifier.
 * @param size - Size specifier.
 *
 * @return Number of written characters.
 */
int write_unsgnd(int is_negative, int ind,
    char buffer[],
    int flags, int width, int precision, int size)
{
    int length = BUFF_SIZE - ind - 1, i = 0;
    char padding = ' ';

    UNUSED(is_negative);
    UNUSED(size);

    if (precision == 0 && ind == BUFF_SIZE - 2 && buffer[ind] == '0')
        return 0;

    if (precision > 0 && precision < length)
        padding = ' ';

    if ((flags & F_ZERO) && !(flags & F_MINUS))
        padding = '0';

    while (precision > length)
    {
        buffer[--ind] = '0';
        length++;
    }

    if (width > length)
    {
        for (i = 0; i < width - length; i++)
            buffer[i] = padding;

        buffer[i] = '\0';

        if (flags & F_MINUS)
        {
            return (write(1, &buffer[ind], length) + write(1, &buffer[0], i));
        }
        else
        {
            return (write(1, &buffer[0], i) + write(1, &buffer[ind], length));
        }
    }

    return (write(1, &buffer[ind], length));
}

/**
 * Writes a memory address.
 *
 * @param buffer - Array of characters.
 * @param ind - Index where the number starts in the buffer.
 * @param length - Length of the number.
 * @param width - Width specifier.
 * @param flags - Flags specifier.
 * @param padding - Padding character.
 * @param extra_char - Extra character.
 * @param padd_start - Index at which padding should start.
 *
 * @return Number of written characters.
 */
int write_pointer(char buffer[], int ind, int length,
    int width, int flags, char padding, char extra_char, int padd_start)
{
    int i;

    if (width > length)
    {
        for (i = 3; i < width - length + 3; i++)
            buffer[i] = padding;
        buffer[i] = '\0';
        if (flags & F_MINUS)
        {
            buffer[--ind] = 'x';
            buffer[--ind] = '0';
            if (extra_char)
                buffer[--ind] = extra_char;
            return (write(1, &buffer[ind], length) + write(1, &buffer[3], i - 3));
        }
        else
        {
            buffer[--ind] = 'x';
            buffer[--ind] = '0';
            if (extra_char)
                buffer[--ind] = extra_char;
            return (write(1, &buffer[3], i - 3) + write(1, &buffer[ind], length));
        }
    }
    buffer[--ind] = 'x';
    buffer[--ind] = '0';
    if (extra_char)
        buffer[--ind] = extra_char;
    return (write(1, &buffer[ind], BUFF_SIZE - ind - 1));
}
