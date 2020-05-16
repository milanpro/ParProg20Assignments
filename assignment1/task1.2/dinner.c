#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"

void *philosopher(void *args)
{
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

  for (i = 0; i < num_philosopers; i++)
  {

    pthread_create(&threads[i], NULL,
                   philosopher, NULL);

    printf("Philosopher %d is thinking\n", i + 1);
  }

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
