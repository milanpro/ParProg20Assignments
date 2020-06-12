#include <openssl/md5.h>
#include <inttypes.h>
#include <vector>
#include <stdio.h>
#include <stdlib.h>

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
    std::vector<unsigned char> mask{0xc0, 0xff, 0xee};

    generateBlock(mask, 0);
    generateBlock(mask, 1);

    if (argc < 3)
    {
        return -1;
    }
    int init_block = atoi(argv[1]);
    int num_blocks = atoi(argv[2]);
    int *queried_indices = new int[argc - 3];
    for (int i = 3; i < argc; i++)
    {
        queried_indices[i - 3] = atoi(argv[i]);
    }
    /* code */
    return 0;
}
