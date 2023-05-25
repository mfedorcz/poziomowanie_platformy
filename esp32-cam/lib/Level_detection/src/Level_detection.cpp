#include <Arduino.h>

typedef struct {
  uint32_t x = 0;
  uint32_t y = 0;
} pixel_position_t;

pixel_position_t get_position(size_t index, size_t width) {
  pixel_position_t position;
  position.x = index % width;
  position.y = index / width;
  return position;
}

pixel_position_t get_spot(uint32_t x, uint32_t y, uint32_t weight) {
  pixel_position_t position;
  if (weight == 0){
    return position;
  }
  position.x = x / weight;
  position.y = y / weight;
  return position;
}

int is_center(pixel_position_t position) {
  if (620 < position.x && position.x < 660 && 340 < position.y && position.y < 380){
   return 1;
  } else if (position.y < 340) {
    return 2;
  } else if (position.y > 380)
  {
    return 3;
  }  
  return 0;
}