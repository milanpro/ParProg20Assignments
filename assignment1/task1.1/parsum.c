#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "pthread.h"
#include "math.h"

typedef __uint128_t result_type;

void *calculate_range(void *range)
{
  result_type *sum = malloc(sizeof(result_type));

  long start = *((long *)range);
  long stop = *((long *)range + 1);

  for (long i = start; i <= stop; i++)
  {
    *sum = *sum + (result_type)i;
  }

  return (void *)sum;
}

int main(int argc, char const *argv[])
{
  int num_threads = 16;
  long start = 1;
  long stop = 6074001000;

  if (argc == 4)
  {
    num_threads = atoi(argv[1]);
    start = atol(argv[2]);
    stop = atol(argv[3]);
  }

  if (stop - start < num_threads)
  {
    num_threads = stop - start + 1;
  }

  pthread_t threads[num_threads];
  long *ranges = (long *)malloc(sizeof(long) * num_threads * 2);
  long current = start;

  for (int i = 0; i < num_threads; i++)
  {
    double step = (double)(stop - current) / (double)(num_threads - i);

    long end = current + step;
    ranges[i * 2] = current;
    ranges[i * 2 + 1] = end;

    current = end + 1;

    pthread_create(&threads[i], NULL, calculate_range, &ranges[i * 2]);
  }

  result_type sum = 0;

  for (int i = 0; i < num_threads; i++)
  {
    void *result;
    if (pthread_join(threads[i], &result) != 0)
    {
      break;
    }
    sum = sum + *((result_type *)result);
    free(result);
  }

  char arr[39];
  char basechars[] = "0123456789";
  int length = 0;
  while (sum != 0)
  {
    int radix;
    radix = sum % 10;
    sum = sum / 10;
    arr[length++] = basechars[radix];
  }
  if (length == 0)
  {
    printf("0");
  }
  else
  {
    while (length--)
      printf("%c", arr[length]);
  }

  free(ranges);

  return 0;
}
