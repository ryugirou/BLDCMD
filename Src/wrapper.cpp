/*
 * wrapper.cpp
 *
 *  Created on: May 29, 2019
 *      Author: ryu
 */
#include "stm32f1xx_hal.h"

#include "wrapper.hpp"
#include "LedBlink.hpp"
//#include "timer.hpp"
#include "EXTI.hpp"
#include "Driver.hpp"

void main_cpp(void) {
    Driver driver;

    uint32_t last_time = HAL_GetTick();
    uint32_t interval = 500 / 10;
    uint32_t angle = 0;

    while(1){
    	//HAL_Delay(driver.time());
    	if (HAL_GetTick() - last_time > interval){
    		driver.next();
    		if(angle>=12){
    			if(interval > 5){
    				interval--;
    			}
    			angle=0;
    		}
    		else angle++;
    		last_time = HAL_GetTick();
    	}
    }
}


