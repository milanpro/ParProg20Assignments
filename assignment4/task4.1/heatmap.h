#include <metal/stream.h>


#define CELL_BITS 8
typedef ap_uint<CELL_BITS> cell_t;

#define ROW_CELLS 64
#define ROW_BITS (ROW_CELLS*CELL_BITS)
typedef ap_uint<ROW_BITS> row_t;
// assert(ROW_CELLS == STREAM_BYTES);

#define SLICE(word, offset) (word(CELL_BITS*((offset)+1)-1, CELL_BITS*(offset)))


void heatmap(mtl_stream &in, mtl_stream &out);
