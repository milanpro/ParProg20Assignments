#include "colorfilter.h"
// NOTE: Have a look at colorfilter.h, there are some useful definitions.



component_t grayscale(component_t red, component_t green, component_t blue) {

  // TODO: Implement efficient grayscale conversion.

}

void process_stream(mtl_stream &in, mtl_stream &out) {

  // TODO: Transform pixels in the input stream element
  //       (pixels per element might vary ~ STREAM_BYTES)
  //       Do not modify header bytes!
  //       Padding in colorfilter() ensures
  //       that header ends on a stream element boundary.

}

void colorfilter(mtl_stream &in, mtl_stream &out) {
    #pragma HLS INTERFACE axis port=in name=axis_input
    #pragma HLS INTERFACE axis port=out name=axis_output
    #pragma HLS INTERFACE s_axilite port=return bundle=control

    #pragma HLS DATAFLOW
    mtl_stream padded_stream;
    mtl_stream processed_stream;

    insert_padding<HEADER_OFFSET>(in, padded_stream);
    process_stream(padded_stream, processed_stream);
    remove_padding<HEADER_OFFSET>(processed_stream, out);
}
