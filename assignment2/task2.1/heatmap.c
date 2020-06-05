#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "csv.h"
#include <omp.h>

int width;
int height;
int core_count;

double *src;
double *dest;

struct hotspot *hotspot_list;

void initialize(const char *hotspots_filename)
{
  src = calloc(width * height, sizeof(double));
  dest = calloc(width * height, sizeof(double));
  ranges = calloc(core_count, sizeof(struct task_range));

  hotspot_list = parse_hotspot_list(hotspots_filename);
}

double get_value(double *from, int x, int y)
{
  if ((x < 0 || x >= width) || (y < 0 || y >= height))
  {
    return 0;
  }

  return from[width * y + x];
}

void set_value(double *from, int x, int y, double value)
{
  from[width * y + x] = value;
}

void write_results()
{
  FILE *fp;
  fp = fopen("output.txt", "w");

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      double value = get_value(src, x, y);

      if (value > 0.9)
      {
        fprintf(fp, "X");
      }
      else
      {
        char output[50];
        value += 0.09;

        snprintf(output, 50, "%f", value);
        fprintf(fp, "%c", output[2]);
      }
    }

    fprintf(fp, "\n");
  }

  fprintf(fp, "\n");

  fclose(fp);
}

void write_results_coords(const char *coords_filename)
{
  struct coord *coord_list = read_coords(coords_filename);

  FILE *fp;
  fp = fopen("output.txt", "w");

  struct coord *last_coord = coord_list;
  while (last_coord != NULL)
  {
    double value = get_value(src, last_coord->x, last_coord->y);

    fprintf(fp, "%.4f\n", value);
    last_coord = last_coord->next_coord;
  }

  fclose(fp);
}

void compute_point(int x_coord, int y_coord)
{
  double acc = 0;

  for (int x = -1; x <= 1; x++)
  {
    for (int y = -1; y <= 1; y++)
    {
      acc += get_value(src, x_coord + x, y_coord + y);
    }
  }

  acc /= 9;

  set_value(dest, x_coord, y_coord, acc);
}

void set_hotspots(int round)
{
  struct hotspot *last_hotspot = hotspot_list;
  while (last_hotspot != NULL)
  {
    if (round >= last_hotspot->startround && round < last_hotspot->endround)
    {
      set_value(src, last_hotspot->x, last_hotspot->y, 1.0f);
    }

    last_hotspot = last_hotspot->next_hotspot;
  }
}

int main(int argc, char const *argv[])
{
  width = 50;
  height = 20;
  int rounds = 32;
  const char *hotspots_filename = "test_hotspots.csv";
  const char *coords_filename = NULL; //"test_coords.csv";

  core_count = get_nprocs();

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
      coords_filename = NULL;
    }
  }

  initialize(hotspots_filename);

  set_hotspots(0);

  for (int i = 0; i < rounds; i++)
  {
    printf("Round: %d/%d starting...\n", i, rounds);

    #pragma omp parallel for
    for (int x = 0; x < width; x++)
    {

      printf("Thread number: %d, x: %d\n", omp_get_thread_num(), x);

      for (int y = 0; y < height; y++)
      {
        compute_point(x, y);
      }
    }

    printf("Round: %d/%d done\n", i, rounds);

    //Swapping src into dest and the other way round
    double *temp = dest;
    dest = src;
    src = temp;

    set_hotspots(i + 1);
  }

  if (coords_filename != NULL)
  {
    write_results_coords(coords_filename);
  }
  else
  {
    write_results();
  }

  return 0;
}
