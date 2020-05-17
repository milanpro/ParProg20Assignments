#include "gmp.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "pthread.h"
#include "math.h"

void *calculate_range(void *range)
{
  mpz_t *sum = malloc(sizeof(mpz_t));
  mpz_init(*sum);

  long start = *((long *)range);
  long stop = *((long *)range + 1);

  for (long i = start; i <= stop; i++)
  {
    mpz_add_ui(*sum, *sum, i);
  }

  return (void *)sum;
}

int main(int argc, char const *argv[])
{
  int num_threads = 32;
  long start = 23;
  long stop = 42;

  if (argc === 4) {
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

  mpz_t sum;
  mpz_init(sum);

  for (int i = 0; i < num_threads; i++)
  {
    void *result;
    if (pthread_join(threads[i], &result) != 0)
    {
      break;
    }
    mpz_add(sum, sum, result);
    free(result);
  }

  mpz_out_str(stdout, 10, sum);
  free(ranges);

  return 0;
}
