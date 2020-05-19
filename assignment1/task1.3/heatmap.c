#include "stdio.h"
#include "pthread.h"
#include "stdlib.h"
#include "string.h"
#include "unistd.h"
#include "csv.h"

int width;
int height;

double *src;
double *dest;

struct hotspot *hotspot_list;

struct coord
{
  int x;
  int y;

  struct coord *next_coord;
};

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

  hotspot_list = parse_hotspot_list(hotspots_filename);
}

double get_value(double *from, int x, int y)
{
  if ((x < 0 || x > width) || (y < 0 || y > height))
  {
    return 0;
  }

  return from[width * y + x];
}

void set_value(double *from, int x, int y, double value)
{
  from[width * y + x] = value;
}

struct coord *read_coords(const char *coords_filename)
{
  struct coord* coord_list;

  FILE *fp;
  fp = fopen(coords_filename, "r");
  char *line = NULL;
  size_t len = 0;

  if (fp == NULL)
  {
    exit(EXIT_FAILURE);
  }

  int return_code = getline(&line, &len, fp);
  if (return_code == -1) {
    exit(EXIT_FAILURE);
  }

  char *token;
  struct coord* last_coord = NULL;
  struct coord* new_coord = NULL;
  while (getline(&line, &len, fp) != -1)
  {
    if (last_coord == NULL) {
      coord_list = malloc(sizeof(struct coord));
      new_coord = coord_list;
    } else {
      new_coord = malloc(sizeof(struct coord));
      last_coord->next_coord = new_coord;
    }
    token = strtok(line, ",");
    new_coord->x = atoi(token);
    token = strtok(NULL, ",");
    new_coord->y = atoi(token);
    new_coord->next_coord = NULL;

    last_coord = new_coord;
  }

  fclose(fp);
  if (line)
    free(line);

  return coord_list;
}

void write_results()
{
  FILE * fp;
  fp = fopen ("output.txt", "w");

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      double value = get_value(dest, x, y);

      if (value > 0.9)
      {
        fprintf (fp, "X");
      }
      else
      {
        value += 0.09;
        char output[4];

        snprintf(output, 50, "%.2f", value);
        if (value > 0.1)
          printf("Value: %.2f Character: %c\n", value, output[2]);
        fprintf (fp, "%c", output[2]);
      }
    }

    fprintf(fp, "\n");
  }

  fclose (fp);
}

void write_results_coords(const char *coords_filename)
{
  struct coord* coord_list = read_coords(coords_filename);

  FILE * fp;
  fp = fopen ("output.txt", "w");

  struct coord* last_coord = coord_list;
  while (last_coord != NULL)
  {
    double value = get_value(dest, last_coord->x, last_coord->y);

    fprintf (fp, "%.4f\n", value);
    last_coord = last_coord->next_coord;
  }

  fclose (fp);
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
  const char *coords_filename = ""; //"test_coords.csv";

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

    struct hotspot *last_hotspot = hotspot_list;
    while (last_hotspot != NULL)
    {
      if (i >= last_hotspot->startround && i < last_hotspot->endround)
      {
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

    //Swapping src into dest and the other way round
    double *temp = dest;
    dest = src;
    src = temp;
  }

  if(strcmp(coords_filename, "") != 0)
    write_results_coords(coords_filename);
  else
    write_results();

  return 0;
}
