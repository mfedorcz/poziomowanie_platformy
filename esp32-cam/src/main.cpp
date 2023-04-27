#include <Arduino.h>
#include "esp_camera.h"
#include <Level_detection.h>
#include <Uart_comm.h>
#include <Motor_movement.h>

#define SERIAL_BAUD_RATE 115200

#define THRESHOLD (uint8_t) 120


void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, 2, 14);  
  
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
  wait_for_signal();
  
  camera_fb_t * fb = esp_camera_fb_get();
  if (!fb) {  
    Serial.println("ERROR");
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
  
  

  if (is_center(spot))
  {
    wake_neighbor();
  }
  
  esp_camera_fb_return(fb);
  
  delay(200);
}
