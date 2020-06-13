
void benchmark()
{
    for (;;)
    {
        asm(
            "addl $0x1, %eax\n"
            "subl $0x1, %eax");
    }
}

int main(int argc, char const *argv[])
{
    benchmark();
    return 0;
}