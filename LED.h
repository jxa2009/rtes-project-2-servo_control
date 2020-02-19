#ifndef __STM32L476G_DISCOVERY_LED_H
#define __STM32L476G_DISCOVERY_LED_H

#include "stm32l476xx.h"

void LED_Init(void);
void set_red_led( uint32_t on );
void set_green_led( uint32_t on );
#endif /* __STM32L476G_DISCOVERY_LED_H */
