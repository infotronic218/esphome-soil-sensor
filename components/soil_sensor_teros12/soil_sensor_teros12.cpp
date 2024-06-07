#include"soil_sensor_teros12.h"
#include "esphome/core/log.h"
#include<SDI12.h>

static const char *TAG = "Teros 12 Soil Sensor" ;
// Calculate CRC
static uint8_t calculateCRC(const byte *data, size_t len) {
  uint8_t crc = 0;
  for (size_t i = 0; i < len; i++) {
    crc ^= data[i];
  }
  return crc;
}

namespace esphome {
  namespace  soil_sensor_teros12_{
     void Soil_Sensor_Teros12::setup(){
      ESP_LOGI(TAG, "Initialization");
      pinMode(this->pin_data, INPUT);
      this->serialSDI12 =  new SDI12(this->pin_data);
      this->serialSDI12->begin();
     };

     void Soil_Sensor_Teros12::dump_config(){
       ESP_LOGCONFIG(TAG, "Teros 12 Soil Sensor Configurations");
       ESP_LOGCONFIG(TAG, "Sensor DATA PIN : %d ",this->pin_data);
      
     };
  
     /** Read and publish data to the sensors
     */
     void Soil_Sensor_Teros12::update(){
        ESP_LOGI(TAG, "Starting new measurement from the sensor");
        scanAddressSpace(this->serialSDI12);
     };  


    void Soil_Sensor_Teros12::printInfo(SDI12 *sdi, char i){
        String command = "";
        command += (char)i;
        command += "I!";
        sdi->sendCommand(command);
        sdi->clearBuffer();
        delay(30);

        ESP_LOGI(TAG,"  --");
        ESP_LOGI(TAG,"%c",i);
        ESP_LOGI(TAG, "--  ");

        while (sdi->available()) {
          ESP_LOGI(TAG,"%d",sdi->read());
          delay(10);  // 1 character ~ 7.5ms
        }

    }

    boolean Soil_Sensor_Teros12::checkActive(SDI12 *sdi, char i){
      String myCommand = "";
      myCommand        = "";
      myCommand += (char)i;  // sends basic 'acknowledge' command [address][!]
      myCommand += "!";

      for (int j = 0; j < 3; j++) {  // goes through three rapid contact attempts
        sdi->sendCommand(myCommand);
        sdi->clearBuffer();
        delay(30);
        if (sdi->available()) {  // If we here anything, assume we have an active sensor
          return true;
        }
  }
  sdi->clearBuffer();
  return false;

    }

    void Soil_Sensor_Teros12::scanAddressSpace(SDI12 *sdi){
            // scan address space 0-9
        for (char i = '0'; i <= '9'; i++)
          if (checkActive(sdi, i)) { printInfo(sdi, i); }
        // scan address space a-z
        for (char i = 'a'; i <= 'z'; i++)
          if (checkActive(sdi, i)) { printInfo(sdi, i); }
        // scan address space A-Z
        for (char i = 'A'; i <= 'Z'; i++)
          if (checkActive(sdi, i)) { printInfo(sdi, i); };
      }

  };
};