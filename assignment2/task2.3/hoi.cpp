#include <openssl/md5.h>
#include <inttypes.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <sstream>

void generateBlock(std::vector<unsigned char> &mask, int index)
{
    unsigned char *block = new unsigned char[64];
    std::copy(mask.begin(), mask.end(), block);

    uint64_t voodoMagic = *reinterpret_cast<uint64_t *>(&block[0]);

    uint64_t moreMagic = be64toh(voodoMagic);
    moreMagic += index;
    uint64_t output = htobe64(moreMagic);

    unsigned char *outputBlock = reinterpret_cast<unsigned char *>(&output);

    printf("%lx\n", output);
}

unsigned char *compute_hash(std::vector<unsigned char> &block)
{
    unsigned char *md = new unsigned char[64];
    MD5(block.data(), 64, md);
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
    std::vector<unsigned char> mask;
    unsigned int buffer;
    unsigned int offset = 0;
    while (offset < hex.length()) {
        hexstream.clear();
        hexstream << std::hex << hex.substr(offset, 2);
        hexstream >> buffer;
        mask.push_back(static_cast<unsigned char>(buffer));
        offset += 2;
    }
    int num_blocks = std::atoi(argv[2]);
    int *queried_indices = new int[argc - 3];
    for (int i = 3; i < argc; i++)
    {
        queried_indices[i - 3] = std::atoi(argv[i]);
    }

    for (int i = 0; i < num_blocks; i++) {
        generateBlock(mask, i);
    }
    return 0;
}
