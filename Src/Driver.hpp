/*
 * Driver.hpp
 *
 *  Created on: Jun 3, 2019
 *      Author: ryu
 */

#ifndef DRIVER_HPP_
#define DRIVER_HPP_

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
	high = 994;
	low = 5;
	step = Current::UtoV;
	t = 50;
	angle = 0;
	// Start TIM3 (72MHz / 4 /1000 = 18KHz PWM) channel: CH3 (PB0)

	// Initialize TIM3 before timer configs
	TIM3->CR1 &= ~(TIM_CR1_CEN);                // Disable TIM3 counter (temporary)
	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM3RST);   // Reset TIM3 (Software reset)
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM3RST);   // Reset TIM3 (Software reset)

	// Timer configs
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;         // Enable TIM3 clock
	TIM3->EGR  |= TIM_EGR_UG;                   // Update Generation
	TIM3->CR1 |= TIM_CR1_ARPE;                  // Enable TIM3 ARR auto-reload

	TIM3->PSC = 20 - 1;            // Set TIM3 prescaler
	TIM3->ARR = 1000 - 1;         // Set TIM3 auto-reload value

	// Timer configs depend on Channel No. (CH1)
	TIM3->CCR1 = 450;			                          // Set TIM3 CH4 pulse width
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 + TIM_CCMR1_OC1M_2; // PWM mode 1
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;                     // Enable output compare 3 preload
	TIM3->CCER |= TIM_CCER_CC1E;                        // Enable TIM3 CH3 positive output

	// Enable TIM3 Counter
	TIM3->CR1 |= TIM_CR1_CEN;
	// Finished to start TIM3 (40KHz PWM)

	// Start TIM4 (72MHz / 4 /1000 = 18KHz PWM) channel: CH3 (PB0)

	// Initialize TIM4 before timer configs
	TIM4->CR1 &= ~(TIM_CR1_CEN);                // Disable TIM3 counter (temporary)
	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM4RST);   // Reset TIM3 (Software reset)
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM4RST);   // Reset TIM3 (Software reset)

	// Timer configs
	RCC->APB1ENR |= RCC_APB1ENR_TIM4EN;         // Enable TIM3 clock
	TIM4->EGR  |= TIM_EGR_UG;                   // Update Generation
	TIM4->CR1 |= TIM_CR1_ARPE;                  // Enable TIM3 ARR auto-reload

	TIM4->PSC = 4 - 1;            // Set TIM3 prescaler
	TIM4->ARR = 1000 - 1;         // Set TIM3 auto-reload value

	// Timer configs depend on Channel No. (CH1)
	TIM4->CCR1 = 450;			                          // Set TIM3 CH4 pulse width
	TIM4->CCMR1 |= TIM_CCMR1_OC1M_1 + TIM_CCMR1_OC1M_2; // PWM mode 1
	TIM4->CCMR1 |= TIM_CCMR1_OC1PE;                     // Enable output compare 3 preload
	TIM4->CCER |= TIM_CCER_CC1E;                        // Enable TIM3 CH3 positive output

	// Timer configs depend on Channel No. (CH2)
	TIM4->CCR2 = 450;			                          // Set TIM3 CH4 pulse width
	TIM4->CCMR1 |= TIM_CCMR1_OC2M_1 + TIM_CCMR1_OC2M_2; // PWM mode 1
	TIM4->CCMR1 |= TIM_CCMR1_OC2PE;                     // Enable output compare 3 preload
	TIM4->CCER |= TIM_CCER_CC2E;                        // Enable TIM3 CH3 positive output

	// Enable TIM4 Counter
	TIM4->CR1 |= TIM_CR1_CEN;
	// Finished to start TIM4 (40KHz PWM)
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
	TIM3->CR1 &= ~(TIM_CR1_CEN);
	TIM4->CR1 &= ~(TIM_CR1_CEN);
	switch(step){
	case Current::UtoV:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		TIM3->CCR1 = low;
		TIM4->CCR2 = low;
		TIM4->CCR1 = high;
		step = Current::UtoW;//1&6
		break;
	case Current::UtoW:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		TIM3->CCR1 = low;
		TIM4->CCR2 = low;
		TIM4->CCR1 = high;
		step = Current::VtoW;//2&6
		break;
	case Current::VtoW:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		TIM3->CCR1 = high;
		TIM4->CCR2 = low;
		TIM4->CCR1 = low;
		step = Current::VtoU;//2&4
		break;
	case Current::VtoU:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		TIM3->CCR1 = high;
		TIM4->CCR2 = low;
		TIM4->CCR1 = low;
		step = Current::WtoU;//3&4
		break;
	case Current::WtoU:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_SET);
		TIM3->CCR1 = low;
		TIM4->CCR2 = high;
		TIM4->CCR1 = low;
		step = Current::WtoV;//3&5
		break;
	case Current::WtoV:
		HAL_GPIO_WritePin(GPIOA, GPIO_PIN_7, GPIO_PIN_SET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_0, GPIO_PIN_RESET);
		HAL_GPIO_WritePin(GPIOB, GPIO_PIN_1, GPIO_PIN_RESET);
		TIM3->CCR1 = low;
		TIM4->CCR2 = high;
		TIM4->CCR1 = low;
		step = Current::UtoV;//1&5
		break;
	}
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
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

#endif /* DRIVER_HPP_ */
