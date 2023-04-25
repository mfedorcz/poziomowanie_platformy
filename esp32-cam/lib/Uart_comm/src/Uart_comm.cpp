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
    Serial2.print("ACK");
}
