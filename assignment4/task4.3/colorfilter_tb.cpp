#include "colorfilter.h"

#include <fstream>

int main() {
  mtl_stream in;
  mtl_stream out;
  mtl_stream_element element;

  std::ifstream infile("/workspace/apples.bmp", std::ifstream::in | std::ifstream::binary);
  do {
    infile.read((char *)&element.data, STREAM_BYTES);
    element.keep = (mtl_stream_keep)-1;
    element.last = infile.eof();
    in.write(element);
  } while (!element.last);
  infile.close();

  colorfilter(in, out);

  std::ofstream outfile("/workspace/apples.out.bmp", std::ofstream::out | std::ofstream::binary);
  do {
    element = out.read();
    outfile.write((char *)&element.data, STREAM_BYTES);
  } while (!element.last);
  outfile.close();

  return 0;
}
