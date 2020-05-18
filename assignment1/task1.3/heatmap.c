#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "semaphore.h"
#include "unistd.h"

_Atomic int width;
_Atomic int height;

int main(int argc, char const *argv[])
{
  width = 50;
  height = 20;
  int rounds = 32;
  char* hotspots_filename = "test_hotspots.csv";
  char* coords_filename = "test_coords.csv";

  if (argc != 1) {
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    rounds = atoi(argv[3]);
    hotspots_filename = argv[4];
    if (argc == 6) {
      coords_filename = argv[5];
    } else {
      coords_filename = "";
    }
  }

  /* code */
  return 0;
}
