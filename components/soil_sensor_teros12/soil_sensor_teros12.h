/* The code is a modification of the sources codes on thes links
https://github.com/Emperiusm/TDR-Sensor/blob/main/tdr_soil_sensor.ino
https://scienceinhydroponics.com/2023/01/connecting-a-low-cost-tdr-moisture-content-ec-temp-sensor-to-a-nodemcuv3.html

*/


#ifndef __TDR_SOIL_SENSOR__
#define __TDR_SOIL_SENSOR__

#include "esphome/core/component.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/switch/switch.h"
#include<Arduino.h>

#include"esp32-sdi12.h"
namespace esphome {
  namespace  soil_sensor_teros12_{
     class DigitalSwitch;
     class Soil_Sensor_Teros12 : public PollingComponent{
       private:
       sensor::Sensor *temperature_sensor_{nullptr} ;
       sensor::Sensor *ec_sensor_{nullptr} ;
       sensor::Sensor *vwc_sensor_{nullptr} ;
       soil_sensor_teros12_::DigitalSwitch *_scan_mode_switch{nullptr} ;
       uint8_t pin_data;
       uint8_t address{100} ;
       ESP32_SDI12 *serialSDI12;
       public:
       void setup() override;
       void update() override ;
       void dump_config() override;
       float get_setup_priority() const override { return esphome::setup_priority::HARDWARE; }
       
       void scanBus();
       void readSensors();
       void readSensorData(uint8_t addr);
       void set_scan_mode_switch(soil_sensor_teros12_::DigitalSwitch  *sw){this->_scan_mode_switch = sw ;}
       bool scan_mode_switch_selected();
       void set_temperature_sensor(sensor::Sensor *s){this->temperature_sensor_ = s ;}
       void set_ec_sensor(sensor::Sensor *s) {this->ec_sensor_ = s ;}
       void set_vwc_sensor(sensor::Sensor *s) {this->vwc_sensor_ = s ;}
      
       void set_pin_data(uint8_t pin ){this->pin_data = pin ;}
       void set_address(uint8_t addr ){this->address = address ;}
     };


    class DigitalSwitch: public switch_::Switch, public Component{
            public:
            DigitalSwitch(){

            }
            void setup() override;
            void write_state(bool state) override;
            void dump_config() override;
            bool pinState ;
    };

  };
};

#endif