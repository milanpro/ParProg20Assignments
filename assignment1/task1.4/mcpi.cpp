#include <iostream>
#include <thread>
 
void foo() 
{
  std::cout << "doing great.\n";
}

int main() 
{
  std::thread first (foo);
  std::thread second (foo);

  first.join();
  second.join();

  std::cout << "foo\n";

  return 0;
}