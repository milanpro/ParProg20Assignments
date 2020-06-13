#include <openssl/md5.h>
#include <inttypes.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <omp.h>

#define byte unsigned char

bool compare(std::vector<byte> *in1, std::vector<byte> *in2)
{
    for (long unsigned int i = 0; i < in1->size(); i++)
    {
        if (in1->at(i) != in2->at(i))
        {
            return in1->at(i) < in2->at(i);
        }
    }

    return true;
}

std::vector<byte> *generateBlock(std::vector<byte> &mask, int index)
{
    std::vector<byte> *block = new std::vector<byte>(64, 0);
    std::copy(mask.begin(), mask.end(), block->begin());

    // What the fuck
    for (int y = 0; y < index; y++)
        for (int i = 63; i >= 0; i--)
            if (++block->data()[i])
                break;

    return block;
}

std::vector<byte> *compute_hash(std::vector<byte> &block)
{
    std::vector<byte> *md = new std::vector<byte>(16);
    MD5(block.data(), block.size(), md->data());
    return md;
}

int main(int argc, char const *argv[])
{
    if (argc < 3)
    {
        return -1;
    }

    std::stringstream hexstream;
    std::string hex(argv[1]);
    std::vector<byte> mask;

    unsigned int buffer;
    unsigned int offset = 0;

    while (offset < hex.length())
    {
        hexstream.clear();
        hexstream << std::hex << hex.substr(offset, 2);
        hexstream >> buffer;
        mask.push_back(static_cast<byte>(buffer));
        offset += 2;
    }

    int num_blocks = std::atoi(argv[2]);
    int *queried_indices = new int[argc - 3];

    for (int i = 3; i < argc; i++)
    {
        queried_indices[i - 3] = std::atoi(argv[i]);
    }

    std::vector<std::vector<byte> *> *hashes = new std::vector<std::vector<byte> *>(num_blocks);

#pragma omp parallel for
    for (int i = 0; i < num_blocks; i++)
    {
        std::vector<byte> *veccy = generateBlock(mask, i);
        std::vector<byte> *hash = compute_hash(*veccy);

        hashes->at(i) = hash;
    }

    std::sort(hashes->begin(), hashes->end(), compare);

    for (int i = 0; i < argc - 3; i++)
    {
        for (auto help : *hashes->at(queried_indices[i]))
        {
            printf("%02hhx", help);
        }
        printf("\n");
    }

    return 0;
}
