#include "../includes/philo.h"

//  ### INDICE ###

// #--- Utilidades ---#

void    error_exit(char *text);
void    correct(char *text);
long    simple_atol(char *str);
void    *safe_malloc(size_t bytes);
void    print_assigned_forks(t_table *table);
void    print_forks(t_table *table, t_fork *forks);
void    print_philos(t_table *table, t_philo *philos);
void    print_table(t_table *table);
void    print_data(t_table *table);

// #--- Parseo ---#      Listo, no toques nada del parseo.

void    parse_input(char **argv);
void    all_digit(char **argv);
int     is_digit(char c);
void    valid_digit(char **argv);

// #--- Inicialización de datos ---# Listo, no toques nada de la inicialización.

void    init_data(int argc, char **argv, t_table *table);
void    init_table(int argc, char **argv, t_table *table);
void    catch_args(int argc, char **argv, t_table *table);
void    init_forks(t_table *table);
void    init_philos(t_table *table);
void    assign_forks(long philo_nbr, t_philo *philo, t_fork *forks, int i);
void    to_lone_philo(t_philo *philo, t_fork *forks);
void    to_multiple_philos(t_philo *philo, t_fork *forks, int position);

// #--- Simulación ---# Estoy con esto...

void    simulation(t_table *table);
void    *lone_philo_routine(void *arg);
void    *philos_routine(void *arg);
//void    *tons_philos_routine(void *arg);
void    *reaper_routine(void *arg); // while !ended_sim siempre
//void    check_philos(t_table *table); // no va.
int     all_philos_full(t_table *table);
int     check_philo_die(t_table *table);
void    *test_routine(void *arg);
void    create_philos(t_table *table);
//void    wait_all_philos(t_table *table);
void    wait_all_philos(char *rol, t_table *table);
void    all_philos_ready(t_table *table);
void    create_reaper(t_table *table);
void    print_death(t_table *table, long death_time, t_philo *dead_philo);
void    kill_em_all(t_table *table);
void    join_philos(t_table *table);
// void    sim_dinner(t_philo *philo);
void    sim_eat(t_philo *philo);
void    sim_sleep(t_philo *philo);
// void    sim_think(); // el tiempo de pensar son los padres.
int     am_i_alive(t_philo *philo); // Estoy vivo?
int     should_i_dead(t_philo *philo); // Debería estar muerto?
long    elapsed_time(t_table *table); // tiempo pasado actualmente desde el inicio de la simulación // current sim time

// #--- Wrapped Handle Functions ---#

void    handle_mutex(t_mutex *mutex, t_pthread opcode);
void    handle_threads(pthread_t *th, void *(*routine)(void *), void *arg, t_pthread opcode);
long    get_time(t_time_units opcode);
void    print_status(t_philo *philo, t_print opcode); // TODO

// #--- Getter Setters with Security Mutex ---#

bool    get_bool(t_mutex *mutex, bool value);
long    get_long(t_mutex *mutex, long value);
void    set_bool(t_mutex *mutex, bool *dest, bool new_value);
void    set_long(t_mutex *mutex, long *dest, long new_value);
void    increase_long(t_mutex *mutex, long *dest); // Va mal

// #--- Clear Data ---#

void    clear_data(t_table *table);

// TODO probar el return de get_time, los resultados no me parecen correctos.
// TODO progarmar un ft_usleep?
// usleep recibe microsegundos como argumento y yo estoy trabajando con microsegundos, he de realizar una conversión.
// la función elapsed time no va bien. Donde debería mostrar 1200 muestra 1000200. HECHO

// los printeos necesitan el tiempo en milisegundos, el id del philo y lo que sea que vaya a hacer.
void    print_status(t_philo *philo, t_print opcode)
{
    t_mutex print;
    t_table *table;

    print = philo->table->print_mutex;
    table = philo->table;
    // sleep(3); debug
    handle_mutex(&print, LOCK);
    if (opcode == EAT)
        printf("%li %li is eating\n", elapsed_time(table), philo->id);
    else if (opcode == SLEEP)
        printf("%li %li is sleeping\n", elapsed_time(table), philo->id);
    else if (opcode == THINK)
        printf("%li %li is thinking\n", elapsed_time(table), philo->id);
    else if (opcode == DIE)
        printf("%li %li died\n", elapsed_time(table), philo->id);
    else if (opcode == FIRST_FORK)
        printf("%li %li has taken a fork\n", elapsed_time(table), philo->id);
    else if (opcode == SECOND_FORK)
        printf("%li %li has taken a fork\n", elapsed_time(table), philo->id);
    else
        error_exit("Wrong opcode on print_status function");
    handle_mutex(&print, UNLOCK);
}

// Cuando la rutina tire me preocuparé de esto
void    clear_data(t_table *table)
{
    int compila = table->philo_nbr;
    compila++;
    printf("philo: Clear Data funtion\n");
}

/* esto está mal
long    get_time(t_time_units opcode)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
		error_exit("Gettimeofday failed");
    if (opcode == SECONDS)
        return (tv.tv_sec + (tv.tv_usec / 1e6));
    else if(opcode == MICROSECONDS)
        return ((tv.tv_sec * 1e3) + tv.tv_usec);
    else if (opcode == MILLISECONDS)
        return ((tv.tv_sec * 1e6) + (tv.tv_usec / 1e3));
    else
        error_exit("Wrong opcode on get_time function");
    return ((long)"fool_return");
}
*/

long    get_time(t_time_units opcode)
{
    struct timeval tv;

    if (gettimeofday(&tv, NULL) == -1)
		error_exit("Gettimeofday failed");
    if (opcode == SECONDS)
        return (tv.tv_sec + tv.tv_usec / 1e6);
    else if(opcode == MICROSECONDS)
        return (tv.tv_sec * 1e6 + tv.tv_usec);
    else if (opcode == MILLISECONDS)
        return ((tv.tv_sec * 1e3) + (tv.tv_usec / 1e3));
    else
        error_exit("Wrong opcode on get_time function");
    return ((long)"fool_return");
}

/*
// me falta usar la variable alive.
void    sim_dinner(t_philo *philo)
{
    t_table *table;

    table = philo->table;
    // mientras la simulación no haya acabado && y no haya terminado de comer
    while (!get_bool(&table->table_mutex, table->ended_sim) && !get_bool(&philo->philo_mutex, philo->full))
    {
        if (philo->hungry && (!pthread_mutex_lock(&philo->first_fork->fork) && !pthread_mutex_lock(&philo->second_fork->fork))) // puedo bloquear tenedores sin problemas y tengo hambre?
            sim_eat(philo);
        else
        {
            // suelto los tenedores por si acaso he cogido alguno y pienso
            pthread_mutex_unlock(&philo->first_fork->fork);
            pthread_mutex_unlock(&philo->second_fork->fork);
            sim_think(philo);
            // sigo teniendo hambre
        }
        // según termine de comer me pongo a pensar si no me he llenado, no tengo hambre pero he de seguir comiendo tras dormir
        if (!philo->hungry) // ¿He comido? Si he estado pensado no puedo ponerme ha dormir, tengo hambre
            sim_sleep(philo);
            // como he dormido tras comer puedo ponerme a comer de nuevo, tengo hambre
    }
}
*/

void    *test_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    set_bool(&philo->philo_mutex, &philo->alive, true);

    handle_mutex(&philo->table->print_mutex, LOCK);
    printf("\nHilo creado. Identificador:  %li\n", philo->id);
    printf("Hola, soy el comensal número %li y... ¿estoy vivo?\nphilo[%li]->alive = %i\n", philo->id, philo->id - 1, get_bool(&philo->philo_mutex, philo->alive));
    handle_mutex(&philo->table->print_mutex, UNLOCK);
    wait_all_philos("comensal", philo->table);
    return (NULL);
}

// spinlock - pistoletazo de salida
void    wait_all_philos(char *rol, t_table *table)
{
    handle_mutex(&table->print_mutex, LOCK); // debug
    printf("Soy %s: Espero - wait_all_philos\n", rol); // debug
    handle_mutex(&table->print_mutex, UNLOCK); // debug
    while (!table->all_threads_ready)
        ;
}

/*
void    sim_eat(t_philo *philo)
{
    if (philo->alive)
    {
        handle_mutex(&philo->table->print_mutex, LOCK);
        printf("%li - %li is eating\n", get_time(MILLISECONDS) - philo->table->sim_start_chrono, philo->id);
        handle_mutex(&philo->table->print_mutex, UNLOCK);
    }
    if (philo->alive)
        usleep(philo->table->tt_eat / 1e3);
    if (philo->alive)
        set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECONDS));
    if (philo->table->must_eat != -1)
        set_long(&philo->philo_mutex, &philo->meals_counter, philo->meals_counter + 1);
    if (philo->meals_counter == philo->table->must_eat)
        set_bool(&philo->philo_mutex, &philo->full, true);
}
*/

/*
void    sim_sleep(t_philo *philo)
{

}
*/

// current simulation time
long    elapsed_time(t_table *table)
{
    // printf("pistoletazo: %li\n", table->sim_start_chrono); // debug
    // printf("transcurrido: %li\n", get_time(MILLISECONDS) - table->sim_start_chrono); // debug
    return (get_time(MILLISECONDS) - table->sim_start_chrono);
}

int should_i_dead(t_philo *philo)
{
    t_table *table;
    
    table = philo->table;
    if (philo->last_meal_time - elapsed_time(table) >= table->tt_die)
    {
        philo->alive = false;
        // print_status(&philos[i], DIE); ¿Debería hacer el print de la muerte aquí o desde el reaper? TODO
        // realmente el que notifica su muerte es el propio philo, si muere luego ya el reaper debe matar a los otros.
        return (1);
    }
    else
        return (0);
}

int am_i_alive(t_philo *philo)
{
    if (philo->alive == false)
        return (0);
    if (should_i_dead(philo) == 1)
        return (0);
    else
        return (1);
}

/* Puedes ahorrarte esta función tras el añadido a la nueva rutina de los philos.
    no sé muy bien como terminar la rutina, por lo que tal vez cuando el philo esté full tendría que "matarlo" en alive = 0;
void    *lone_philo_routine(void *arg)
{
    t_philo *philo;

    philo = (t_philo *)arg;
    wait_all_philos("solito", philo->table);
    set_long(&philo->philo_mutex, &philo->last_meal_time, get_time(MILLISECONDS));
    while (!philo->table->ended_sim && philo->alive && !philo->full)
    {
        // cena: eat, sleep, nothink, repeat
        // printf("lone_philo_routine\n");
        sim_eat(&philo[0]);
        //sim_sleep(&philo[0]);
    }
    return (NULL);
}
*/

void    delay_by_type(t_philo *philo, t_type opcode)
{
    if (opcode == EVEN)
    {
        if (philo->id % 2 == 0)
            usleep(2000);
    }
    else if (opcode == ODD)
    {
        if (philo->id % 2 != 0)
            usleep(2000);
    }
}

// transformo datos
// espera spinlock para que todos los hilos estén creados antes de que ninguno inicie la simulación
// delay por pares, la ñapa/chapuza de hacer que esperen por pares
// bucle de simulación
void    *philos_routine(void *arg)
{
    t_philo *philo;
    t_table *table;

    philo = (t_philo *)arg;
    table = philo->table;
    set_bool(&philo->philo_mutex, &philo->alive, true);
    wait_all_philos("philo", table);
    if (table->philo_nbr == 1)
    {
        // print status pilla tenedor 1
        print_status(philo, FIRST_FORK);
        printf("sida: %li\n", table->tt_die); // debug
        usleep(table->tt_die * 1000); // debug
        // print status se muere en tt_die;
        print_status(philo, DIE);
        //printf("%li 1 died\n", table->tt_die);
		return (NULL);
    }
    delay_by_type(philo, EVEN);
    // bucle de la cena para cuando haya más de 1
    while (!table->ended_sim && philo->meals_counter != table->must_eat)
    {
        // eat
        sim_eat(philo);
        // sleep
        sim_sleep(philo);
        // se incrementa el iterador 
        if (am_i_alive(philo))
            set_long(&philo->philo_mutex, &philo->meals_counter, philo->meals_counter + 1);
            //increase_long(&philo->philo_mutex, &philo->meals_counter); // iteración
    }
    set_bool(&philo->philo_mutex, &philo->full, true);
    return (NULL);
}

/* Pilla tenedores, come, suelta tenedores.
*/
void    sim_eat(t_philo *philo)
{
    // coge tenedores
    print_status(philo, FIRST_FORK);
    print_status(philo, FIRST_FORK);
    // come y espera la comida
    print_status(philo, EAT);
    usleep(philo->table->tt_eat * 1000);
}

/* imprimir que duerme, meter un tiempo de espera que simula la siesta, imprimir pensar.
*/
void    sim_sleep(t_philo *philo)
{
    print_status(philo, SLEEP);
    usleep(philo->table->tt_sleep * 1000);
    print_status(philo, THINK);
}

// new create philos con el que me ahorro una rutina específica para el lone_philo.
void    create_philos(t_table *table)
{
    int i;
    t_philo *philo;

    i = 0;
    philo = table->philos;
    while (i < table->philo_nbr)
    {
        handle_threads(&philo[i].thread, philos_routine, &philo[i], CREATE);
        table->threads_running_nbr++;
        i++;
    }
}

/*
void    create_philos(t_table *table)
{
    int i;
    t_philo *philo;

    i = 0;
    philo = table->philos;
    if (table->philo_nbr == 1)
    {
        handle_threads(&philo[i].thread, lone_philo_routine, &philo[i], CREATE); // TODO adapta la rutina y cambia esta ft.
        table->threads_running_nbr++;
    }
    else
    {
        while (i < table->philo_nbr)
        {
            handle_threads(&philo[i].thread, philos_routine, &philo[i], CREATE);
            table->threads_running_nbr++;
            i++;
        }
    }
}
*/

void    join_philos(t_table *table)
{
    int i;
    t_philo *philo;

    i = 0;
    philo = table->philos;
    while (i < table->philo_nbr)
    {
        handle_threads(&philo[i].thread, NULL, NULL, JOIN);
        i++;
    }
}

// Esperar a que todos los hilos se creen y "notificarlo" actualizando all_threads_ready. Los comensales han llegado.
// Además se encapsula el pistoletazo de salida como inicio de la simulacion
void    all_philos_ready(t_table *table)
{
    while (table->threads_running_nbr != table->philo_nbr)
        ;
    //printf("Total de hilos corriendo: %li   [x][x][x][x] [x][x][x][x] [x][x][x][x] [x][x][x][x] \n", table->threads_running_nbr);
    table->sim_start_chrono = get_time(MILLISECONDS);
    printf("holiwi: %li\n", table->sim_start_chrono); // debug
    table->all_threads_ready = true;
}

/*
void    print_death(t_table *table, long death_time, t_philo *dead_philo)
{
    handle_mutex(&table->print_mutex, LOCK);
    printf("%li - %li died\n", death_time, dead_philo->id);
    handle_mutex(&table->print_mutex, UNLOCK);
}
*/

void    kill_em_all(t_table *table)
{
    int i;
    t_philo *philos;

    i = 0;
    philos = table->philos;
    while(i < get_long(&table->table_mutex, table->philo_nbr))  
    {
        set_bool(&philos[i].philo_mutex, &philos[i].alive, false);
        i++;
    }
}

// Mata a los philos cuando uno se excede de tiempo entre comida y comida. Da fallos.
/*
void    check_philos(t_table *table)
{
    int     i;
    long    death_time;
    t_philo *philo;

    i = 0;
    death_time = 0;
    philo = table->philos;
    while (!table->ended_sim && table->all_threads_ready)
    {
        while (!table->ended_sim && i < table->philo_nbr)
        {
            if (get_time(MILLISECONDS) - philo[i].last_meal_time >= table->tt_die)
            {
                death_time = get_time(MILLISECONDS) - table->sim_start_chrono;
                print_death(table, death_time, &philo[i]);
                table->ended_sim = true;
                kill_em_all(table);
            }
            i++;
        }
        i = 0;
    }
}
*/

/* Chequea que todos los philos hayan terminado de comer.
 Sumando +1 a un contador por cada uno que haya acabado
 compara el resultado con el total de philos, si es igual,
 han terminado todos y lo notifica mediante el return.*/ 
int all_philos_full(t_table *table)
{
    long    i;
    long    full_philos;
    t_philo *philos;

    i = 0;
    full_philos = 0;
    philos = table->philos;
    while (i < get_long(&table->table_mutex, table->philo_nbr))
    {
        if (get_bool(&philos[i].philo_mutex, philos[i].full) == 1)
            full_philos++;
        i++;
    }
    if (full_philos == get_long(&table->table_mutex, table->philo_nbr))
        return (1);
    return (0);
}

// verifica el estado vital de los philos e imprime y notifica la muerte con el return en caso de morir alguno.
int check_philo_die(t_table *table)
{
    int i;
    t_philo *philos;

    i = 0;
    philos = table->philos;
    while (i < table->philo_nbr)
    {
        if (get_bool(&philos[i].philo_mutex, philos[i].alive) == 0)
        {
            print_status(&philos[i], DIE);
            // printf("%i died\n", get_bool(&philos[i].philo_mutex, philos[i].alive)); // faltaría encapsular el tiempo de muerte para imprimirlo. TODO
            return (1);
        }
        i++;
    }
    return (0);
}

/* Nueva rutina del segador: 
 Símplemente verifica mientras, la simulación no haya acabado,
 que no haya ningún philo muerto.
 Los philos han de notificar su muerte.
 Si hay alguno muerto o si todos comieron hasta llenarse,
 da por finalizada la simulación. Solo él la finaliza. */
void    *reaper_routine(void *arg)
{
    t_table *table;

    table = (t_table *)arg;
    handle_mutex(&table->print_mutex, LOCK); // debug
    printf("Segador creado\n"); // debug
    handle_mutex(&table->print_mutex, UNLOCK); //debug 
    wait_all_philos("segador", table);
    while (!table->ended_sim && !all_philos_full(table))
    {
        //printf("Bucle segador\n"); // debug
        if (check_philo_die(table)) // Si alguno está muerto...
        {
            // ...los matas a todos y teminas la simulación
            set_bool(&table->table_mutex, &table->ended_sim, true);
            kill_em_all(table);
        }
    }
    // Si no hay muertos y terminaron de comer ha de terminarse la simulación.
    set_bool(&table->table_mutex, &table->ended_sim, true);
    printf("Segador fin\n"); // debug
    return (NULL);
}

/* me da fallos constantemente. Mata a los philos al iniciar la simulación.
void    *reaper_routine(void *arg)
{
    t_table *table;

    table = (t_table *)arg;
    all_philos_ready(table);
    usleep((table->tt_die / 1e3) / 4);
    check_philos(table);
    return (NULL);
}
*/

void    create_reaper(t_table *table)
{
    handle_threads(&table->reaper, reaper_routine, table, CREATE);
}

void    simulation(t_table *table)
{
    //printf("\n --- [SIMULATION] ---\n");
    // Crear hilos comensales
    create_philos(table);
    // Crear hilo segador
    create_reaper(table);
    // pistoletazo de salida
    all_philos_ready(table);
    // Esperar a los hilos terminen su rutina con join
    join_philos(table);
}

void    handle_threads(pthread_t *th, void *(*routine)(void *), void *arg, t_pthread opcode)
{
    if (opcode == CREATE)
        pthread_create(th, NULL, routine, arg);
    else if (opcode == JOIN)
        pthread_join(*th, NULL);
    else
        error_exit("Wrong opcode on handle_threads function");
}

bool    get_bool(t_mutex *mutex, bool value)
{
    bool    ret_bool;

    handle_mutex(mutex, LOCK);
    ret_bool = value;
    handle_mutex(mutex, UNLOCK);
    return (ret_bool);
}

long    get_long(t_mutex *mutex, long value)
{
    long    ret_long;

    handle_mutex(mutex, LOCK);
    ret_long = value;
    handle_mutex(mutex, UNLOCK);
    return (ret_long);
}

void    set_bool(t_mutex *mutex, bool *dest, bool new_value)
{
    handle_mutex(mutex, LOCK);
    *dest = new_value;
    handle_mutex(mutex, UNLOCK);
}

void    set_long(t_mutex *mutex, long *dest, long new_value)
{
    handle_mutex(mutex, LOCK);
    *dest = new_value;
    handle_mutex(mutex, UNLOCK);
}

void    increase_long(t_mutex *mutex, long *dest) // revisa como va esto, men.
{
    handle_mutex(mutex, LOCK);
    (*dest)++;
    handle_mutex(mutex, LOCK);
}

void    print_table(t_table *table)
{
    printf("\n- table\n");
    printf(" - philo_nbr: %li\n", table->philo_nbr);
    printf(" - tt_die: %li\n", table->tt_die);
    printf(" - tt_eat: %li\n", table->tt_eat);
    printf(" - tt_sleep: %li\n", table->tt_sleep);
    printf(" - must_eat: %li\n", table->must_eat);
    printf(" - sim_start_chrono: %li\n", table->sim_start_chrono);
    printf(" - ended_sim: %i\n", table->ended_sim);
    printf(" - all_threads_ready: %i\n", table->all_threads_ready);
    printf(" - threads_running_nbr: %li\n", table->threads_running_nbr);

    //printf(" - : %li\n", table->);
    printf("\n");
}

void    print_data(t_table *table)
{
        print_table(table);
        print_forks(table, table->forks);
        print_philos(table, table->philos);
        print_assigned_forks(table);
}

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
    table->ended_sim = false;
    table->all_threads_ready = false;
    table->threads_running_nbr = 0;
    handle_mutex(&table->table_mutex, INIT);
    handle_mutex(&table->print_mutex, INIT);
    table->philos = safe_malloc(sizeof(t_philo) * table->philo_nbr);
    table->forks = safe_malloc(sizeof(t_fork) * table->philo_nbr);
}

void    init_forks(t_table *table)
{
    int     i;
    t_fork *forks_array;

    i = 0;
    forks_array = table->forks;
    while (i < table->philo_nbr)
    {
        handle_mutex(&forks_array[i].fork, INIT);
        forks_array[i].fork_id = i;
        i++;
    }
}

// Cogen tenedores según su posición relativa y despues la posición relativa + 1
// En caso de ser el último filósofo, coge el último y primer tenedor.
void    to_tons_philos(long philo_nbr, t_philo *philo, t_fork *forks, int i)
{
    int position;
    int last_philo;

    position = i;
    last_philo = philo_nbr - 1;
    if (position == last_philo)
    {
        philo->first_fork = &forks[position];
        philo->second_fork = &forks[position - (philo_nbr + 1)];
    }
    else
    {
        philo->first_fork = &forks[position];
        philo->second_fork = &forks[position + 1];
    }
}

void    to_lone_philo(t_philo *philo, t_fork *forks)
{
    philo->first_fork = &forks[0];
}

void    assign_forks(long philo_nbr, t_philo *philo, t_fork *forks, int i)
{
    if (philo_nbr == 1)
        to_lone_philo(philo, forks);
    else
        to_tons_philos(philo_nbr, philo, forks, i);
}

void    init_philos(t_table *table)
{
    int     i;
    t_philo *philos_array;

    i = 0;
    philos_array = table->philos;
    while(i < table->philo_nbr)
    {
        philos_array[i].id = i + 1;
        philos_array[i].alive = true;
        philos_array[i].meals_counter = 0;
        philos_array[i].full = false;
        philos_array[i].last_meal_time = 0;
        handle_mutex(&philos_array[i].philo_mutex, INIT);
        philos_array[i].table = table;
        assign_forks(table->philo_nbr, &philos_array[i], table->forks, i);
        philos_array[i].hungry = 1;
        i++;
    }
}

void    print_assigned_forks(t_table *table)
{
    int i;

    i = 0;
    printf("- assigned_forks\n");
    if (table->philo_nbr == 1)
        printf(" - philo[%i] fork_id: %i\n", i, table->philos[i].first_fork->fork_id);
    else
    {
        while (i < table->philo_nbr)
        {
            printf(" - philo[%i] first fork_id:     %i\n", i, table->philos[i].first_fork->fork_id);
            printf(" - philo[%i] second fork_id:    %i\n", i, table->philos[i].second_fork->fork_id);
            i++;
        }
    }
    printf("\n");
}

void    print_forks(t_table *table, t_fork *forks)
{
    int i = 0;
    
    while (i < table->philo_nbr)
    {
        printf("- fork[%i]\n - fork_id: %i\n", i, forks[i].fork_id);
        printf("\n");
        i++;
    }
}

void    print_philos(t_table *table, t_philo *philos)
{
    int i = 0;
    while (i < table->philo_nbr)
    {
        printf("- philos[%i]\n", i);
        printf(" - id:              %li\n", philos[i].id);
        printf(" - alive:           %i\n", philos[i].alive);
        printf(" - full:            %i\n", philos[i].full);
        printf(" - meals_counter:   %li\n", philos[i].meals_counter);
        printf(" - last_meal_time:  %li\n", philos[i].last_meal_time);
        printf(" - table pointer:   %p\n", philos[i].table);
        printf(" - first_fork:      %p\n", philos[i].first_fork);
        printf(" - second_fork:     %p\n", philos[i].second_fork);
        printf(" - hungry:          %i\n", philos[i].hungry);
        printf("\n");
        i++;
    }
}

void    init_data(int argc, char **argv, t_table *table)
{
    init_table(argc, argv, table);
    init_forks(table);
    init_philos(table);
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
    //printf("result: %li\n", result);
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
    //correct("Bien, los argumentos introducidos son mayores al ejemplo: ./philo 1 60 60 60 1");
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
    //correct("Todos los argumentos son numéricos");
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
        //print_args(argc, argv);
        parse_input(argv);
        table = safe_malloc(sizeof(t_table) * 1);
        init_data(argc, argv, table);
        //print_data(table);
        simulation(table); // Crear simulación. Diseñar como iría. Crear hilos comensales y segador.
        clear_data(table);
    }
    else
        error_exit("Total de argumentos incorrectos. Han de ser 5 o 6");
    return (0);
}