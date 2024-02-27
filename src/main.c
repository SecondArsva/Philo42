#include "../includes/philo.h"

//  ### INDICE ###

// --- Utilidades ---
void    error(char *text);
void    correct(char *text);
long    simple_atol(char *str);

// --- Parseo ---
void    parse_input(char **argv);
void    all_digit(char **argv);
int     is_digit(char c);
void    valid_digit(char **argv);

// Dado que no se permite el uso de atoi, he de hacer el mío propio que convierta n = >INT_MAX
long    simple_atol(char *str)
{
    int i;
    long result;

    i = 0;
    result = 0;
    while (str[i])
    {
        result = (result * 10) + (str[i] - '0');
        i++;
    }
    printf("result: %li\n", result);
    return (result);
}


// 1  2   3   4  5
// p  d   e   s  m - philos die eat sleep meals
// 5 500 100 100 5
// 'p' y 'm' han de ser mayores a 0. 'd', 'e' y 's' mayores de 60 ya que representan milisegundos.
// No pueden ser mayores al INT_MAX.
void    valid_digit(char **argv)
{
    int i;
    int digit;

    i = 1;
    digit = 0;
    while (argv[i])
    {
        digit = simple_atol(argv[i]); // TODO atoul
        if (digit > INT_MAX)
            error("Los valores introducidos no pueden superar el INT_MAX");
        else if ((i == 1 || i == 5) && digit >= 1)
            i++;
        else if((i >= 2 && i <= 4) && digit >= 60)
            i++;
        else
            error("Los valores introducidos no pueden ser menores al ejemplo: ./philo 1 60 60 60 1");
    }
    correct("Bien, los argumentos introducidos son mayores al ejemplo: ./philo 1 60 60 60 1");
}

void    all_digit(char **argv)
{
    int i;
    int j;

    i = 1;
    j = 0;
    while (argv[i])
    {
        //printf("%s\n", argv[i]);
        while (argv[i][j])
        {
            if (is_digit(argv[i][j]))
            {
                //printf("-%c\n", argv[i][j]);
                j++;
            }
            else
                error("Los argumentos solo pueden componerse de números");
        }
        j = 0;
        i++;
    }
    correct("Todos los argumentos son numéricos");
}

int is_digit(char c)
{
    if (c >= '0' && c <= '9')
        return (1);
    else
        return (0);
}

void    parse_input(char **argv)
{
    all_digit(argv);
    valid_digit(argv);
}

// Printea los argumentos en caso de ser 5 o 6. NO ENTREGAR EN EL PROYECTO FINAL.
void    print_args(int argc, char **argv)
{
    int i;

    i = 0;
    printf("philo: Argumentos válidos. Total introducidos: %i.\nphilo: ", argc);
    while (argv[i])
    {
        printf("%s ", argv[i]);
        i++;
    }
    printf("\n");
}

// Función para printear a modo de prueba cuando las cosas van bien
void    correct(char *text)
{
    printf("philo: %s.\n", text);
}

// Mata el programa en caso de error.
void    error(char *text)
{
    printf("philo: ERROR: %s.\n", text);
    exit(EXIT_FAILURE);
}

// El main, amigo...
int main(int argc, char **argv)
{
    //t_table table;

    if (argc == 5 || argc == 6)
    {
        print_args(argc, argv);
        parse_input(argv);
    }
    else
        error("Total de argumentos incorrectos. Han de ser 5 o 6");
    return (0);
}