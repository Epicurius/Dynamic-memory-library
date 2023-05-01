/*
 * -*- coding: utf-8 -*-
 * vim: ts=4 sw=4 tw=80 et ai si
 *
 * Author: Niklas Neronin <niklas.neronin@gmail.com>
 */

#include <time.h>
#include <stdio.h>
#include <stdlib.h>

clock_t	g_clock_start;

/*
 * Start timing. 
 */
void	ft_timer_start(void)
{
	g_clock_start = clock();
}

/*
 * End timer and return elapsed time in seconds.
 */
double	ft_timer_end(void)
{
	double	cpu_time_used;
	clock_t	end;

	end = clock();
	cpu_time_used = ((double)(end - g_clock_start)) / CLOCKS_PER_SEC;
	return cpu_time_used;
}

# define NUMBER_OF_ALLOCS 10000
# define MAX_ALLOC_SIZE 5000
# define NUMBER_OF_RUNS 100

int main(int argc, char **argv)
{
	void *arr[NUMBER_OF_ALLOCS];
	double split[3] = {0, 0, 0};
	srand(64);

	ft_timer_start();
	for (int i = 0; i < NUMBER_OF_ALLOCS; i++) {
		size_t size = rand() % MAX_ALLOC_SIZE;
		arr[i] = malloc(size);
	}
	split[0] += ft_timer_end();

	for (int times = 0; times < NUMBER_OF_RUNS; times++) {
		ft_timer_start();
		for (int i = 0; i < NUMBER_OF_ALLOCS; i++) {
			int n = rand() % NUMBER_OF_ALLOCS;
			if (arr[n])
				continue;
			size_t size = rand() % MAX_ALLOC_SIZE;
			arr[n] = malloc(size);
		}
		split[0] += ft_timer_end();

		ft_timer_start();
		for (int i = 0; i < NUMBER_OF_ALLOCS; i++) {
			int n = rand() % NUMBER_OF_ALLOCS;
			free(arr[n]);
			arr[n] = NULL;
		}
		split[1] += ft_timer_end();

		ft_timer_start();
		for (int i = 0; i < NUMBER_OF_ALLOCS; i++) {
			int n = rand() % NUMBER_OF_ALLOCS;
			size_t size = rand() % MAX_ALLOC_SIZE;
			arr[n] = realloc(arr[n], size + 1);
		}
		split[2] += ft_timer_end();
	}

	ft_timer_start();
	for (int i = 0; i < NUMBER_OF_ALLOCS; i++)
		free(arr[i]);
	split[2] += ft_timer_end();
	printf("%f\n%f\n%f", split[0], split[1], split[2]);
	return 0;
}
