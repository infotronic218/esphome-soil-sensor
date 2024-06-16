#include"soil_sensor_teros12.h"
#include "esphome/core/log.h"
#include"esp32-sdi12.h"

static const char *TAG = "Teros 12 Soil Sensor" ;
static ESP32_SDI12::Sensors sensors;
static float values[10];
static uint8_t addr;
static uint8_t start_address=0;
static bool address_founded;
static uint8_t addr_found;
static int default_addr = -10;
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
      start_address= 0;

      ESP_LOGI(TAG, "Initialization Completed");

     };

     void Soil_Sensor_Teros12::dump_config(){
       ESP_LOGCONFIG(TAG, "Teros 12 Soil Sensor Configurations");
       ESP_LOGCONFIG(TAG, "Sensor DATA PIN : %d ",this->pin_data);
      
     };

     bool Soil_Sensor_Teros12::scan_mode_switch_selected(){
      if(this->_scan_mode_switch !=nullptr){
        return this->_scan_mode_switch->state ;
      }
      return false ;

     }
  
     /** Read and publish data to the sensors
     */
     void Soil_Sensor_Teros12::update(){
       
      
        if(scan_mode_switch_selected()){
            // SCAN MODE 
            if(start_address==0){
              ESP_LOGI(TAG,"START SDI-12 SENSORS SCANNING ...");
              address_founded = false ;
            }
            // san to addr at once not to block the board
            int index = 0 ;
            ESP_LOGI(TAG, "SDI-12 Scanning ...");
            while(start_address<10 && index<4){
              // Check if the current address is available
              if (this->serialSDI12->querySensor(address, ESP32_SDI12::SDI12_Information) == ESP32_SDI12::SDI12_OK,0){
                  ESP_LOGI(TAG, "SDI Sensor Found on the bus at : %d ", start_address);
                  if(!address_founded){
                    addr_found = address ;
                  }
                  address_founded= true ;
              }
              start_address++;
              index++;
            }
            
            if(start_address==10){
            ESP_LOGI(TAG,"END SDI-12 SENSORS SCANNING ...");
            start_address=0;
            if(!address_founded){
              ESP_LOGI(TAG,"No SDI Sensor Found on the bus ");
            }else{
               default_addr =  addr_found ;
            }
            }
            
        }else {
          if(address!=100){
            // Using the define address sensor

            readSensorData(address);
          }else if(address_founded){
            // Use the first address found in the scanning 
            readSensorData(default_addr);
          }else{
            ESP_LOGI(TAG, "No sensor addr to read ");
          }
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


     void Soil_Sensor_Teros12::readSensorData(uint8_t addr){
            ESP_LOGI(TAG, "Starting new measurement from the sensor");
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
                if(n_returned_values==3){
                    if(this->temperature_sensor_!=nullptr){
                      this->temperature_sensor_->publish_state(values[1]);
                    }
                    if(this->ec_sensor_!=nullptr){
                      this->ec_sensor_->publish_state(values[2]);
                    }

                    if(this->vwc_sensor_!=nullptr){
                      this->vwc_sensor_->publish_state(values[0]);
                    }
                }

              } else {
                 // Measure was unsuccessful check res against Status enum
                ESP_LOGI(TAG,"Error: %d\n", res);
            }
          

     }


    void DigitalSwitch::setup(){
       this->pinState = true ;
    }

    void DigitalSwitch::write_state(bool state){
        this->pinState = state ;
        this->publish_state(state);
    }

    void DigitalSwitch::dump_config(){   
    }
  };
};