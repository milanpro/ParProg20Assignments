#include "colorfilter.h"
// NOTE: Have a look at colorfilter.h, there are some useful definitions.

component_t grayscale(component_t red, component_t green, component_t blue)
{
  factor_t red_with_factor = ((factor_t)red) * RED_FACTOR;
  factor_t green_with_factor = ((factor_t)green) * GREEN_FACTOR;
  factor_t blue_with_factor = ((factor_t)blue) * BLUE_FACTOR;
  return (uint8_t)(red_with_factor >> 8) +
         (uint8_t)(green_with_factor >> 8) +
         (uint8_t)(blue_with_factor >> 8);
}

pixel_t process_pixel(pixel_t pixel)
{
  component_t comp_a = PIXEL_ALPHA(pixel);
  component_t comp_r = PIXEL_RED(pixel);
  component_t comp_g = PIXEL_GREEN(pixel);
  component_t comp_b = PIXEL_BLUE(pixel);

  if (comp_r < comp_g || comp_r < comp_b)
  {
    component_t comp_gray = grayscale(comp_r, comp_g, comp_b);
    return (comp_a, comp_gray, comp_gray, comp_gray);
  }
  else
  {
    return (comp_a, comp_r, comp_g, comp_b);
  }
}

void process_stream(mtl_stream &in, mtl_stream &out)
{
  mtl_stream_element input, output;
  pixel_t element_idx = 0;

  do
  {
#pragma HLS PIPELINE
    input = in.read();

    // skip header
    if (element_idx < HEADER_ELEMENTS)
    {
      output = input;
      ++element_idx;
    }
    else
    {
      for (int i = 0; i < STREAM_BYTES / 4; ++i)
      {
#pragma HLS UNROLL
        pixel_t pixel_in = SLICE(input.data, i);
        pixel_t pixel_out = process_pixel(pixel_in);
        SLICE(output.data, i) = pixel_out;
      }
      output.last = input.last;
      output.keep = input.keep;
    }

    out.write(output);
  } while (!output.last);
}

void colorfilter(mtl_stream &in, mtl_stream &out)
{
#pragma HLS INTERFACE axis port = in name = axis_input
#pragma HLS INTERFACE axis port = out name = axis_output
#pragma HLS INTERFACE s_axilite port = return bundle = control

#pragma HLS DATAFLOW
  mtl_stream padded_stream;
  mtl_stream processed_stream;

  insert_padding<HEADER_OFFSET>(in, padded_stream);
  process_stream(padded_stream, processed_stream);
  remove_padding<HEADER_OFFSET>(processed_stream, out);
}
