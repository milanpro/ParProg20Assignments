#include "heatmap.h"

#include <fstream>
#include <iostream>


int main() {
  mtl_stream in;
  mtl_stream tmp;
  mtl_stream out;

  mtl_stream_data first = 0;
  mtl_stream_data second = 0;
  mtl_stream_data third = 0;

  first(15,8) = 0xff;
  second(255, 248) = 0xff;
  second(263, 256) = 0xff;

  mtl_stream_element element;
  element.keep = ~(mtl_stream_keep)0;

  element.data = first;
  in.write(element);
  element.data = second;
  in.write(element);
  element.last = true;
  element.data = third;
  in.write(element);

  heatmap(in, tmp);
  heatmap(tmp, out);

  std::stringstream expected, actual;
  expected
    << "1X10000000000000000000000000001221000000000000000000000000000000" << std::endl
    << "1110000000000000000000000000001XX1000000000000000000000000000000" << std::endl
    << "0000000000000000000000000000001221000000000000000000000000000000" << std::endl;

  do {
    element = out.read();

    for (int j=0; j < ROW_CELLS; j++) {
      unsigned const value = SLICE(element.data, j);
      if (value == 0xff) {
          actual << "X";
      } else {
          actual << 9 * value / 255;
      }
    }
    actual << std::endl;
  } while (!element.last);

  std::cout << actual.str();

  if (expected.str() != actual.str()) {
      std::cout << "Did not receive expected result" << std::endl;
      return 1;
  }

  std::cout << "Success." << std::endl;
  return 0;
}

