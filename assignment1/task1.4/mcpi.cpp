#include <iostream>
#include <thread>
#include <random>
#include <future>

enum Strategy
{
  LOCAL = 1,
  SYNC = 2,
  ATOMIC = 3
};

Strategy execution_strategy;

std::atomic<int> global_circle_points;

bool in_circle(double x, double y)
{
  return (x * x) + (y * y) <= 1.0f;
}

void monte_carlo_worker(uint64_t npoints, std::promise<uint64_t> &&result)
{
  std::hash<std::thread::id> hasher;
  std::mt19937 generator(hasher(std::this_thread::get_id()));
  std::uniform_real_distribution<double> dist(0.0, 1.0);

  uint64_t circle_points = 0;

  for (int i = 0; i < npoints; i++)
  {
    if (in_circle(dist(generator), dist(generator)))
    {
      circle_points++;
    }
  }

  result.set_value(circle_points);
}

double mcpi_local(uint64_t point_count, int thread_count)
{
  std::thread threads[thread_count];
  std::promise<uint64_t> in_circle_promises[thread_count];
  std::future<uint64_t> in_circle_futures[thread_count];

  for (int i = 0; i < thread_count; i++)
  {
    in_circle_futures[i] = in_circle_promises[i].get_future();
  }

  uint64_t thread_points = point_count / thread_count;
  int remainder = point_count % thread_count;
  for (int i = 0; i < thread_count; i++)
  {
    if (i == thread_count - 1)
    {
      thread_points += remainder;
    }
    threads[i] = std::thread(monte_carlo_worker, thread_points, std::move(in_circle_promises[i]));
  }

  uint64_t in_circle_acc = 0;
  for (int i = 0; i < thread_count; i++)
  {
    threads[i].join();
    if (execution_strategy == Strategy::LOCAL)
    {
      in_circle_acc += in_circle_futures[i].get();
    }
  }

  double pi = 4 * (double)in_circle_acc / (double)point_count;

  return pi;
}

double mcpi_sync(uint64_t point_count, int thread_count)
{
  return 0.0;
}

double mcpi_atomic(uint64_t point_count, int thread_count)
{
  return 0.0;
}

int main(int argc, char *argv[])
{
  uint64_t point_count = atoll(argv[1]);
  int thread_count = atoi(argv[2]);
  execution_strategy = static_cast<Strategy>(atoi(argv[3]));

  double pi;

  switch(execution_strategy) {
    case LOCAL:
      pi = mcpi_local(point_count, thread_count);
      break;
    case SYNC:
      pi = mcpi_sync(point_count, thread_count);
      break;
    default:
      pi = mcpi_atomic(point_count, thread_count);
      break;
  }

  std::cout << "PI: " << pi << "\n";

  return 0;
}