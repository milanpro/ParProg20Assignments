#include <altivec.h>

//Color invert constants
vector unsigned char invertMask = {255, 255, 255, 0,
                                   255, 255, 255, 0,
                                   255, 255, 255, 0,
                                   255, 255, 255, 0};

vector unsigned char invertRMask = {255, 0, 255, 0,
                                    255, 0, 255, 0,
                                    255, 0, 255, 0,
                                    255, 0, 255, 0};

vector unsigned char alphaPattern = {0x00, 0x01, 0x02, 0x13,
                                     0x04, 0x05, 0x06, 0x17,
                                     0x08, 0x09, 0x0a, 0x1b,
                                     0x0c, 0x0d, 0x0e, 0x1f};

//RGB to GRAY constants
#define RCONST 0.29891f
#define GCONST 0.58661f
#define BCONST 0.11448f

vector float rFactor = {RCONST, RCONST, RCONST, RCONST};
vector float gFactor = {GCONST, GCONST, GCONST, GCONST};
vector float bFactor = {BCONST, BCONST, BCONST, BCONST};

vector unsigned char bPattern = {0x00, 0x10, 0x10, 0x10,
                                 0x04, 0x10, 0x10, 0x10,
                                 0x08, 0x10, 0x10, 0x10,
                                 0x0c, 0x10, 0x10, 0x10};

vector unsigned char gPattern = {0x01, 0x10, 0x10, 0x10,
                                 0x05, 0x10, 0x10, 0x10,
                                 0x09, 0x10, 0x10, 0x10,
                                 0x0d, 0x10, 0x10, 0x10};

vector unsigned char rPattern = {0x02, 0x10, 0x10, 0x10,
                                 0x06, 0x10, 0x10, 0x10,
                                 0x0a, 0x10, 0x10, 0x10,
                                 0x0e, 0x10, 0x10, 0x10};

vector unsigned char grayPattern = {0x00, 0x00, 0x00, 0x13,
                                    0x04, 0x04, 0x04, 0x17,
                                    0x08, 0x08, 0x08, 0x1b,
                                    0x0c, 0x0c, 0x0c, 0x1f};

vector unsigned char zeroVector = {0, 0, 0, 0,
                                   0, 0, 0, 0,
                                   0, 0, 0, 0,
                                   0, 0, 0, 0};

vector unsigned char alphaMask = {0, 0, 0, 255,
                                  0, 0, 0, 255,
                                  0, 0, 0, 255,
                                  0, 0, 0, 255};

vector unsigned int maxPattern = {255, 255, 255, 255};
