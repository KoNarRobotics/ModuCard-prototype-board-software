#pragma once
#include "main.hpp"
#include "version.hpp"
#include "stmepic.hpp"
#include "can.hpp"

namespace se = stmepic;
extern se::GpioPin gpio_ch4;
extern se::GpioPin gpio_ch2;
extern se::GpioPin gpio_ch1;
extern se::GpioPin gpio_ch3;
extern se::GpioPin gpio_health_led;
extern float micro_siwert;
extern uint32_t CPM;

void config_usb_device();
void main_prog();

void can_callback_gpio_set(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gpio_status(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_gpio_read(se::CanBase &can, se::CanDataFrame &msg, void *args);

void can_callback_geiger_read(se::CanBase &can, se::CanDataFrame &msg, void *args);
void can_callback_geiger_status(se::CanBase &can, se::CanDataFrame &msg, void *args);
