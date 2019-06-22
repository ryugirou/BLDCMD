/*
 * EXTI.hpp
 *
 *  Created on: Jun 3, 2019
 *      Author: ryu
 */

#ifndef EXTI_HPP_
#define EXTI_HPP_

#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"

void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if ( GPIO_Pin == GPIO_PIN_12 ){
    	HAL_GPIO_TogglePin(GPIOC, GPIO_PIN_13);
    }
}




#endif /* EXTI_HPP_ */
