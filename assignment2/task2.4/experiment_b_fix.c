
void benchmark()
{
  int add1 = 0;
  int add2 = 0;
  int add3 = 0;
  int add4 = 0;

  int mul1 = 3;
  int mul2 = 5;
  int mul3 = 7;
  int mul4 = 9;

  while (1)
  {
    add1 += 3;
    add2 += 5;
    add3 += 7;
    add4 += 9;

    mul1 *= 9;
    mul2 *= 7;
    mul3 *= 5;
    mul4 *= 3;
  }
}

int main()
{
  // initialize();
  benchmark();
  return 0;
}
