#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/sysinfo.h>
#include "csv.h"

__device__ double get_value(double *from, int x, int y, int width, int height)
{
  if ((x < 0 || x >= width) || (y < 0 || y >= height))
  {
    return 0;
  }

  return from[width * y + x];
}

double get_value_cpu(double *from, int x, int y, int width, int height)
{
  if ((x < 0 || x >= width) || (y < 0 || y >= height))
  {
    return 0;
  }

  return from[width * y + x];
}

__device__ void set_value(double *from, int x, int y, double value, int width)
{
  from[width * y + x] = value;
}

void set_value_cpu(double *from, int x, int y, double value, int width)
{
  from[width * y + x] = value;
}

void write_results(double *src, int width, int height)
{
  FILE *fp;
  fp = fopen("output.txt", "w");

  for (int y = 0; y < height; y++)
  {
    for (int x = 0; x < width; x++)
    {
      double value = get_value_cpu(src, x, y, width, height);

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

void write_results_coords(const char *coords_filename, double *src, int width, int height)
{
  struct coord *coord_list = read_coords(coords_filename);

  FILE *fp;
  fp = fopen("output.txt", "w");

  struct coord *last_coord = coord_list;
  while (last_coord != NULL)
  {
    double value = get_value_cpu(src, last_coord->x, last_coord->y, width, height);

    fprintf(fp, "%.4f\n", value);
    last_coord = last_coord->next_coord;
  }

  fclose(fp);
}

__device__ void compute_point(double *src, double *dest, int x_coord, int y_coord, int width, int height)
{
  double acc = 0;

  for (int x = -1; x <= 1; x++)
  {
    for (int y = -1; y <= 1; y++)
    {
      acc += get_value(src, x_coord + x, y_coord + y, width, height);
    }
  }

  acc /= 9;

  set_value(dest, x_coord, y_coord, acc, width);
}

__global__ void worker(double *src, double *dest, int width, int height)
{
    int col = threadIdx.x + blockIdx.x * blockDim.x;
    int row = threadIdx.y + blockIdx.y * blockDim.y;

    compute_point(src, dest, col, row, width, height);
}

void set_hotspots(int round, struct hotspot *hotspot_list, double *src, int width)
{
  struct hotspot *last_hotspot = hotspot_list;
  while (last_hotspot != NULL)
  {
    if (round >= last_hotspot->startround && round < last_hotspot->endround)
    {
        set_value_cpu(src, last_hotspot->x, last_hotspot->y, 1.0f, width);
    }

    last_hotspot = last_hotspot->next_hotspot;
  }
}

int main(int argc, char const *argv[])
{
  int width;
  int height;

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

  dim3 blockSize = dim3(width, height);
  dim3 gridSize = dim3(1,1);

  cudaStream_t stream;
  cudaStreamCreate(&stream);

  double *src;
  double *dest;
  cudaMallocManaged(&src, width * height * sizeof(double), cudaMemAttachHost);
  cudaMallocManaged(&dest, width * height * sizeof(double), cudaMemAttachHost);
  cudaStreamAttachMemAsync(stream, src);
  cudaStreamAttachMemAsync(stream, dest);
  struct hotspot *hotspot_list = parse_hotspot_list(hotspots_filename);
  
  set_hotspots(0, hotspot_list, src, width);

  cudaStreamSynchronize(stream);
  for (int i = 0; i < rounds; i++)
  {
    printf("Round: %d/%d starting...\n", i, rounds);

    worker<<< gridSize, blockSize, 0, stream >>>(src, dest, width, height);
    cudaStreamSynchronize(stream);

    printf("Round: %d/%d done\n", i, rounds);

    //Swapping src into dest and the other way round
    double *temp = dest;
    dest = src;
    src = temp;

    set_hotspots(i + 1, hotspot_list, src, width);
  }

  cudaStreamDestroy(stream);

  if (coords_filename != NULL)
  {
    write_results_coords(coords_filename, src, width, height);
  }
  else
  {
    write_results(src, width, height);
  }

  cudaFree(src);
  cudaFree(dest);

  return 0;
}
