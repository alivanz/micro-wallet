#include <Arduino.h>
#include <WiFi.h>
#include "trng.hpp"
#include <esp_wifi.h>

namespace TRNG{
  void Init(){
//    if(esp_wifi_start()!=ESP_OK){
//      throw "unable to turn on wifi";
//    }
  }
    
  uint32_t Read(){
    return esp_random();
  }
}
