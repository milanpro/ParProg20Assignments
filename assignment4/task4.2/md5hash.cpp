#include "md5hash.h"
#include <iostream>
// NOTE: Have a look at md5hash.h, there are some useful definitions.


// NOTE: You may use the pseudo code reference from wikipedia (https://en.wikipedia.org/wiki/MD5#Pseudocode)

const word_index_t G[ITERATION_RANGE] = {
  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14, 15,   //  i         for 0..15
  1,  6, 11,  0,  5, 10, 15,  4,  9, 14,  3,  8, 13,  2,  7, 12,   // (5*i+1)%16 for 16..31
  5,  8, 11, 14,  1,  4,  7, 10, 13,  0,  3,  6,  9, 12, 15,  2,   // (3*i+5)%16 for 32..47
  0,  7, 14,  5, 12,  3, 10,  1,  8, 15,  6, 13,  4, 11,  2,  9 }; // (7*i)%16   for 48..63
const unit_index_t S[ITERATION_RANGE] = {
  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,  7, 12, 17, 22,
  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,  5,  9, 14, 20,
  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,  4, 11, 16, 23,
  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21,  6, 10, 15, 21 };
const unit_t K[ITERATION_RANGE] = {
  0xd76aa478, 0xe8c7b756, 0x242070db, 0xc1bdceee,
  0xf57c0faf, 0x4787c62a, 0xa8304613, 0xfd469501,
  0x698098d8, 0x8b44f7af, 0xffff5bb1, 0x895cd7be,
  0x6b901122, 0xfd987193, 0xa679438e, 0x49b40821,
  0xf61e2562, 0xc040b340, 0x265e5a51, 0xe9b6c7aa,
  0xd62f105d, 0x02441453, 0xd8a1e681, 0xe7d3fbc8,
  0x21e1cde6, 0xc33707d6, 0xf4d50d87, 0x455a14ed,
  0xa9e3e905, 0xfcefa3f8, 0x676f02d9, 0x8d2a4c8a,
  0xfffa3942, 0x8771f681, 0x6d9d6122, 0xfde5380c,
  0xa4beea44, 0x4bdecfa9, 0xf6bb4b60, 0xbebfbc70,
  0x289b7ec6, 0xeaa127fa, 0xd4ef3085, 0x04881d05,
  0xd9d4d039, 0xe6db99e5, 0x1fa27cf8, 0xc4ac5665,
  0xf4292244, 0x432aff97, 0xab9423a7, 0xfc93a039,
  0x655b59c3, 0x8f0ccc92, 0xffeff47d, 0x85845dd1,
  0x6fa87e4f, 0xfe2ce6e0, 0xa3014314, 0x4e0811a1,
  0xf7537e82, 0xbd3af235, 0x2ad7d2bb, 0xeb86d391 };

hash_t single_hash(word_t word) {
#pragma HLS INLINE

  // TODO: Implement MD5 rounds here and ensure they are pipelined!

  // TODO: Actually implement the algorithm
  unit_t a0 = 0x67452301;
  unit_t b0 = 0xefcdab89;
  unit_t c0 = 0x98badcfe;
  unit_t d0 = 0x10325476;

  unit_t A = a0;
  unit_t B = b0;
  unit_t C = c0;
  unit_t D = d0;

  unit_t M[16];

  for (int i = 0; i < 16; i++){
#pragma HLS UNROLL
    M[i] = EXTRACT_UNIT(word, i);
  }

  for (iteration_ctr_t i = 0; i < ITERATION_RANGE; i++){
#pragma HLS PIPELINE
    unit_t F;
    word_index_t g = G[i];

    if (i < 16){
      F = (B & C) | ((~B) & D);
    }
    else if (i < 32){
      F = (D & B) | ((~D) & C);
    }
    else if (i < 48){
      F = B ^ C ^ D;
    }
    else {
      F = C ^ (B | (~D));
    }

    F = F + A + K[i] + M[g];
    A = D;
    D = C;
    C = B;
    B = B + ((F << S[i]) | (F >> (32 - S[i])));
  }

  a0 = a0 + A;
  b0 = b0 + B;
  c0 = c0 + C;
  d0 = d0 + D;

  hash_t hash = 0;

  EXTRACT_UNIT(hash, 0) = a0;
  EXTRACT_UNIT(hash, 1) = b0;
  EXTRACT_UNIT(hash, 2) = c0;
  EXTRACT_UNIT(hash, 3) = d0;

  return hash;
}

void md5hash(mtl_stream &in, mtl_stream &out, hash_t search) {
#pragma HLS INTERFACE axis port=in name=axis_input
#pragma HLS INTERFACE axis port=out name=axis_output
#pragma HLS INTERFACE s_axilite port=search bundle=control offset=0x100
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // TODO: Check the hash of each input stream element against search.
  //       Output a single matching element or an empty element if none match.
  mtl_stream_element input;
  mtl_stream_element output;
  output.data = 0;
  output.keep = (mtl_stream_keep) -1;
  output.last = true;

  bool found = false;

  do {

    input = in.read();

    hash_t hash = single_hash((word_t) input.data);

    if (hash == search) {
      output.data = input.data;

      found = true;
    }
  } while (!input.last && !found);

  out.write(output);

  // Clear input stream
  while (!input.last) {
    input = in.read();
  }

  return;
}

