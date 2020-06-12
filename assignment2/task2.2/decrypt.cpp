#include <crypt.h>
#include <iostream>
#include <fstream>
#include <vector>
#include <omp.h>


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

  std::vector<std::string> matches;

  std::vector<std::string> attachments = {"", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9"};

  #pragma omp parallel for
  for (size_t i = 0; i < hashes.size(); i++)
  {
    // Can't use break inside an OMP for loop
    if (matches.size() >= 2)
    {
      continue;
    }

    for (size_t j = 0; j < dictionary.size(); j++)
    {
      for (size_t att = 0; att < attachments.size(); att++)
      {
        struct crypt_data data;
        data.initialized = 0;

        // crypt_r is threadsafe, crypt is not!
        std::string hash = crypt_r((dictionary[j] + attachments[att]).c_str(), salts[i].c_str(), &data);

        if (hash == hashes[i])
        {
          matches.push_back(usernames[i] + ";" + dictionary[j] + attachments[att]);
          break;
        }
      }
    }
  }

  for (size_t i = 0; i < matches.size(); i++)
  {
    std::cout << matches[i] << std::endl;
  }
  
  return 0;
}