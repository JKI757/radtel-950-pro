/* add user code begin Header */
/**
  **************************************************************************
  * @file     wk_usart.c
  * @brief    work bench config program
  **************************************************************************
  *                       Copyright notice & Disclaimer
  *
  * The software Board Support Package (BSP) that is made available to
  * download from Artery official website is the copyrighted work of Artery.
  * Artery authorizes customers to use, copy, and distribute the BSP
  * software and its related documentation for the purpose of design and
  * development in conjunction with Artery microcontrollers. Use of the
  * software is governed by this copyright notice and the following disclaimer.
  *
  * THIS SOFTWARE IS PROVIDED ON "AS IS" BASIS WITHOUT WARRANTIES,
  * GUARANTEES OR REPRESENTATIONS OF ANY KIND. ARTERY EXPRESSLY DISCLAIMS,
  * TO THE FULLEST EXTENT PERMITTED BY LAW, ALL EXPRESS, IMPLIED OR
  * STATUTORY OR OTHER WARRANTIES, GUARANTEES OR REPRESENTATIONS,
  * INCLUDING BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY,
  * FITNESS FOR A PARTICULAR PURPOSE, OR NON-INFRINGEMENT.
  *
  **************************************************************************
  */
/* add user code end Header */

/* Includes ------------------------------------------------------------------*/
#include "wk_usart.h"

/* add user code begin 0 */

/* add user code end 0 */

/**
  * @brief  init usart1 function
  * @param  none
  * @retval none
  */
void wk_usart1_init(void)
{
  /* add user code begin usart1_init 0 */

  /* add user code end usart1_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart1_init 1 */

  /* add user code end usart1_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_10;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure param */
  usart_init(USART1, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART1, TRUE);
  usart_receiver_enable(USART1, TRUE);
  usart_parity_selection_config(USART1, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART1, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart1_init 2 */

  /* add user code end usart1_init 2 */

  usart_enable(USART1, TRUE);

  /* add user code begin usart1_init 3 */

  /* add user code end usart1_init 3 */
}

/**
  * @brief  init usart2 function
  * @param  none
  * @retval none
  */
void wk_usart2_init(void)
{
  /* add user code begin usart2_init 0 */

  /* add user code end usart2_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart2_init 1 */

  /* add user code end usart2_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_2;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_3;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure param */
  usart_init(USART2, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART2, TRUE);
  usart_receiver_enable(USART2, TRUE);
  usart_parity_selection_config(USART2, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART2, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart2_init 2 */

  /* add user code end usart2_init 2 */

  usart_enable(USART2, TRUE);

  /* add user code begin usart2_init 3 */

  /* add user code end usart2_init 3 */
}

/**
  * @brief  init usart3 function
  * @param  none
  * @retval none
  */
void wk_usart3_init(void)
{
  /* add user code begin usart3_init 0 */

  /* add user code end usart3_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart3_init 1 */

  /* add user code end usart3_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOD, &gpio_init_struct);

  gpio_pin_remap_config(USART3_GMUX_0011, TRUE);

  /* configure param */
  usart_init(USART3, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART3, TRUE);
  usart_receiver_enable(USART3, TRUE);
  usart_parity_selection_config(USART3, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART3, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart3_init 2 */

  /* add user code end usart3_init 2 */

  usart_enable(USART3, TRUE);

  /* add user code begin usart3_init 3 */

  /* add user code end usart3_init 3 */
}

/**
  * @brief  init uart4 function
  * @param  none
  * @retval none
  */
void wk_uart4_init(void)
{
  /* add user code begin uart4_init 0 */

  /* add user code end uart4_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin uart4_init 1 */

  /* add user code end uart4_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOA, &gpio_init_struct);

  gpio_pin_remap_config(UART4_GMUX_0010, TRUE);

  /* configure param */
  usart_init(UART4, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(UART4, TRUE);
  usart_receiver_enable(UART4, TRUE);
  usart_parity_selection_config(UART4, USART_PARITY_NONE);

  usart_hardware_flow_control_set(UART4, USART_HARDWARE_FLOW_NONE);

  /* add user code begin uart4_init 2 */

  /* add user code end uart4_init 2 */

  usart_enable(UART4, TRUE);

  /* add user code begin uart4_init 3 */

  /* add user code end uart4_init 3 */
}

/**
  * @brief  init uart5 function
  * @param  none
  * @retval none
  */
void wk_uart5_init(void)
{
  /* add user code begin uart5_init 0 */

  /* add user code end uart5_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin uart5_init 1 */

  /* add user code end uart5_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_9;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOB, &gpio_init_struct);

  gpio_pin_remap_config(UART5_GMUX_0001, TRUE);

  /* configure param */
  usart_init(UART5, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(UART5, TRUE);
  usart_receiver_enable(UART5, TRUE);
  usart_parity_selection_config(UART5, USART_PARITY_NONE);

  usart_hardware_flow_control_set(UART5, USART_HARDWARE_FLOW_NONE);

  /* add user code begin uart5_init 2 */

  /* add user code end uart5_init 2 */

  usart_enable(UART5, TRUE);

  /* add user code begin uart5_init 3 */

  /* add user code end uart5_init 3 */
}

/**
  * @brief  init usart6 function
  * @param  none
  * @retval none
  */
void wk_usart6_init(void)
{
  /* add user code begin usart6_init 0 */

  /* add user code end usart6_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin usart6_init 1 */

  /* add user code end usart6_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_6;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOC, &gpio_init_struct);

  /* configure param */
  usart_init(USART6, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(USART6, TRUE);
  usart_receiver_enable(USART6, TRUE);
  usart_parity_selection_config(USART6, USART_PARITY_NONE);

  usart_hardware_flow_control_set(USART6, USART_HARDWARE_FLOW_NONE);

  /* add user code begin usart6_init 2 */

  /* add user code end usart6_init 2 */

  usart_enable(USART6, TRUE);

  /* add user code begin usart6_init 3 */

  /* add user code end usart6_init 3 */
}

/**
  * @brief  init uart7 function
  * @param  none
  * @retval none
  */
void wk_uart7_init(void)
{
  /* add user code begin uart7_init 0 */

  /* add user code end uart7_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin uart7_init 1 */

  /* add user code end uart7_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_8;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOE, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_7;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOE, &gpio_init_struct);

  /* configure param */
  usart_init(UART7, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(UART7, TRUE);
  usart_receiver_enable(UART7, TRUE);
  usart_parity_selection_config(UART7, USART_PARITY_NONE);

  usart_hardware_flow_control_set(UART7, USART_HARDWARE_FLOW_NONE);

  /* add user code begin uart7_init 2 */

  /* add user code end uart7_init 2 */

  usart_enable(UART7, TRUE);

  /* add user code begin uart7_init 3 */

  /* add user code end uart7_init 3 */
}

/**
  * @brief  init uart8 function
  * @param  none
  * @retval none
  */
void wk_uart8_init(void)
{
  /* add user code begin uart8_init 0 */

  /* add user code end uart8_init 0 */

  gpio_init_type gpio_init_struct;
  gpio_default_para_init(&gpio_init_struct);

  /* add user code begin uart8_init 1 */

  /* add user code end uart8_init 1 */

  /* configure the TX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_MUX;
  gpio_init_struct.gpio_pins = GPIO_PINS_1;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOE, &gpio_init_struct);

  /* configure the RX pin */
  gpio_init_struct.gpio_drive_strength = GPIO_DRIVE_STRENGTH_MODERATE;
  gpio_init_struct.gpio_out_type  = GPIO_OUTPUT_PUSH_PULL;
  gpio_init_struct.gpio_mode = GPIO_MODE_INPUT;
  gpio_init_struct.gpio_pins = GPIO_PINS_0;
  gpio_init_struct.gpio_pull = GPIO_PULL_NONE;
  gpio_init(GPIOE, &gpio_init_struct);

  /* configure param */
  usart_init(UART8, 115200, USART_DATA_8BITS, USART_STOP_1_BIT);
  usart_transmitter_enable(UART8, TRUE);
  usart_receiver_enable(UART8, TRUE);
  usart_parity_selection_config(UART8, USART_PARITY_NONE);

  usart_hardware_flow_control_set(UART8, USART_HARDWARE_FLOW_NONE);

  /* add user code begin uart8_init 2 */

  /* add user code end uart8_init 2 */

  usart_enable(UART8, TRUE);

  /* add user code begin uart8_init 3 */

  /* add user code end uart8_init 3 */
}

/* add user code begin 1 */

/* add user code end 1 */
