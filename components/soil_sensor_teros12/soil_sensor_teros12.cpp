#include"soil_sensor_teros12.h"
#include "esphome/core/log.h"
#include"esp32-sdi12.h"

static const char *TAG = "Teros 12 Soil Sensor" ;
static ESP32_SDI12::Sensors sensors;
static float values[10];
static uint8_t addr;
static uint8_t n_returned_values;
static int number = 0 ;
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
      //pinMode(this->pin_data, INPUT);
      
      this->serialSDI12 =  new ESP32_SDI12(this->pin_data);
      
      
      
      this->serialSDI12->begin();

      ESP_LOGI(TAG, "Initialization Completed");

     };

     void Soil_Sensor_Teros12::dump_config(){
       ESP_LOGCONFIG(TAG, "Teros 12 Soil Sensor Configurations");
       ESP_LOGCONFIG(TAG, "Sensor DATA PIN : %d ",this->pin_data);
      
     };
  
     /** Read and publish data to the sensors
     */
     void Soil_Sensor_Teros12::update(){
        ESP_LOGI(TAG, "Starting new measurement from the sensor");
        if(number==0){
          scanBus();
        }else {
           readSensors();
        }
        


        number++;
        if(number==11){
          number=0;
        }
     };  


     void Soil_Sensor_Teros12::scanBus(){
      // Initialise SDI-12 pin definition
          // Get a list of sensors on the bus on startup
          ESP32_SDI12::Status res = serialSDI12->sensorsOnBus(&sensors);

          if(res != ESP32_SDI12::SDI12_OK){
              // No sensors found, stop execution
              ESP_LOGE(TAG,"No SDI-12 sensors found. Stopping.\n");
              
          } else {
              // Devices found
              ESP_LOGI(TAG,"%d SDI-12 sensors found.\n", sensors.count);
          }
     }


     void Soil_Sensor_Teros12::readSensors(){
       // Loop through sensors that were found on the bus
        for(uint8_t i = 0; i < sensors.count; i++){
           // Get the sensors address
           addr = sensors.sensor[i].address;

            // Take a measurement
            ESP32_SDI12::Status res = serialSDI12->measure(addr, values,
                                                sizeof(values),
                                                &n_returned_values);

            // Handle response status
            if(res  == ESP32_SDI12::SDI12_OK){
                 // Measure was successful, print values
                ESP_LOGI(TAG,"Measure complete.\n");
                for(uint8_t v = 0; v < n_returned_values; v++){
                  ESP_LOGI(TAG,"Value %d: %f\n", v + 1, values[v]);
                }
              } else {
                 // Measure was unsuccessful check res against Status enum
                ESP_LOGI(TAG,"Error: %d\n", res);
            }
          }

     }



  };
};