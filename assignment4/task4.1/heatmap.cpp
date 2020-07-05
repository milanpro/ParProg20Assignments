#include "heatmap.h"
// NOTE: Have a look at heatmap.h, there are some useful definitions.


// NOTE: You may define additional functions to make the code more readable

void heatmap(mtl_stream &in, mtl_stream &out) {
#pragma HLS INTERFACE axis port=in name=axis_input
#pragma HLS INTERFACE axis port=out name=axis_output
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // TODO: Place your single-round heatmap implementation here

}


