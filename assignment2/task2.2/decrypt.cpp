#include <iostream>

int main(int argc, char *argv[])
{
  std::string passwd_filename = "taskCryptPw.txt";
  std::string dict_filename = "taskCryptDict.txt";

  if (argc != 1)
  {
    passwd_filename = argv[1];
    dict_filename = argv[2];
  }

  std::cout << passwd_filename << "\n" << dict_filename << "\n";

  return 0;
}