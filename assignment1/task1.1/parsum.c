#include "gmp.h"
#include "stdio.h"
#include "stdlib.h"
#include "stdint.h"
#include "pthread.h"
#include "math.h"

void *calculate_range(void *range)
{
  uint64_t start = *((uint64_t *)range);
  uint64_t stop = *((uint64_t *)range + 1);

  mpz_t *sum = malloc(sizeof(mpz_t));
  mpz_init(*sum);

  for (uint64_t i = start; i <= stop; i++)
  {
    mpz_add_ui(*sum, *sum, i);
  }

  return (void *)sum;
}

int main(int argc, char const *argv[])
{
  int num_threads = atoi(argv[1]);
  uint64_t start = atol(argv[2]);
  uint64_t stop = atol(argv[3]);

  pthread_t threads[num_threads];
  uint64_t *ranges = (uint64_t *)malloc(sizeof(uint64_t) * num_threads * 2);
  uint64_t current = start;

  for (int i = 0; i < num_threads; i++)
  {
    double step = (double)(stop - current) / (double)(num_threads - i);
    uint64_t end = current + step;
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
    pthread_join(threads[i], &result);

    mpz_add(sum, sum, result);
  }

  mpz_out_str(stdout, 10, sum);
  free(ranges);

  return 0;
}
