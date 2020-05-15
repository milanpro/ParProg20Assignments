#include "stdio.h"
#include "stdlib.h"
#include "pthread.h"
#include "math.h"

void *calculate_range(void *range)
{
  long start = *((long *)range);
  long stop = *((long *)range + 1);

  long sum = 0;

  for (long i = start; i <= stop; i++)
  {
    sum += i;
  }

  return (void *)sum;
}

int main(int argc, char const *argv[])
{
  int num_threads = atoi(argv[1]);
  long start = atol(argv[2]);
  long stop = atol(argv[3]);

  pthread_t threads[num_threads];
  long *ranges = (long *)malloc(sizeof(long) * num_threads * 2);
  long current = start;

  for (int i = 0; i < num_threads; i++)
  {
    double step = (double)(stop - current) / (double)(num_threads - i);
    if (step < 0.0f)
    {
      break;
    }
    long end = current + step;
    ranges[i * 2] = current;
    ranges[i * 2 + 1] = end;

    current = end + 1;
    int result_code = pthread_create(&threads[i], NULL, calculate_range, &ranges[i * 2]);
  }

  long sum = 0;

  for (int i = 0; i < num_threads; i++)
  {
    void *result;
    int result_code = pthread_join(threads[i], &result);
    if (result_code != 0)
    {
      break;
    }
    sum += (long)result;
  }

  printf("%ld", sum);
  free(ranges);

  return 0;
}
