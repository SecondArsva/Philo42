#include "../includes/philo.h"

//  ### INDICE ###

// #--- Utilidades ---#
void    error_exit(char *text);
void    correct(char *text);
long    simple_atol(char *str);
void    *safe_malloc(size_t bytes);

// #--- Parseo ---#      Listo, no toques nada del parseo.
void    parse_input(char **argv);
void    all_digit(char **argv);
int     is_digit(char c);
void    valid_digit(char **argv);

// #--- Inicialización de datos ---# Estoy con esto...
void    init_data(int argc, char **argv, t_table *table);
void    init_table(int argc, char **argv, t_table *table);
void    catch_args(int argc, char **argv, t_table *table);
//void    init_forks(t_table *table);
//void    init_philos(t_table *table);


// #--- Wrapped Handle Functions ---#
void    handle_mutex(t_mutex *mutex, t_pthread opcode);
//void    handle_threads(pthread_t *thread, t_pthread opcode);

// #--- Getter Setters with Security Mutex ---#

void    handle_mutex(t_mutex *mutex, t_pthread opcode)
{
    if (opcode == INIT)
        pthread_mutex_init(mutex, NULL);
    else if (opcode == LOCK)
        pthread_mutex_lock(mutex);
    else if (opcode == UNLOCK)
        pthread_mutex_unlock(mutex);
    else if (opcode == DESTROY)
        pthread_mutex_destroy(mutex);
    else
        error_exit("Wrong opcode on handle_mutex function");
}
/*
void    handle_threads(pthread_t *thread, void *(*routine)(void *), void *arg  t_pthread opcode)
{
    // create join
    if (opcode == CREATE)
        pthread_create(thread, );
    else if (opcode == JOIN)
        pthread_join(thread, );
}
*/

// En caso de que no se haya intruducido el número de veces
// que han de comer, el valor será "-1" para gestionar el
// comportamiento del programa a futuro.
void    catch_args(int argc, char **argv, t_table *table)
{
    table->philo_nbr = simple_atol(argv[1]);
    table->tt_die = simple_atol(argv[2]);
    table->tt_eat = simple_atol(argv[3]);
    table->tt_sleep = simple_atol(argv[4]);
    if (argc == 5)
        table->must_eat = -1;
    else if (argc == 6)
        table->must_eat = simple_atol(argv[5]);
}

void    init_table(int argc, char **argv, t_table *table)
{
    catch_args(argc, argv, table);
    table->sim_start_chrono = 0;
    table->end_sim = false;
    table->all_threads_ready = false;
    table->threads_running_nbr = 0;
    handle_mutex(&table->table_mutex, INIT);
    handle_mutex(&table->print_mutex, INIT);
    table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
    table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
}

void    init_data(int argc, char **argv, t_table *table)
{
    init_table(argc, argv, table);
    correct("post init_table");
    //init_forks(table);
    //init_philos(table);
    // vas por aquí!!!!!!!!!!!!!!!!!!
}

void    *safe_malloc(size_t size)
{
    void    *reserve;

    reserve = malloc(size);
    if (!reserve)
        error_exit("Error with the memory allocation");
    return (reserve);
}

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
    long digit;

    i = 1;
    digit = 0;
    while (argv[i])
    {
        digit = simple_atol(argv[i]);
        if (digit > INT_MAX)
            error_exit("Los valores introducidos no pueden superar el INT_MAX");
        else if ((i == 1 || i == 5) && digit >= 1)
            i++;
        else if((i >= 2 && i <= 4) && digit >= 60)
            i++;
        else
            error_exit("Los valores introducidos no pueden ser menores al ejemplo: ./philo 1 60 60 60 1");
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
                error_exit("Los argumentos solo pueden componerse de números");
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
void    error_exit(char *text)
{
    printf("philo: ERROR: %s.\n", text);
    exit(EXIT_FAILURE);
}

// El main, amigo...
int main(int argc, char **argv)
{
    t_table *table;

    if (argc == 5 || argc == 6)
    {
        print_args(argc, argv);
        parse_input(argv);
        table = safe_malloc(sizeof(t_table) * 1);
        init_data(argc, argv, table);
    }
    else
        error_exit("Total de argumentos incorrectos. Han de ser 5 o 6");
    return (0);
}