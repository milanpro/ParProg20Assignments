#include "heatmap.h"
// NOTE: Have a look at heatmap.h, there are some useful definitions.

mtl_stream_data compute_row(mtl_stream_data first, mtl_stream_data second, mtl_stream_data third) {
  mtl_stream_data result = 0;

  for (int j=0; j < ROW_CELLS; j++) {
    mtl_stream_data left_top = 0;
    mtl_stream_data left_center = 0;
    mtl_stream_data left_bottom = 0;

    if (j > 0){
      left_top = SLICE(first, j - 1);
      left_center = SLICE(second, j - 1);
      left_bottom = SLICE(third, j - 1);
    }

    mtl_stream_data middle_top = SLICE(first, j);
    mtl_stream_data middle_center = SLICE(second, j);
    mtl_stream_data middle_bottom = SLICE(third, j);

    mtl_stream_data right_top = 0;
    mtl_stream_data right_center = 0;
    mtl_stream_data right_bottom = 0;

    if (j < ROW_CELLS - 1){
      right_top = SLICE(first, j + 1);
      right_center = SLICE(second, j + 1);
      right_bottom = SLICE(third, j + 1);
    }

    if (middle_center == 0xff) {
      result(CELL_BITS*((j)+1)-1, CELL_BITS*(j)) = 255;
    } else {
      result(CELL_BITS*((j)+1)-1, CELL_BITS*(j)) = (left_top + left_center + left_bottom + middle_top + middle_center + middle_bottom + right_top + right_center + right_bottom) / 9;
    }
  }

  return result;
}

// NOTE: You may define additional functions to make the code more readable

void heatmap(mtl_stream &in, mtl_stream &out) {
#pragma HLS INTERFACE axis port=in name=axis_input
#pragma HLS INTERFACE axis port=out name=axis_output
#pragma HLS INTERFACE s_axilite port=return bundle=control

  // TODO: Place your single-round heatmap implementation here
  mtl_stream_data dummy = 0;

  mtl_stream_element element = in.read();

  mtl_stream_data first = dummy;
  mtl_stream_data second = element.data;
  mtl_stream_data third = dummy;

  mtl_stream_data result;
  bool is_last = element.last;

  while (!is_last) {
    element = in.read();

    third = element.data;
    is_last = element.last;

    result = compute_row(first, second, third);
    element.data = result;
    element.last = false;
    out.write(element);

    first = second;
    second = third;
    third = dummy;
  }

  // Also works if there is only one row
  result = compute_row(first, second, third);
  element.data = result;
  element.last = true;
  out.write(element);
}


