#include <metal/stream.h>

#define RED_FACTOR 54
#define GREEN_FACTOR 183
#define BLUE_FACTOR 18

#define HEADER_BYTES 138
#define HEADER_ELEMENTS ((HEADER_BYTES / STREAM_BYTES) + 1)
#define HEADER_OFFSET (STREAM_BYTES - (HEADER_BYTES % STREAM_BYTES))

#define COMPONENT_BITS 8
typedef ap_uint<COMPONENT_BITS> component_t;

#define FACTOR_BITS (2 * COMPONENT_BITS)
typedef ap_uint<FACTOR_BITS> factor_t;

#define PIXEL_BITS (4 * COMPONENT_BITS)
typedef ap_uint<PIXEL_BITS> pixel_t;

#define PIXEL_ALPHA(pix) (pix(4 * COMPONENT_BITS - 1, 3 * COMPONENT_BITS))
#define PIXEL_RED(pix) (pix(3 * COMPONENT_BITS - 1, 2 * COMPONENT_BITS))
#define PIXEL_GREEN(pix) (pix(2 * COMPONENT_BITS - 1, 1 * COMPONENT_BITS))
#define PIXEL_BLUE(pix) (pix(1 * COMPONENT_BITS - 1, 0 * COMPONENT_BITS))

#define STREAM_BITS (STREAM_BYTES * 8)
#define STREAM_PIXELS (STREAM_BITS / PIXEL_BITS)
#define SLICE(word, idx) (word(PIXEL_BITS * ((idx) + 1) - 1, PIXEL_BITS * idx))

void colorfilter(mtl_stream &in, mtl_stream &out);
