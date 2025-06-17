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

se::SimpleTask task_blink;


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
  (void)pvParameters;

  return se::Status::OK();
}


void main_prog() {
  // START ALL INTERRUPTS
  // HAL_NVIC_SetPriority(TIM6_IRQn, 1, 0);
  // HAL_NVIC_EnableIRQ(TIM6_IRQn);

  HAL_TIM_Base_Start_IT(&htim6);

  // INIT LOGGER
  std::string version = std::to_string(VERSION_MAJOR) + "." + std::to_string(VERSION_MINOR) + "." + std::to_string(VERSION_BUILD);
  se::Logger::get_instance().init(se::LOG_LEVEL::LOG_LEVEL_DEBUG, true, nullptr, true, version);


  // START MAIN TASK
  task_blink.task_init(task_blink_func, nullptr, 100, init_board, 3500, 2, "MainTask", false);
  task_blink.task_run();
}