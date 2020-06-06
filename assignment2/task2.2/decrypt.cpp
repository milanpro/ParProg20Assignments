#include <iostream>
#include <fstream>
#include <vector>
#include <crypt.h>

std::vector<std::string> usernames;
std::vector<std::string> salts;
std::vector<std::string> hashes;
std::vector<std::string> dictionary;

void read_passwds(std::string filename)
{
  std::ifstream passwd_file(filename);

  std::string line;
  while (std::getline(passwd_file, line))
  {
    int sep = line.find(":");
    usernames.push_back(line.substr(0, sep));
    salts.push_back(line.substr(sep + 1, sep + 3));
    hashes.push_back(line.substr(sep + 1));
  }
}

void read_dictionary(std::string filename)
{
  std::ifstream dict_file(filename);

  std::string line;
  while (std::getline(dict_file, line))
  {
    dictionary.push_back(line);
  }
}

int main(int argc, char *argv[])
{
  std::string passwd_filename = "taskCryptPw.txt";
  std::string dict_filename = "taskCryptDict.txt";

  if (argc != 1)
  {
    passwd_filename = argv[1];
    dict_filename = argv[2];
  }

  read_passwds(passwd_filename);
  read_dictionary(dict_filename);

  std::string exact_match;
  std::string digit_match;
  int found = 0;

  for (int i = 0; i < hashes.size() && found < 1; i++)
  {
    for (int j = 0; j < dictionary.size(); j++)
    {
      struct crypt_data data;
      data.initialized = 0;

      // crypt_r is threadsafe, crypt is not!
      std::string hash = crypt_r(dictionary[j].c_str(), salts[i].c_str(), &data);

      if (hash == hashes[i])
      {
        exact_match = usernames[i] + ";" + dictionary[j];
        found++;
        break;
      }
    }
  }

  std::cout << exact_match << "\n";
  
  return 0;
}