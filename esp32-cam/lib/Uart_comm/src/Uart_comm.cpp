#include <Arduino.h>
#include <esp_camera.h>

void wake_neighbor(){
    unsigned long start = millis();
    Serial.print("WAKE");
    while (!Serial.find("ACK") && (millis() - start <= 1000))
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
