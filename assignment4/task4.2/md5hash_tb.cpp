#include "md5hash.h"

#include <iostream>


const char lineA_buf[64] = "FifteenDivisibleTannedCrawlersSkilletExemplarySteepTrac\x80\xb8\x01\x00\x00\x00\x00\x00";
const char lineB_buf[64] = "TrilogyHandinessGallowsUnsignedSponsorPetticoatThinnerE\x80\xb8\x01\x00\x00\x00\x00\x00";
const char lineC_buf[64] = "FamiliarMolecularDoorstopDominionAmiableRigorRetreatBlo\x80\xb8\x01\x00\x00\x00\x00\x00";

const char search_buf[64]  = {0x5d, 0x6c, 0x73, 0xd8, 0xdf, 0xce, 0xf7, 0x14, 0xfe, 0xd8, 0x8f, 0x58, 0x4f, 0x04, 0xe6, 0x3f};

void print_hex(const char * msg, const unsigned char * buffer, size_t length) {
  std::cout << msg << "0x";
  for(int i = 0; i < length; i++) {
    std::cout << std::hex << std::setw(2) << std::setfill('0') << (unsigned) buffer[i];
  }
  std::cout << std::endl;
}

int main() {
  hash_t search;
  mtl_stream in;
  mtl_stream out;
  mtl_stream_element element;


  memcpy((char *) search.getRawData(), search_buf, 16);
  print_hex("Hash:  ", (unsigned char *) search.getRawData(), 16);


  element.last = false;
  element.keep = (mtl_stream_keep) -1;
  memcpy(&element.data, lineA_buf, 64);
  print_hex("LineA: ", (unsigned char *) &element.data, 64);
  in.write(element);

  element.last = false;
  element.keep = (mtl_stream_keep) -1;
  memcpy(&element.data, lineB_buf, 64);
  print_hex("LineB: ", (unsigned char *) &element.data, 64);
  in.write(element);

  element.last = true;
  element.keep = (mtl_stream_keep) -1;
  memcpy(&element.data, lineC_buf, 64);
  print_hex("LineC: ", (unsigned char *) &element.data, 64);
  in.write(element);


  md5hash(in, out, search);


  element = out.read();
  print_hex("Out:   ", (unsigned char *) &element.data, 64);
  print_hex("Exp:   ", (unsigned char *) lineB_buf, 64);

  if (memcmp(lineB_buf, &element.data, 64) != 0) {
    std::cout << "Output mismatch!" << std::endl;
    return 1;
  }

  std::cout << "Success" << std::endl;
  return 0;
}
