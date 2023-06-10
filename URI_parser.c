#include <string.h>
#include <stdlib.h>>

#include "URI_parser.h"

char *Uri_parser(char *uri)
{
    char *ptr = uri;
    char *result = calloc(1, strlen(uri));
    char current_hex[3];
    // busca la primera aparición de ptr
    while ((ptr = strchr(ptr, '%')) != NULL)
    {
        // Donde este el ultimo valor de % lo elimino
        *(ptr++) = '\0';
        result = strcat(result, uri);
        // Se salta los números hexadecimales
        uri = ptr + 2;
        // Copio los numeros hexadecimales
        strncpy(current_hex, ptr, 2);

        char c1 = HexaToDec(current_hex);
        // Copio el numero a decimal
        result = strncat(result, &c1, 1);
    }
    strcat(result, uri);
    return result;
}

char HexaToDec(char *Hex)
{
    int dec = 0;
    int base = 1;
    int len = strlen(Hex);
    for (int i = len - 1; i >= 0; i--)
    {
        switch (Hex[i])
        {
        case 'A':
        case 'a':
            dec += 10 * base;
            break;
        case 'B':
        case 'b':
            dec += 11 * base;
            break;
        case 'C':
        case 'c':
            dec += 12 * base;
            break;
        case 'D':
        case 'd':
            dec += 13 * base;
            break;
        case 'E':
        case 'e':
            dec += 14 * base;
            break;
        case 'F':
        case 'f':
            dec += 15 * base;
            break;
        default:
            // ASCII to int
            dec += ((int)Hex[i] - 48) * base;
            break;
        }
        // Multiplico por 16
        base *= 16;
    }
    return (char)dec;
}
