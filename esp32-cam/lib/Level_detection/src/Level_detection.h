#ifndef LEVEL_DETECTION_H
#define LEVEL_DETECTION_H
#include <Arduino.h>

typedef struct {
  uint32_t x = 0;
  uint32_t y = 0;
} pixel_position_t;

pixel_position_t get_position(size_t index, size_t width);

pixel_position_t get_spot(uint32_t x, uint32_t y, uint32_t weight);

int is_center(pixel_position_t position);

#endif