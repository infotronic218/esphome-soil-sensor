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
      

       

     };

     void Soil_Sensor_Teros12::dump_config(){
       ESP_LOGCONFIG(TAG, "Teros 12 Soil Sensor Configurations");
       ESP_LOGCONFIG(TAG, "Sensor DATA PIN : %d ",this->pin_data);
      
     };
  
     /** Read and publish data to the sensors
     */
     void Soil_Sensor_Teros12::update(){
         ESP_LOGI(TAG, "Starting new measurement");


     };  
  };
};