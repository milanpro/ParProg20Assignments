#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"

int width;
int height;

double *src;
double *dest;

struct hotspot
{
  int x;
  int y;
  // inclusive
  int startround;
  // exclusive
  int endround;
  struct hotspot* next_hotspot;
};

struct hotspot* hotspot_list;

struct dest_target
{
  int x;
  int y;
};

struct dest_target *targets;

void initialize(const char *hotspots_filename)
{
  src = malloc(sizeof(double) * width * height);
  dest = malloc(sizeof(double) * width * height);
  targets = malloc(sizeof(struct dest_target) * width * height);

  FILE *fp;
  char *line = NULL;
  size_t len = 0;
  ssize_t read;

  fp = fopen(hotspots_filename, "r");

  if (fp == NULL)
  {
    exit(EXIT_FAILURE);
  }

  int return_code = getline(&line, &len, fp);
  if (return_code == -1) {
    exit(EXIT_FAILURE);
  }

  char *token;
  struct hotspot* last_hotspot = NULL;
  struct hotspot* new_hotspot = NULL;
  while (getline(&line, &len, fp) != -1)
  {
    if (last_hotspot == NULL) {
      hotspot_list = malloc(sizeof(struct hotspot));
      new_hotspot = hotspot_list;
    } else {
      new_hotspot = malloc(sizeof(struct hotspot));
      last_hotspot->next_hotspot = new_hotspot;
    }
    token = strtok(line, ",");
    new_hotspot->x = atoi(token);
    token = strtok(NULL, ",");
    new_hotspot->y = atoi(token);
    token = strtok(NULL, ",");
    new_hotspot->startround = atoi(token);
    token = strtok(NULL, ",");
    new_hotspot->endround = atoi(token);

    last_hotspot = new_hotspot;
  }

  fclose(fp);
  if (line)
    free(line);
}

double get_value(double *from, int x, int y)
{
  return from[width * y + x];
}

void set_value(double *from, int x, int y, double value)
{
  from[width * y + x] = value;
}

void *worker(void *args)
{
  struct dest_target *target = (struct dest_target *)args;
  double acc = 0;

  for (int x = target->x - 1; x < target->x + 1; x++)
  {
    for (int y = target->y - 1; y < target->y + 1; y++)
    {
      acc += get_value(src, x, y);
    }
  }

  acc /= 9;

  set_value(dest, target->x, target->y, acc);

  return NULL;
}

int main(int argc, char const *argv[])
{
  width = 50;
  height = 20;
  int rounds = 32;
  const char *hotspots_filename = "test_hotspots.csv";
  const char *coords_filename = "test_coords.csv";

  if (argc != 1)
  {
    width = atoi(argv[1]);
    height = atoi(argv[2]);
    rounds = atoi(argv[3]);
    hotspots_filename = argv[4];
    if (argc == 6)
    {
      coords_filename = argv[5];
    }
    else
    {
      coords_filename = "";
    }
  }

  initialize(hotspots_filename);

  pthread_t threads[width * height];

  for (int i = 0; i < rounds; i++)
  {
    printf("Round: %d/%d starting...\n", i, rounds);

    struct hotspot* last_hotspot = hotspot_list;
    while (last_hotspot != NULL) {
      if (i >= last_hotspot->startround && i < last_hotspot->endround) {
        set_value(src, last_hotspot->x, last_hotspot->y, 1.0f);
      }

      last_hotspot = last_hotspot->next_hotspot;
    }

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        targets[width * y + x].x = x;
        targets[width * y + x].y = y;

        pthread_create(&threads[width * y + x], NULL,
                       worker, &targets[width * y + x]);
      }
    }

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        pthread_join(threads[width * y + x], NULL);
      }
    }
    
    printf("Round: %d/%d done\n", i, rounds);

    for (int x = 0; x < width; x++)
    {
      for (int y = 0; y < height; y++)
      {
        printf("%.1f,", get_value(dest, x, y));
      }
      printf("\n");
    }

    //Swapping src into dest and the other way round
    double *temp = dest;
    dest = src;
    src = temp;
  }

  return 0;
}
