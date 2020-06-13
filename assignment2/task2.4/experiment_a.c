#define ITERATIONS 100000000000ULL

void benchmark()
{
  for (;;)
  {
    unsigned long long v = 0;
    for (unsigned long long i = 0; i < ITERATIONS; ++i)
    {
      v *= v * i;
    }
  }
}

int main()
{
  // initialize();
  benchmark();
  return 0;
}
