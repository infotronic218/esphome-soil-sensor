import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import pins
from esphome import components as cp
from esphome.components import sensor, text_sensor, number,switch,   gpio, i2c
from esphome.components import web_server_base
from esphome.components.web_server_base import CONF_WEB_SERVER_BASE_ID
from esphome.core import CORE, coroutine_with_priority
from esphome import automation
from esphome.const import (
    CONF_ID,
    CONF_INITIAL_VALUE,
    CONF_LAMBDA,
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_OPTIMISTIC,
    CONF_MODE,
    CONF_RESTORE_VALUE,
    CONF_INITIAL_VALUE,
    CONF_STEP,
    UNIT_CELSIUS,
    DEVICE_CLASS_EMPTY,
    STATE_CLASS_MEASUREMENT
)
CONF_PIN_DATA = "pin_data"
CONF_SCAN_MODE="scan_mode"
CONF_ADDRESS="address"
CONF_TEMPERATURE = "temperature"
CONF_VWC = "vwc"
CONF_EC =  "ec"

MULTI_CONF = True
print(cp)

DEPENDENCIES = ['esp32']
AUTO_LOAD = ["number", "template", "sensor", "switch"]

soil_sensor_teros12__ns = cg.esphome_ns.namespace("soil_sensor_teros12_")
SOIL_SENSOR_TEROS12 = soil_sensor_teros12__ns.class_("Soil_Sensor_Teros12", cg.Component)
DIGITAL_SWITCH = soil_sensor_teros12__ns.class_("DigitalSwitch", switch.Switch, cg.Component)

CONFIG_SCHEMA = cv.Schema({
      cv.GenerateID(): cv.declare_id(SOIL_SENSOR_TEROS12),
      cv.Required(CONF_PIN_DATA):int,
      cv.Optional(CONF_ADDRESS):int,
      cv.Optional(CONF_SCAN_MODE):switch.SWITCH_SCHEMA.extend({
            cv.GenerateID(): cv.declare_id(DIGITAL_SWITCH)
       }),

      cv.Optional(CONF_TEMPERATURE):sensor.sensor_schema(
                unit_of_measurement=UNIT_CELSIUS,
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_EMPTY,
                state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_EC):sensor.sensor_schema(
                unit_of_measurement="",
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_EMPTY,
                state_class=STATE_CLASS_MEASUREMENT,
        ),
        cv.Optional(CONF_VWC):sensor.sensor_schema(
                unit_of_measurement="",
                accuracy_decimals=2,
                device_class=DEVICE_CLASS_EMPTY,
                state_class=STATE_CLASS_MEASUREMENT,
        )

    }).extend(cv.polling_component_schema("60s"))


#@coroutine_with_priority(45.0)
async def to_code(config):
    var =  cg.new_Pvariable(config[CONF_ID])
    await cg.register_component(var, config)
    #await cg.register_component(var, config)
    
    if CONF_PIN_DATA in config:
        pin = config[CONF_PIN_DATA]
        cg.add(var.set_pin_data(pin))
    
    if CONF_ADDRESS in config:
        addr = config[CONF_ADDRESS]
        cg.add(var.set_address( addr))

    if  CONF_SCAN_MODE in config:
        conf = config[CONF_SCAN_MODE]
        my_switch = await switch.new_switch(conf)
        cg.add(var.set_scan_mode_switch(my_switch))


    if CONF_TEMPERATURE in config:
        conf = config[CONF_TEMPERATURE]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_temperature_sensor(sens))

    if CONF_EC in config:
        conf = config[CONF_EC]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_ec_sensor(sens))

    if CONF_VWC in config:
        conf = config[CONF_VWC]
        sens = await sensor.new_sensor(conf)
        cg.add(var.set_vwc_sensor(sens))
