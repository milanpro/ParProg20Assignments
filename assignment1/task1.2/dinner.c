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
sem_t* forks;

void request_fork(int i, int side) {

}

void *philosopher(void *args)
{
  while (state == RUNNING)
  {
    printf("Schmeckt das jut!");
    sleep(1);
  }
  
  return (void *)"500";
}

void initialization() {
  forks = malloc(sizeof(sem_t) * num_philosophers);

  for (int i = 0; i < num_philosophers; i++) {
    sem_init(&forks[i], 0, 0);
  }
}

int main(int argc, char const *argv[])
{
  num_philosophers = atoi(argv[1]);
  int run_time = atoi(argv[2]);

  FILE *output_file;
  output_file = fopen("./output.txt", "w+");
  int i;
  pthread_t threads[num_philosophers];

  printf("%d philosophers are eating for %d seconds\n", num_philosophers, run_time);
  for (i = 0; i < num_philosophers; i++)
  {

    pthread_create(&threads[i], NULL,
                   philosopher, i);

    printf("Philosopher %d is thinking\n", i + 1);
  }

  sleep(run_time);
  printf("Dinner is over, terminating...\n");
  state = TERMINATING;

  for (i = 0; i < num_philosophers; i++)
  {
    void *result;
    pthread_join(threads[i], &result);
    fprintf(output_file, "%s", result);

    if (i != num_philosophers - 1 ) {
      fprintf(output_file, ";");
    }
  }

  free(forks);
  fclose(output_file);
  return 0;
}
