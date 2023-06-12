#include <Arduino.h>
#include "esp_camera.h"
#include <Level_detection.h>
#include <Uart_comm.h>
#include <Motor_movement.h>

// MODE 
// #define INITIAL_CAMERA  /* comment for slaves */
// #define DEBUG_CAMERA
// #define DEBUG_UART

// PINS

#define SERIAL_BAUD_RATE 115200

#define THRESHOLD (uint8_t) 255 /* default treshold 120 */ 

#define FLASH_PIN (int8_t) 4

#define SLAVE_TX (int8_t) 14
#define SLAVE_RX (int8_t) 2

// Motor statistics
CurrentSet currentSet(2, 1000);


void setup() {
  // UART
  Serial.begin(SERIAL_BAUD_RATE);
  Serial2.begin(SERIAL_BAUD_RATE, SERIAL_8N1, SLAVE_RX, SLAVE_TX);  

  // Motor Movement
  pinMode(stepPin, OUTPUT);
	pinMode(dirPin, OUTPUT);
  
  // LED indicator
#ifdef DEBUG
  pinMode(FLASH_PIN, OUTPUT);
#endif
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
  config.frame_size = FRAMESIZE_HD;
  config.jpeg_quality = 12;
  config.fb_count = 1;
  
  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera init failed with error 0x%x", err);
    return;
  }

  // TODO delete this line

#ifdef INITIAL_CAMERA
  wake_neighbor();
#endif
}

void loop() {
#ifdef DEBUG_UART
  digitalWrite(FLASH_PIN, HIGH);
  wait_for_signal();
  digitalWrite(FLASH_PIN, LOW);
# elif defined DEBUG_CAMERA
  
#else
  wait_for_signal();
#endif

  currentSet.setStats(2, 1000);

  while (1) {
    int treshold_count = 0;
    camera_fb_t * fb = esp_camera_fb_get();
    if (!fb) {  
      Serial.println("ERROR");
      return;
    }
    
    uint32_t x = 0;
    uint32_t y = 0;
    uint32_t weight = 0;
    u_short result;
    int max_amount = 0;
    int min_amount = 0;
    int normal_amount = 0;
    //Serial.printf("fb->LEN: %d\n", fb->len);
    //Serial.printf("fb->width: %d\n", fb->width);
    //Serial.printf("fb->height: %d\n", fb->height);
    uint32_t sum_pixel_value = 0;
    for (size_t i = 0; i < fb->len; i++) {
      if(fb->buf[i] >= THRESHOLD) {
        
        treshold_count++;
        pixel_position_t position = get_position(i, fb->width);

        if(position.x > 430 && position.x < 850) {
          weight += fb->buf[i];
          x += position.x * fb->buf[i];
          y += position.y * fb->buf[i];
          max_amount++;
        }

        //printf("i: %d, X: %d, Y: %d\n", i, position.x, position.y);
      } else if(fb->buf[i] > 0 && fb->buf[i] < 255) {
        normal_amount++;
      } else if(fb->buf[i] == 0){
        min_amount++;
      }
      
    }

    #ifdef DEBUG_CAMERA
    Serial.printf("Threshold amount: %d\n", treshold_count);
    Serial.printf("Min amount: %d\n", min_amount);
    Serial.printf("Normal amount: %d\n", normal_amount);
    Serial.printf("Max brightness: %d\n", max_amount);
    #endif

    pixel_position_t spot = get_spot(x, y, weight);

    result = is_center(spot);
      
    #ifdef DEBUG_CAMERA
    Serial.printf("Point: x:%d, y:%d, is_center: %d\n", spot.x, spot.y, result);
    #endif

    esp_camera_fb_return(fb);

    if(result == 0 || result == 1) {
      break;    // Getting out of while(1)
    } else if(result == 3) {
      motor_move_down(currentSet.stepsPerRevolution, 1000);
    } else if(result == 2) {
      motor_move_up(currentSet.stepsPerRevolution, 1000);
    }

    currentSet.setMotorStats(result);
  }

  #ifndef DEBUG_CAMERA
    wake_neighbor();
  #endif
  
  delay(1000);
}
