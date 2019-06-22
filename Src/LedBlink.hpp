/*
 * LedBlink.hpp
 *
 *  Created on: May 29, 2019
 *      Author: ryu
 */

#ifndef LEDBLINK_HPP_
#define LEDBLINK_HPP_

#include "stm32f1xx_hal.h"

class LedBlink {
public:
    void toggle();
};

void LedBlink::toggle() {
    HAL_GPIO_TogglePin(GPIOA, GPIO_PIN_6);
    HAL_Delay(100);
}

#endif /* LEDBLINK_HPP_ */
