#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "semaphore.h"
#include "unistd.h"

#define RUNNING 1
#define TERMINATING 0

int state = RUNNING;

void *philosopher(void *args)
{
  while (state == RUNNING)
  {
    printf("Schmeckt das jut!");
    sleep(1);
  }
  
  return (void *)"500";
}

int main(int argc, char const *argv[])
{
  int num_philosopers = atoi(argv[1]);
  int run_time = atoi(argv[2]);

  FILE *output_file;
  output_file = fopen("./output.txt", "w+");
  int i;
  pthread_t threads[num_philosopers];

  printf("%d philosophers are eating for %d seconds\n", num_philosopers, run_time);
  for (i = 0; i < num_philosopers; i++)
  {

    pthread_create(&threads[i], NULL,
                   philosopher, NULL);

    printf("Philosopher %d is thinking\n", i + 1);
  }

  sleep(run_time);
  printf("Dinner is over, terminating...\n");
  state = TERMINATING;

  for (i = 0; i < num_philosopers; i++)
  {
    void *result;
    pthread_join(threads[i], &result);
    fprintf(output_file, "%s", result);

    if (i != num_philosopers - 1 ) {
      fprintf(output_file, ";");
    }
  }
  fclose(output_file);
  return 0;
}
