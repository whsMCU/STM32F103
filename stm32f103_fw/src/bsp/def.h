/*
 * def.h
 *
 *  Created on: Dec 6, 2020
 *      Author: baram
 */

#ifndef SRC_COMMON_DEF_H_
#define SRC_COMMON_DEF_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdarg.h>
#include <math.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>

#define _DEF_USB            0
#define _DEF_UART1          1
#define _DEF_UART2          2
#define _DEF_UART3          3
#define _DEF_UART4          4
#define _DEF_UART5          5
#define _DEF_UART6          6

#define _DEF_ADC1             0
#define _DEF_ADC2             1

#define _DEF_I2C1             0
#define _DEF_I2C2             1
#define _DEF_I2C3             2
#define _DEF_I2C4             3

#define _DEF_SPI1             0
#define _DEF_SPI2             1
#define _DEF_SPI3             2
#define _DEF_SPI4             3

#define _DEF_CAN1             0
#define _DEF_CAN2             1
#define _DEF_CAN3             2
#define _DEF_CAN4             3

#define _DEF_TIM1             0
#define _DEF_TIM2             1
#define _DEF_TIM3             2

#define _DEF_LOW              0
#define _DEF_HIGH             1

#define _DEF_INPUT            0
#define _DEF_INPUT_PULLUP     1
#define _DEF_INPUT_PULLDOWN   2
#define _DEF_INPUT_IT_RISING  3
#define _DEF_OUTPUT           4
#define _DEF_OUTPUT_PULLUP    5
#define _DEF_OUTPUT_PULLDOWN  6
#define _DEF_INPUT_AF_PP      7
#define _DEF_INPUT_ANALOG     8

#define CAN_DLC_BYTES_0       0
#define CAN_DLC_BYTES_1       1
#define CAN_DLC_BYTES_2       2
#define CAN_DLC_BYTES_3       3
#define CAN_DLC_BYTES_4       4
#define CAN_DLC_BYTES_5       5
#define CAN_DLC_BYTES_6       6
#define CAN_DLC_BYTES_7       7
#define CAN_DLC_BYTES_8       8

#define CAN_FRAME_CLASSIC     0
#define CAN_FRAME_FD_NO_BRS   1
#define CAN_FRAME_FD_BRS      2


#endif /* SRC_COMMON_DEF_H_ */
