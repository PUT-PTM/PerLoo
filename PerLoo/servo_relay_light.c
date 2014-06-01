#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "stm32f4xx_adc.h"
#include "misc.h"
#include "servo_relay_light.h"

int period = 1000;
int pres;

uint8_t zmienna = 0;
void servo_relay_light_init(void)
{

	int fcpu = 72000000;
	int hz = 50; //20ms

    pres = (fcpu/(hz*(2*(period))));

    RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM3, ENABLE);
   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOB, ENABLE);
   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
   	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOD, ENABLE);
   	RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1, ENABLE);

	init_leds_relay();

    init_timer_pwm();

	TIM3->CCR3 = SERVO_MAX;

	GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_1;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AN;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	ADC_InitTypeDef ADC_InitStructure;
	ADC_InitStructure.ADC_Resolution = ADC_Resolution_12b;
	ADC_InitStructure.ADC_ScanConvMode = DISABLE;
	ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;
	ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_T1_CC1;
	ADC_InitStructure.ADC_ExternalTrigConvEdge = ADC_ExternalTrigConvEdge_None;
	ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
	ADC_InitStructure.ADC_NbrOfConversion =	1;
	ADC_Init(ADC1, &ADC_InitStructure);
	ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 1, ADC_SampleTime_3Cycles);
	ADC_Cmd(ADC1, ENABLE);
}

void init_timer_pwm(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure;
	TIM_TimeBaseStructure.TIM_Period = period;
	TIM_TimeBaseStructure.TIM_Prescaler = pres;
	TIM_TimeBaseStructure.TIM_ClockDivision = TIM_CKD_DIV1;
	TIM_TimeBaseStructure.TIM_CounterMode = TIM_CounterMode_Up;
	TIM_TimeBaseInit(TIM3, &TIM_TimeBaseStructure);


	TIM_OCInitTypeDef TIM_OCInitStructure;
	TIM_OCInitStructure.TIM_OCMode = TIM_OCMode_PWM1;
	TIM_OCInitStructure.TIM_OutputState = TIM_OutputState_Enable;
	TIM_OCInitStructure.TIM_Pulse = 0;
	TIM_OCInitStructure.TIM_OCPolarity = TIM_OCPolarity_High;
	TIM_OC3Init(TIM3, &TIM_OCInitStructure);
	TIM_OC3PreloadConfig(TIM3, TIM_OCPreload_Enable);

    GPIO_InitTypeDef GPIO_InitStructure;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOB, &GPIO_InitStructure);
	GPIO_PinAFConfig(GPIOB, GPIO_PinSource0, GPIO_AF_TIM3);
	TIM_Cmd(TIM3, ENABLE);
}

void init_leds_relay(void){
	GPIO_InitTypeDef GPIO_InitLeds;
	GPIO_InitLeds.GPIO_Pin = GPIO_Pin_12 |
	GPIO_Pin_13 | GPIO_Pin_14 |	GPIO_Pin_15 |
	//for relay
	GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitLeds.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitLeds.GPIO_OType = GPIO_OType_PP;
	GPIO_InitLeds.GPIO_Speed = GPIO_Speed_100MHz;
	GPIO_InitLeds.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_Init(GPIOD, &GPIO_InitLeds);
}
