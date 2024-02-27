/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   philo.h                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: davidga2 <davidga2@student.42madrid.com>   +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2023/10/21 16:39:50 by davidga2          #+#    #+#             */
/*   Updated: 2024/01/02 23:39:37 by davidga2         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#ifndef PHILO_H
# define PHILO_H

# include <stdio.h>		// printf
# include <stdlib.h>	// malloc free
# include <unistd.h>	// usleep
# include <sys/time.h>	// gettimeofday
# include <pthread.h>	// mutex: init destroy lock unlock
						// threads: create join detach
# include <limits.h>	// INT_MAX
# include <errno.h>		// pthreads errors
#include <stdbool.h>	// for use the bool variable type

// forward declarations
//typedef s_fork	t_fork;
//typedef s_table t_table;

//*** structures ***

typedef pthread_mutex_t t_mutex; // code more redeable
typedef struct s_philo t_philo;

typedef struct	s_fork{
	t_mutex	fork;
	int		fork_id;
}				t_fork;

// "tt_" means "time_to_" son "tt_die" its "time_to_die", etc.
typedef struct	s_table{	// datos globales a falta de poder tener variables globales reales
	long		philo_nbr;
	long		tt_die;
	long		tt_eat;
	long		tt_sleep;
	long		must_eat;		// [5] | Flag if -1

	long		sim_start_chrono;	// importante para el timestamp. TODO start_sim_time.
	bool		end_sim; // a philo dies or all philos full;
	bool		all_threads_ready; // synchro philo to start at the same time
	long		threads_running_nbr;
	
	t_mutex		table_mutex;	// avoid races while reading from table
	t_mutex		print_mutex;	// For avoid the overlap when printf with threads without use fflush.
	
	t_philo		*philos; // array de filósofos;
	t_fork		*forks; // array de tenedores;
	
	pthread_t	grimdeath;
}				t_table;

typedef struct	s_philo{
	int			id;				// identificador
	long		meals_counter;	// veces que ha comido
	int			full_bool;		// flag que determina que ha terminado de comer
	long		last_meal_time;	// tiempo transcurrido desde la última vez que comió
	t_fork		*first_fork;
	t_fork		*second_fork;
	pthread_t	thread_id;		// a philo is a thread
	t_mutex		philo_mutex;	// usefull for races with grim monitor
	t_table		*table;
}				t_philo;

// Enum OPCODE for the wrapper handle functions for more
// legibility. The enum its like a block of macros of a theme.

//			### PTHREAD OPCODE ###
typedef enum	e_pthread
{
	INIT,
	LOCK,
	UNLOCK,
	DESTROY,
	CREATE,
	JOIN,
}				t_pthread;

//			### TIME UNITS OPECODE ###
typedef enum	e_time_units
{
	SECONDS,
	MILLISECONDS,
	MICROSECONDS,
}				t_time_units;


//			### PRINT OPECODE ###
typedef	enum	e_print
{
	EAT,
	SLEEP,
	THINK,
	DIE,
	FIRST_FORK_TAKED,
	SECOND_FORK_TAKED,
}				t_print;

// #--- STRUCT PRINTER ---#
typedef enum	e_struct
{
	TABLE,
	PHILO,
	FORK,
	ALL,
}

//			### FUNCTIONS BY FILES ###
#endif