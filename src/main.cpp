#include <Arduino.h>
#include "esp_camera.h"

#define SERIAL_BAUD_RATE 115200

#define THRESHOLD (uint8_t) 120

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

void print_pixel_sector(pixel_position_t position) {
  if (150 < position.x && position.x < 170 && 110 < position.y && position.y < 130){
    Serial.printf("Centrum, %d %d \n", position.x, position.y);
  }
  else {
    Serial.printf("X: %d, Y: %d \n", position.x, position.y);
  }
}


void setup() {
  Serial.begin(SERIAL_BAUD_RATE); // Inicjalizacja portu szeregowego
  Serial.println("ESP32-CAM started");
  
  // Inicjalizacja kamery
  camera_config_t config;
  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5;
  config.pin_d1 = 18;
  config.pin_d2 = 19;
  config.pin_d3 = 21;
  config.pin_d4 = 36;
  config.pin_d5 = 39;
  config.pin_d6 = 34;
  config.pin_d7 = 35;
  config.pin_xclk = 0;
  config.pin_pclk = 22;
  config.pin_vsync = 25;
  config.pin_href = 23;
  config.pin_sscb_sda = 26;
  config.pin_sscb_scl = 27;
  config.pin_pwdn = 32;
  config.pin_reset = -1;
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_GRAYSCALE;
  
  // Konfiguracja rozmiaru obrazu
  config.frame_size = FRAMESIZE_QVGA;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }
}



void loop() {
  
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {  // Null czy nullptr?
    Serial.println("Camera capture failed");
    return;
  }
  
  uint32_t x = 0;
  uint32_t y = 0;
  uint32_t weight = 0;

  uint32_t sum_pixel_value = 0;
  for (size_t i = 0; i < fb->len; i++) {
    if(fb->buf[i] > THRESHOLD) {
      pixel_position_t position = get_position(i, fb->width);
      weight += fb->buf[i];
      x += position.x * fb->buf[i];
      y += position.y * fb->buf[i];
    }
  }


  
  pixel_position_t spot = get_spot(x, y, weight);
  
  print_pixel_sector(spot);

  // Zwolnij pamięć po zdjęciu
  esp_camera_fb_return(fb);
  
  // Poczekaj 0.5 sekundy przed wykonaniem kolejnego zdjęcia
  delay(200);
}
