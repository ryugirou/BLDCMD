/*
 * timer.hpp
 *
 *  Created on: May 30, 2019
 *      Author: ryu
 */

#ifndef TIMER_HPP_
#define TIMER_HPP_

#include "stm32f1xx_hal.h"
#include "stm32f103xb.h"

enum class Current { UtoW,VtoW,VtoU,WtoU,WtoV,UtoV};

class Timer{
private:
	Current step;
	int high;
	int low;
public:
	Timer();
	void gradual();
	void next();
};

Timer::Timer(){
	high = 950;
	low = 50;
	step = Current::UtoV;
	// Start TIM3 (72MHz / 4 /1000 = 18KHz PWM) channel: CH3 (PB0)

	// Initialize TIM3 before timer configs
	TIM3->CR1 &= ~(TIM_CR1_CEN);                // Disable TIM3 counter (temporary)
	RCC->APB1RSTR |=  (RCC_APB1RSTR_TIM3RST);   // Reset TIM3 (Software reset)
	RCC->APB1RSTR &= ~(RCC_APB1RSTR_TIM3RST);   // Reset TIM3 (Software reset)

	// Timer configs
	RCC->APB1ENR |= RCC_APB1ENR_TIM3EN;         // Enable TIM3 clock
	TIM3->EGR  |= TIM_EGR_UG;                   // Update Generation
	TIM3->CR1 |= TIM_CR1_ARPE;                  // Enable TIM3 ARR auto-reload

	TIM3->PSC = 4 - 1;            // Set TIM3 prescaler
	TIM3->ARR = 1000 - 1;         // Set TIM3 auto-reload value

	// Timer configs depend on Channel No. (CH1)
	TIM3->CCR1 = 450;			                          // Set TIM3 CH4 pulse width
	TIM3->CCMR1 |= TIM_CCMR1_OC1M_1 + TIM_CCMR1_OC1M_2; // PWM mode 1
	TIM3->CCMR1 |= TIM_CCMR1_OC1PE;                     // Enable output compare 3 preload
	TIM3->CCER |= TIM_CCER_CC1E;                        // Enable TIM3 CH3 positive output

	// Timer configs depend on Channel No. (CH2)
	TIM3->CCR2 = 450;			                          // Set TIM3 CH4 pulse width
	TIM3->CCMR1 |= TIM_CCMR1_OC2M_1 + TIM_CCMR1_OC2M_2; // PWM mode 1
	TIM3->CCMR1 |= TIM_CCMR1_OC2PE;                     // Enable output compare 3 preload
	TIM3->CCER |= TIM_CCER_CC2E;                        // Enable TIM3 CH3 positive output

	// Timer configs depend on Channel No. (CH3)
	TIM3->CCR3 = 900;                                   // Set TIM3 CH3 pulse width
	TIM3->CCMR2 |= TIM_CCMR2_OC3M_1 + TIM_CCMR2_OC3M_2; // PWM mode 1
	TIM3->CCMR2 |= TIM_CCMR2_OC3PE;                     // Enable output compare 3 preload
	TIM3->CCER |= TIM_CCER_CC3E;                        // Enable TIM3 CH3 positive output
	//TIM3->CCER &= ~(TIM_CCER_CC3NE);                    // Disable TIM3 CH3 negative Output

	// Timer configs depend on Channel No. (CH4)
	TIM3->CCR4 = 450;			                          // Set TIM3 CH4 pulse width
	TIM3->CCMR2 |= TIM_CCMR2_OC4M_1 + TIM_CCMR2_OC4M_2; // PWM mode 1
	TIM3->CCMR2 |= TIM_CCMR2_OC4PE;                     // Enable output compare 3 preload
	TIM3->CCER |= TIM_CCER_CC4E;                        // Enable TIM3 CH3 positive output

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

void Timer::gradual(){
	  for(int i=0;i<1000;i++){
		  TIM3->CR1 &= ~(TIM_CR1_CEN);
		  TIM4->CR1 &= ~(TIM_CR1_CEN);
		  TIM3->CCR1 = i;
		  TIM3->CCR2 = i;
		  TIM3->CCR3 = i;
		  TIM3->CCR4 = i;
		  TIM4->CCR1 = i;
		  TIM4->CCR2 = i;
		  TIM3->CR1 |= TIM_CR1_CEN;
		  TIM4->CR1 |= TIM_CR1_CEN;
		  HAL_Delay(1);
	  }
}

void Timer::next(){//TIM3 CH1:PA6④ CH2:PA7① CH3:PB0② CH4:PB1③ TIM4 CH1:PB6⑥ CH2:PB7⑤ 1:PA7,2:PB0,3:PB1,4:PA6,5:PB7,6:PB6
	TIM3->CR1 &= ~(TIM_CR1_CEN);
	TIM4->CR1 &= ~(TIM_CR1_CEN);
	switch(step){
	case Current::UtoV:
		TIM3->CCR1 = low;
		TIM3->CCR2 = high;
		TIM3->CCR3 = low;
		TIM3->CCR4 = low;
		TIM4->CCR1 = high;
		TIM4->CCR2 = low;
		step = Current::UtoW;//1&6
		break;
	case Current::UtoW:
		TIM3->CCR1 = low;
		TIM3->CCR2 = 0;
		TIM3->CCR3 = 999;
		TIM3->CCR4 = 0;
		TIM4->CCR1 = high;
		TIM4->CCR2 = low;
		step = Current::VtoW;//2&6
		break;
	case Current::VtoW:
		TIM3->CCR1 = high;
		TIM3->CCR2 = 0;
		TIM3->CCR3 = 999;
		TIM3->CCR4 = 0;
		TIM4->CCR1 = low;
		TIM4->CCR2 = low;
		step = Current::VtoU;//2&4
		break;
	case Current::VtoU:
		TIM3->CCR1 = high;
		TIM3->CCR2 = 0;
		TIM3->CCR3 = 0;
		TIM3->CCR4 = 999;
		TIM4->CCR1 = low;
		TIM4->CCR2 = low;
		step = Current::WtoU;//3&4
		break;
	case Current::WtoU:
		TIM3->CCR1 = low;
		TIM3->CCR2 = 0;
		TIM3->CCR3 = 0;
		TIM3->CCR4 = 999;
		TIM4->CCR1 = low;
		TIM4->CCR2 = high;
		step = Current::WtoV;//3&5
		break;
	case Current::WtoV:
		TIM3->CCR1 = low;
		TIM3->CCR2 = 999;
		TIM3->CCR3 = 0;
		TIM3->CCR4 = 0;
		TIM4->CCR1 = low;
		TIM4->CCR2 = high;
		step = Current::UtoV;//1&5
		break;
	}
	TIM3->CR1 |= TIM_CR1_CEN;
	TIM4->CR1 |= TIM_CR1_CEN;
}

#endif /* TIMER_HPP_ */
