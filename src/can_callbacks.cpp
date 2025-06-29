#include "main.hpp"
#include "main_prog.hpp"
#include "can_messages.h"
#include "stmepic.hpp"
#include "can.hpp"


namespace se = stmepic;

void can_callback_gpio_set(se::CanBase &can, se::CanDataFrame &msg, void *args) {
  (void)can;
  (void)args;
  can_gpio_set_t gpio_set;
  can_gpio_set_unpack(&gpio_set, msg.data, msg.data_size);
  relay_state[0] = gpio_set.ch1;
  relay_state[1] = gpio_set.ch2;
  relay_state[2] = gpio_set.ch3;
  relay_state[3] = gpio_set.ch4;
}

void can_callback_gpio_status(se::CanBase &can, se::CanDataFrame &msg, void *args) {
  (void)can;
  (void)args;
  can_gpio_status_t gpio_status;
  gpio_status.status = CAN_GPIO_STATUS_STATUS_OK_CHOICE;
  se::CanDataFrame response_msg;
  response_msg.frame_id       = CAN_GPIO_STATUS_FRAME_ID;
  response_msg.remote_request = false;
  response_msg.extended_id    = CAN_GPIO_STATUS_IS_EXTENDED;
  response_msg.fdcan_frame    = false;
  response_msg.data_size      = CAN_GPIO_STATUS_LENGTH;
  can_gpio_status_pack(response_msg.data, &gpio_status, response_msg.data_size);
  (void)can.write(response_msg);
}

void can_callback_gpio_read(se::CanBase &can, se::CanDataFrame &msg, void *args) {
  (void)can;
  (void)args;
  can_gpio_read_t gpio_read;
  se::CanDataFrame response_msg;
  response_msg.frame_id       = CAN_GPIO_READ_FRAME_ID;
  response_msg.remote_request = false;
  response_msg.extended_id    = CAN_GPIO_READ_IS_EXTENDED;
  response_msg.fdcan_frame    = false;
  response_msg.data_size      = CAN_GPIO_READ_LENGTH;

  gpio_read.ch1 = relay_state[0];
  gpio_read.ch2 = relay_state[1];
  gpio_read.ch3 = relay_state[2];
  gpio_read.ch4 = relay_state[3];
  can_gpio_read_unpack(&gpio_read, response_msg.data, response_msg.data_size);
  (void)can.write(response_msg);
}

void can_callback_geiger_read(se::CanBase &can, se::CanDataFrame &msg, void *args) {
  (void)can;
  (void)args;
  can_geiger_read_t geiger_read;
  geiger_read.micro_siwert = micro_siwert;
  geiger_read.cpm          = (float)CPM;

  se::CanDataFrame response_msg;
  response_msg.frame_id       = CAN_GEIGER_READ_FRAME_ID;
  response_msg.remote_request = false;
  response_msg.extended_id    = CAN_GEIGER_READ_IS_EXTENDED;
  response_msg.fdcan_frame    = false;
  response_msg.data_size      = CAN_GEIGER_READ_LENGTH;

  can_geiger_read_pack(response_msg.data, &geiger_read, response_msg.data_size);
  (void)can.write(response_msg);
}

void can_callback_geiger_status(se::CanBase &can, se::CanDataFrame &msg, void *args) {
  (void)can;
  (void)args;
  can_geiger_status_t geiger_status;
  geiger_status.geiger_status = current_geiger_status;

  se::CanDataFrame response_msg;
  response_msg.frame_id       = CAN_GEIGER_STATUS_FRAME_ID;
  response_msg.remote_request = false;
  response_msg.extended_id    = CAN_GEIGER_STATUS_IS_EXTENDED;
  response_msg.fdcan_frame    = false;
  response_msg.data_size      = CAN_GEIGER_STATUS_LENGTH;

  can_geiger_status_pack(response_msg.data, &geiger_status, response_msg.data_size);
  (void)can.write(response_msg);
}
