#include <Arduino.h>
#include <esp_camera.h>

void wake_neighbor(){
    Serial.print("WAKE");
    while (!Serial.find("ACK"))
    {
        Serial.print("WAKE");
    }
}


void wait_for_signal(){
    while (!Serial2.find("WAKE"))
    {
    }
    unsigned long start = millis();
    while (millis() - start <= 3000)
    {
        Serial2.print("ACK");
    }
}
