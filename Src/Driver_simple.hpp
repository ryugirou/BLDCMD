/*
 * Driver_simple.hpp
 *
 *  Created on: Jun 23, 2019
 *      Author: ryu
 */

#ifndef DRIVER_SIMPLE_HPP_
#define DRIVER_SIMPLE_HPP_


#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"

enum class Current { UtoW,VtoW,VtoU,WtoU,WtoV,UtoV};

class Driver{
private:
	Current step;
	int high;
	int low;
	int t;
	int angle;
public:
	Driver();
	void stop();
	void next();
	int time();
};

Driver::Driver(){
	step = Current::UtoV;
	t = 50;
	angle = 0;
}

void Driver::stop(){
	HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
	HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
	TIM3->CR1 &= ~(TIM_CR1_CEN);
	TIM4->CR1 &= ~(TIM_CR1_CEN);
	TIM3->CCR1 = 0;
	TIM4->CCR1 = 0;
	TIM4->CCR2 = 0;
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
}

void Driver::next(){//TIM3 CH1:PA6④ TIM4 CH1:PB6⑥ CH2:PB7⑤ 1:PA7,2:PB0,3:PB1,4:PA6,5:PB7,6:PB6
	switch(step){
	case Current::UtoV:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_SET);
		step = Current::UtoW;//1&6
		break;
	case Current::UtoW:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		step = Current::VtoW;//2&6
		break;
	case Current::VtoW:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_SET);
		step = Current::VtoU;//2&4
		break;
	case Current::VtoU:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		step = Current::WtoU;//3&4
		break;
	case Current::WtoU:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_6, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_7, GPIO_PIN_SET);
		step = Current::WtoV;//3&5
		break;
	case Current::WtoV:
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		step = Current::UtoV;//1&5
		break;
	}
}

int Driver::time(){
	if(angle>=12){
		if(t > 4){
			t--;
		}
		angle=0;
	}
	angle++;
	return t;
}




#endif /* DRIVER_SIMPLE_HPP_ */
