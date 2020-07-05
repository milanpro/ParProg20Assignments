#include <metal/stream.h>


#define WORD_BITS 512
typedef ap_uint<WORD_BITS> word_t;
// assert(STREAM_BYTES * 8 == WORD_BITS)

#define HASH_BITS 128
typedef ap_uint<HASH_BITS> hash_t;

#define UNIT_IDX_BITS 5
typedef ap_uint<UNIT_IDX_BITS> unit_index_t;
#define UNIT_BITS (0x1<<UNIT_IDX_BITS)
typedef ap_uint<UNIT_BITS> unit_t;

#define WORD_IDX_BITS 4
typedef ap_uint<WORD_IDX_BITS> word_index_t;
#define EXTRACT_UNIT(word, index) (word.range(UNIT_BITS*index+UNIT_BITS-1, UNIT_BITS*index))

#define ITERATION_BITS 7
#define ITERATION_RANGE (0x1<<(ITERATION_BITS-1))
typedef ap_uint<ITERATION_BITS> iteration_ctr_t;


void md5hash(mtl_stream &in, mtl_stream &out, hash_t search);
