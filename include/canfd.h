#ifndef CANFD_H
#define CANFD_H
#include <stdint.h>
#define CANFD_MAX_DATA 64u
typedef struct { uint32_t id; uint8_t dlc; uint8_t data[CANFD_MAX_DATA]; } CanFdFrame;
#endif
