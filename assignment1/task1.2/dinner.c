#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "semaphore.h"
#include "unistd.h"

#define RUNNING 1
#define TERMINATING 0

#define LEFT 1
#define RIGHT 2

_Atomic int state = RUNNING;
_Atomic int num_philosophers;
sem_t *forks;
sem_t *waiter;

int get_fork_id(int id, int side)
{
  if (side == LEFT)
  {
    return (id + (num_philosophers - 1)) % num_philosophers;
  }
  else
  {
    return (id + 1) % num_philosophers;
  }
}

void request_fork(int id, int side)
{
  int fork_id = get_fork_id(id, side);
  sem_wait(&forks[fork_id]);
}

void put_fork_back(int id, int side)
{
  int fork_id = get_fork_id(id, side);
  sem_post(&forks[fork_id]);
}

void *philosopher(void *args)
{
  int id = *((int *)args);
  int eat_counter = 0;
  while (state == RUNNING)
  {
    //Hungry
    //ruins parallelization but works
    sem_wait(waiter);
    request_fork(id, LEFT);
    request_fork(id, RIGHT);
    sem_post(waiter);
    //Eating
    eat_counter = eat_counter + 1;
    //Cleanup
    put_fork_back(id, LEFT);
    put_fork_back(id, RIGHT);
  }
  char *return_val = malloc(sizeof(char) * 12);
  sprintf(return_val, "%d", eat_counter);
  return (void *)return_val;
}

void initialization()
{
  forks = malloc(sizeof(sem_t) * num_philosophers);
  waiter = malloc(sizeof(sem_t));

  for (int i = 0; i < num_philosophers; i++)
  {
    sem_init(&forks[i], 0, 1);
  }

  sem_init(waiter, 0, 1);
}

int main(int argc, char const *argv[])
{
  num_philosophers = 5;
  int run_time = 4;
  if (argc == 3)
  {
    num_philosophers = atoi(argv[1]);
    run_time = atoi(argv[2]);
  }

  FILE *output_file;
  output_file = fopen("./output.txt", "w+");
  int i;
  pthread_t threads[num_philosophers];
  int *phil_ids = malloc(sizeof(int) * num_philosophers);
  initialization();
  printf("%d philosophers are eating for %d seconds\n", num_philosophers, run_time);
  for (i = 0; i < num_philosophers; i++)
  {
    phil_ids[i] = i;
    pthread_create(&threads[i], NULL,
                   philosopher, &phil_ids[i]);

    printf("Philosopher %d is thinking\n", i + 1);
  }

  sleep(run_time);
  printf("Dinner is over, terminating...\n");
  state = TERMINATING;

  for (i = 0; i < num_philosophers; i++)
  {
    void *result;
    pthread_join(threads[i], &result);
    fprintf(output_file, "%s", (char *)result);
    free(result);
    if (i != num_philosophers - 1)
    {
      fprintf(output_file, ";");
    }
  }

  free(forks);
  free(phil_ids);
  fclose(output_file);
  return 0;
}
