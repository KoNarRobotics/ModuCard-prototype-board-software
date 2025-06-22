#include "main.hpp"
#include "main_prog.hpp"
#include "Timing.hpp"
#include "simple_task.hpp"
#include "can2.0.hpp"
#include "logger.hpp"
#include "can_messages.h"
#include "uart.hpp"
#include "i2c.hpp"
#include "gpio.hpp"
#include "version.hpp"
#include "stmepic.hpp"

namespace se = stmepic;

std::shared_ptr<se::I2C> i2c1;

std::shared_ptr<se::UART> uart4  = nullptr;
std::shared_ptr<se::CAN> fdcan = nullptr;

// se::GpioPin gpio_i2c1_scl(*GPIOB, GPIO_PIN_8);
// se::GpioPin gpio_i2c1_sda(*GPIOB, GPIO_PIN_9);
// se::GpioPin gpio_boot_enable(*BOOT_EN_GPIO_Port, BOOT_EN_Pin);

se::GpioPin gpio_ch4(*GPIOA, GPIO_PIN_3); // CH 4
se::GpioPin gpio_ch2(*GPIOC, GPIO_PIN_13); // CH 2
se::GpioPin gpio_ch1(*GPIOC, GPIO_PIN_4); // CH 1
se::GpioPin gpio_ch3(*GPIOA, GPIO_PIN_2); // CH 3
se::GpioPin gpio_health_led(*GPIOA, GPIO_PIN_8); // Health LED
se::SimpleTask task_blink;

bool relay_state[4] = {false, false, false, false}; // Relay states for CH1, CH2, CH3, CH4

/**
 * @brief  Period elapsed callback in non blocking mode
 * @note   This function is called  when TIM7 interrupt took place, inside
 * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
 * a global variable "uwTick" used as application time base.
 * @param  htim : TIM handle
 * @retval None
 */
extern "C" {
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
  if(htim->Instance == TIM6) {
    se::Ticker::get_instance().irq_update_ticker();
    HAL_IncTick();
  }
  if(htim->Instance == TIM7) {
    HAL_IncTick();
  }
}

}


se::Status init_board(se::SimpleTask &task, void *pvParameters) {
  // gpio_user_led_1.write(0);
  // some initialization code here

  vTaskDelay(2000); // Wait for 1 second to ensure all GPIOs and devices booted correctly after power on
 
  return se::Status::OK();
}

se::Status task_blink_func(se::SimpleTask &task, void *pvParameters) {
  gpio_health_led.toggle();
  (void)pvParameters;

  return se::Status::OK();
}

void can_callback_change_relay(stmepic::CanBase &can, stmepic::CanDataFrame &recived_msg, void *args) {
  (void)can;
  (void)recived_msg;
  (void)args;
  can_gpio_send_state_t can_gpio_state;
  can_gpio_send_state_unpack(&can_gpio_state, recived_msg.data, recived_msg.data_size);
  gpio_ch1.write(can_gpio_state.ch1);
  gpio_ch2.write(can_gpio_state.ch2);
  gpio_ch3.write(can_gpio_state.ch3);
  gpio_ch4.write(can_gpio_state.ch4);
}


void main_prog() {
  // START ALL INTERRUPTS
  // HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
  // HAL_NVIC_EnableIRQ(TIM6_IRQn);

  HAL_TIM_Base_Start_IT(&htim6);

  CAN_FilterTypeDef can_filter;
  can_filter.FilterBank           = 1;
  can_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
  can_filter.FilterActivation     = CAN_FILTER_ENABLE;
  can_filter.FilterMode           = CAN_FILTERMODE_IDMASK;
  can_filter.FilterScale          = CAN_FILTERSCALE_16BIT;
  can_filter.FilterIdHigh         = 0;
  can_filter.FilterIdLow          = 0;
  can_filter.FilterMaskIdHigh     = 0;
  can_filter.FilterMaskIdLow      = 0;
  can_filter.SlaveStartFilterBank = 0;

  STMEPIC_ASSING_TO_OR_HRESET(fdcan, se::CAN::Make(hcan2, can_filter, nullptr, nullptr));

  fdcan->add_callback(6, can_callback_change_relay);
  // INIT LOGGER
  std::string version = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_BUILD);
  se::Logger::get_instance().init(se::LOG_LEVEL::LOG_LEVEL_DEBUG, true, nullptr, true, version);

  // START MAIN TASK
  task_blink.task_init(task_blink_func, nullptr, 500, init_board, 3500, 2, "MainTask", false);
  task_blink.task_run();
}