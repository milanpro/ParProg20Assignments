
void benchmark()
{
  double add1 = 0;
  double add2 = 0;
  double add3 = 0;
  double add4 = 0;

  double mul1 = 3.210;
  double mul2 = 5.432;
  double mul3 = 7.654;
  double mul4 = 9.876;

  while (1)
  {
    add1 += 3.210;
    add2 += 5.432;
    add3 += 7.654;
    add4 += 9.876;

    mul1 *= 9.876;
    mul2 *= 7.654;
    mul3 *= 5.432;
    mul4 *= 3.210;
  }
}

int main()
{
  // initialize();
  benchmark();
  return 0;
}
