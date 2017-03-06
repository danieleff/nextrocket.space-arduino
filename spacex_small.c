#include "stdint.h"
#include "CompressedImageData.h"

static const uint8_t palette[] = {
 0x00,  0x4f,  0x85,  0x00,  0x4f,  0x86,  0x00,  0x4f,  0x88,  0x00,  0x4f,  0x89,  0x00,  0x50,  0x85,  0x00, 
 0x50,  0x86,  0x00,  0x50,  0x87,  0x00,  0x50,  0x88,  0x00,  0x50,  0x89,  0x00,  0x51,  0x86,  0x00,  0x51, 
 0x87,  0x00,  0x51,  0x88,  0x00,  0x51,  0x89,  0x00,  0x52,  0x85,  0x00,  0x52,  0x86,  0x00,  0x52,  0x87, 
 0x00,  0x52,  0x88,  0x00,  0x52,  0x89,  0x00,  0x53,  0x86,  0x00,  0x53,  0x87,  0x00,  0x53,  0x88,  0x00, 
 0x53,  0x89,  0xa4,  0xa6,  0xac,  0xa5,  0xa6,  0xac,  0xa5,  0xa7,  0xa9,  0xa6,  0xa7,  0xac,  0xa7,  0xa7, 
 0xac,  0xa5,  0xa8,  0xac,  0xa6,  0xa8,  0xaa,  0xa5,  0xa8,  0xad,  0xa6,  0xa8,  0xab,  0xa6,  0xa8,  0xad, 
 0xa8,  0xa8,  0xa9,  0xa7,  0xa8,  0xac,  0xa8,  0xa8,  0xac,  0xa5,  0xa9,  0xab,  0xa8,  0xa8,  0xad,  0xa5, 
 0xa9,  0xac,  0xa6,  0xa9,  0xaa,  0xa6,  0xa9,  0xab,  0xa6,  0xa9,  0xac,  0xa6,  0xa9,  0xad,  0xa7,  0xa9, 
 0xab,  0xa7,  0xa9,  0xac,  0xa7,  0xa9,  0xad,  0xa8,  0xa9,  0xab,  0xa8,  0xa9,  0xad,  0xa6,  0xaa,  0xab, 
 0xa7,  0xaa,  0xab,  0xa7,  0xaa,  0xad,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff, 
 0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff, 
 0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff,  0xff, 
};

static const uint8_t compressed_data[] = {
 0x9f,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00,  0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c, 
 0x01,  0xe0,  0x0f,  0x00,  0x74,  0x9e,  0x46,  0x11,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00, 
 0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xd3,  0x09,  0x5c,  0xa8,  0x25,  0xe0, 
 0x0f,  0x00,  0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00,  0x78, 
 0x03,  0xa2,  0xd1,  0xc9,  0x5c,  0xae,  0x52,  0x14,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00, 
 0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xd1,  0xb6,  0x10,  0x80,  0x06,  0x46, 
 0x15,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00,  0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07, 
 0x80,  0x3c,  0x01,  0xb2,  0xc9,  0x53,  0x0f,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0, 
 0x0f,  0x00,  0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x0d,  0x86,  0x40,  0x01,  0x96,  0x05, 
 0x7c,  0x01,  0xe0,  0x0f,  0x00,  0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0, 
 0x0f,  0x00,  0x6c,  0x69,  0x88,  0x92,  0x40,  0xb7,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00,  0x78,  0x03, 
 0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0,  0x0e,  0x62,  0x24,  0xac,  0x2b,  0xe0,  0x0f, 
 0x00,  0x78,  0x03,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0x80,  0x3c,  0x01,  0xe0,  0x0f,  0x00,  0x78,  0x03, 
 0x63,  0x4c,  0x34,  0x90,  0x85,  0xbc,  0x01,  0xe0,  0x04,  0x88,  0x00,  0x3e,  0x22,  0x1a,  0xf0,  0x06, 
 0x0e,  0xae,  0x14,  0x4b,  0xf0,  0x00,  0xc4,  0x45,  0xfe,  0x31,  0xa0,  0xc7,  0x87,  0x68,  0x57,  0xe0, 
 0x04,  0x92,  0xb1,  0x52,  0x4c,  0x62,  0xf0,  0x07,  0x80,  0x12,  0x1e,  0x57,  0xf2,  0x6d,  0x85,  0xfc, 
 0xd5,  0xd0,  0x68,  0x84,  0x20,  0x9d,  0xf1,  0x31,  0x6f,  0x93,  0xbc,  0x01,  0xe1,  0xc7,  0x85,  0x83, 
 0x56,  0x1e,  0x4a,  0x99,  0x0b,  0x15,  0x25,  0x46,  0x1f,  0x00,  0x79,  0x6f,  0xc0,  0x17,  0x00,  0x0e, 
 0x79,  0xe3,  0xc3,  0xd0,  0x4e,  0xf1,  0x67,  0x8c,  0xb0,  0x2d,  0xee,  0x4f,  0x07,  0xdc,  0x65,  0x8a, 
 0x92,  0xb2,  0xff,  0x80,  0x3c,  0xf1,  0x81,  0x3f,  0x0e,  0x78,  0x43,  0xc0,  0x0d,  0x06,  0x0f,  0x78, 
 0x81,  0xa1,  0x24,  0x6f,  0x00,  0x78,  0x03,  0xc0,  0x0b,  0x0f,  0x64,  0x34,  0x38,  0x16,  0x52,  0xe6, 
 0x2f,  0x80,  0x3c,  0x01,  0xea,  0xaf,  0x0e,  0x7b,  0x1f,  0xc2,  0x19,  0x0b,  0x06,  0x94,  0x2c,  0x63, 
 0xe0,  0x01,  0x86,  0x11,  0xbc,  0x01,  0xe0,  0x0f,  0x1e,  0x5b,  0x1d,  0x18,  0xab,  0x1a,  0x0f,  0xf8, 
 0x03,  0xc7,  0x7e,  0x13,  0xf0,  0xe7,  0x84,  0x3c,  0x1d,  0x8c,  0x8d,  0x63,  0xf8,  0x03,  0xc0,  0x1e, 
 0x6f,  0xf8,  0x84,  0x14,  0x0a,  0x32,  0xa6,  0x32,  0xc6,  0x03,  0xbe,  0x00,  0xf1,  0xd7,  0xb2,  0x3d, 
 0x6d,  0xe1,  0x0f,  0x00,  0x4c,  0x24,  0x41,  0xec,  0x7f,  0x27,  0x78,  0x02,  0xc3,  0xbe,  0x00,  0x31, 
 0xd1,  0x48,  0xd8,  0xab,  0x2a,  0x0d,  0x78,  0x03,  0xcb,  0x7e,  0x00,  0xf3,  0x15,  0x82,  0xce,  0x1f, 
 0x13,  0x00,  0x44,  0x2c,  0xff,  0xe3,  0xd5,  0x08,  0x7b,  0x1f,  0xcd,  0x7e,  0xc7,  0xf0,  0x06,  0x31, 
 0x56,  0x56,  0x16,  0xf0,  0x07,  0x80,  0x3c,  0xb9,  0xe6,  0x3d,  0x0b,  0x78,  0x00,  0xe1,  0x63,  0xdf, 
 0x48,  0xe3,  0x1f,  0xc5,  0x9e,  0xbd,  0xf0,  0x07,  0x4a,  0x98,  0xaf,  0x0f,  0x22,  0xf8,  0x03,  0xc0, 
 0x1e,  0x00,  0xf3,  0x6e,  0x25,  0x52,  0x26,  0x00,  0xb8,  0xa8,  0xb3,  0xd8,  0xfe,  0xc7,  0xf1,  0x67, 
 0x84,  0xfc,  0x01,  0x8c,  0x55,  0x63,  0x11,  0x5b,  0xc0,  0x1e,  0x00,  0xf0,  0x07,  0xb1,  0xfc,  0x9b, 
 0xe0,  0x06,  0x83,  0x99,  0xd0,  0x01,  0x2c,  0x7f,  0x00,  0x79,  0x3b,  0xc0,  0x1e,  0x00,  0x59,  0x13, 
 0x1b,  0x45,  0x7e,  0x00,  0xf0,  0x07,  0x80,  0x3d,  0x8f,  0xe0,  0x0f,  0x33,  0x43,  0x23,  0xd8,  0xfe, 
 0x00,  0xf2,  0x77,  0x80,  0x3a,  0x3c,  0xc5,  0x58,  0xf1,  0x82,  0x84,  0x5e,  0x00,  0xf0,  0x07,  0x9b, 
 0x7d,  0x8f,  0xe0,  0x0e,  0x88,  0x80,  0x2c,  0xe0,  0x90,  0xf1,  0xe5,  0xc3,  0x85,  0xfe,  0x59,  0xf0, 
 0x07,  0x80,  0x11,  0x8a,  0xb2,  0xb9,  0x18,  0x31,  0x20,  0xcc,  0x86,  0x86,  0x09,  0x7c,  0x01,  0xe5, 
 0xab,  0x00,  0x7b,  0x1f,  0xc0,  0x1e,  0x00,  0xe8,  0x70,  0x94,  0xe0,  0x84,  0xff,  0x27,  0x78,  0x71, 
 0xe1,  0x51,  0x30,  0x07,  0x06,  0xda,  0x32,  0xc6,  0x58,  0xd2,  0x63,  0xcd,  0xbe,  0x00,  0xf0,  0x03, 
 0x3e,  0x1c,  0x01,  0xb0,  0xb6,  0x3f,  0x80,  0x3c,  0x01,  0x86,  0xfb,  0x87,  0x92,  0xbd,  0x8f,  0xe0, 
 0x0f,  0x0d,  0xb4,  0x7d,  0x8c,  0xb2,  0xb2,  0xac,  0xc2,  0xc3,  0xed,  0x0e,  0x37,  0x78,  0x03,  0xd9, 
 0x1e,  0x00,  0x98,  0x4a,  0x86,  0xc6,  0x1e,  0x00,  0xf6,  0x47,  0x12,  0xfc,  0x85,  0x61,  0x7f,  0x0e, 
 0x4c,  0x69,  0x8d,  0xe2,  0xea,  0x36,  0xf8,  0x03,  0xc0,  0x08, };

compressed_image_t spacex_small = {
  682,
  200,
  25,
  64,
  palette,
  compressed_data
};

