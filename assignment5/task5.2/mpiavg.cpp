#include <iostream>
#include <fstream>
#include <vector>
#include <cstdlib>
#include <sstream>
#include <string>
#include <cmath>

#include <mpi.h>

#define IS_INT_ROOT mpi_rank == 0
#define IS_DOUBLE_ROOT mpi_rank == int_count

int main(int argc, char *argv[])
{
    MPI_Init(&argc, &argv);

    int mpi_size, mpi_rank;
    MPI_Comm_size(MPI_COMM_WORLD, &mpi_size);
    MPI_Comm_rank(MPI_COMM_WORLD, &mpi_rank);

    int int_count = atoi(argv[2]);
    int double_count = atoi(argv[3]);

    std::vector<int> int_ranks(int_count, 0);
    for (int i = 0; i < int_count; i++)
    {
        int_ranks[i] = i;
    }

    std::vector<int> double_ranks(double_count, 0);
    for (int i = 0; i < double_count; i++)
    {
        double_ranks[i] = i + int_count;
    }

    std::vector<double> numbers;
    int number_count = 0, number_range = 0;

    if (IS_INT_ROOT)
    {
        std::ifstream numbers_stream(argv[1]);
        std::string line;

        while (getline(numbers_stream, line))
        {
            std::istringstream line_stream(line);
            double number;
            line_stream >> number;
            numbers.push_back(floor(number));
        }

        numbers_stream.close();

        number_count = numbers.size();
        number_range = 1;

        if (number_count < int_count)
        {
            for (int i = 0; i < int_count - number_count; i++)
            {
                numbers.push_back(0);
            }
        }
        else if (number_count > int_count)
        {
            number_range = ceil((double)number_count / int_count);
            for (int i = 0; i < int_count * number_range - number_count; i++)
            {
                numbers.push_back(0);
            }
        }
    }
    else if (IS_DOUBLE_ROOT)
    {
        std::ifstream numbers_stream(argv[1]);
        std::string line;

        while (getline(numbers_stream, line))
        {
            std::istringstream line_stream(line);
            double number;
            line_stream >> number;
            numbers.push_back(number);
        }

        numbers_stream.close();

        number_count = numbers.size();
        number_range = 1;

        if (number_count < double_count)
        {
            for (int i = 0; i < double_count - number_count; i++)
            {
                numbers.push_back(0);
            }
        }
        else if (number_count > double_count)
        {
            number_range = ceil((double)number_count / double_count);
            for (int i = 0; i < double_count * number_range - number_count; i++)
            {
                numbers.push_back(0);
            }
        }
    }

    MPI_Group world_group;
    MPI_Comm_group(MPI_COMM_WORLD, &world_group);

    MPI_Group sub_group;
    if (mpi_rank < int_count)
    {
        MPI_Group_incl(world_group, int_ranks.size(), int_ranks.data(), &sub_group);
    }
    else
    {
        MPI_Group_incl(world_group, double_ranks.size(), double_ranks.data(), &sub_group);
    }

    MPI_Comm sub_communicator;
    MPI_Comm_create(MPI_COMM_WORLD, sub_group, &sub_communicator);

    MPI_Bcast(&number_range, 1, MPI_INT, 0, sub_communicator);

    std::vector<double> values(number_range, 0);

    MPI_Scatter(numbers.data(), number_range, MPI_DOUBLE, values.data(), number_range, MPI_DOUBLE, 0, sub_communicator);

    double process_sum = 0;
    for (int i = 0; i < number_range; i++)
    {
        process_sum += values[i];
    }

    double global_sum = 0;

    MPI_Reduce(&process_sum, &global_sum, 1, MPI_DOUBLE, MPI_SUM, 0, sub_communicator);

    if (IS_INT_ROOT)
    {
        double average = global_sum / number_count;
        std::cout << average << std::endl;
    }

    MPI_Barrier(MPI_COMM_WORLD);

    if (IS_DOUBLE_ROOT)
    {
        double average = global_sum / number_count;
        std::cout << average << std::endl;
    }

    MPI_Finalize();
}