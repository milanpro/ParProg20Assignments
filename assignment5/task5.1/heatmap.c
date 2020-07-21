#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <mpi.h>
#include "csv.h"

static const int SERVER_RANK = 0;

int width;
int height;
int size;
int rank;

double *src;
double *dest;

struct hotspot *hotspot_list;

int start_x;
int start_y;
int count;

void assign_ranges()
{
  if (rank == SERVER_RANK) {
    int x = 0, y = 0;
    int current = 0;
    int total = width * height;
    
    for (int i = 0; i < size; i++)
    {
      int step = (int) ((double)(total - current) / (double)(size - i));
      
      if (i != SERVER_RANK) {
        MPI_Send(&x, 1, MPI_INT, i, 10, MPI_COMM_WORLD);
        MPI_Send(&y, 1, MPI_INT, i, 10, MPI_COMM_WORLD);
        MPI_Send(&step, 1, MPI_INT, i, 10, MPI_COMM_WORLD);
      } else {
        start_x = x;
        start_y = y;
        count = step;
      }
      
      current += step;

      y += (x + step) / width;
      x = (x + step) % width;
    }
  } else {
    MPI_Recv(&start_x, 1, MPI_INT, SERVER_RANK, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&start_y, 1, MPI_INT, SERVER_RANK, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
    MPI_Recv(&count, 1, MPI_INT, SERVER_RANK, 10, MPI_COMM_WORLD, MPI_STATUS_IGNORE);
  }
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

void worker()
{
  printf("Calculating as rank %d\n", rank);
  int col = start_x;
  int row = start_y;

  for (int i = 0; i < count; i++)
  {
    compute_point(col, row);

    col++;
    if (col == width)
    {
      col = 0;
      row++;
    }
  }
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

int main(int argc, char *argv[])
{

  MPI_Init(&argc, &argv);

  MPI_Comm_rank(MPI_COMM_WORLD, &rank);
  MPI_Comm_size(MPI_COMM_WORLD, &size);
  printf("Hello from rank = %d of %d\n", rank, size);

  width = 50;
  height = 20;
  int rounds = 32;
  const char *hotspots_filename = "test_hotspots.csv";
  const char *coords_filename = NULL; //"test_coords.csv";

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

  printf("Width: %d, height: %d, rounds: %d\n", width, height, rounds);

  src = calloc(width * height, sizeof(double));
  dest = calloc(width * height, sizeof(double));

  if (rank == SERVER_RANK) {
    hotspot_list = parse_hotspot_list(hotspots_filename);
    set_hotspots(0);
  }

  printf("Assigning ranges for rank %d...\n", rank);
  assign_ranges();
  printf("start_x: %d, start_y: %d, count: %d\n", start_x, start_y, count);

  for (int i = 0; i < rounds; i++)
  {
    MPI_Bcast(src, width * height, MPI_DOUBLE, SERVER_RANK, MPI_COMM_WORLD);
    if (rank == SERVER_RANK) {
      printf("Round: %d/%d starting...\n", i, rounds);
    }
    worker();

    if (rank == SERVER_RANK) {
      printf("Round: %d/%d done\n", i, rounds);
    }

    MPI_Gather(&(dest[width * start_y + start_x]), count, MPI_DOUBLE, dest, count, MPI_DOUBLE, SERVER_RANK, MPI_COMM_WORLD );
    //Swapping src into dest and the other way round
    if (rank == SERVER_RANK) {
      double *temp = dest;
      dest = src;
      src = temp;
      set_hotspots(i + 1);
    }
  }

  if (rank == SERVER_RANK) {
    if (coords_filename != NULL)
    {
      write_results_coords(coords_filename);
    }
    else
    {
      write_results();
    }
  }

  MPI_Finalize();
  return 0;
}
