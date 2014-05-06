#include "stm32f4xx.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_exti.h"
#include "stm32f4xx_rcc.h"
#include "stm32f4xx_syscfg.h"
#include "misc.h"

#define SERVO_MAX 90
#define SERVO_MIN 25

uint8_t need_for_water = 0;
int period = 1000;
int pres;
void EXTI0_IRQHandler ( void ){
	if( EXTI_GetITStatus ( EXTI_Line0 ) != RESET ){
		need_for_water = need_for_water==0?1:0; //zmiana stanu serwa

		GPIO_ToggleBits(GPIOD, GPIO_Pin_5); //zmiana stanu przekaźnika

	    EXTI_ClearITPendingBit ( EXTI_Line0 );
	}
}

void init_timer_pwm(void); // wyjscie PWM na pinie PB0
void init_leds_relay(void); //wyjscia sterujace PD5 i PD6
void init_user_button(void);

uint8_t zmienna = 0;
int main(void)
{

	int fcpu = 72000000;
	int hz = 50; //20ms

    pres = (fcpu/(hz*(2*(period))));


	SystemInit ();
	SystemCoreClockUpdate ();
	RCC_APB1PeriphClockCmd ( RCC_APB1Periph_TIM3 ,	ENABLE );

	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOB ,	ENABLE );
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOA ,	ENABLE );
	RCC_AHB1PeriphClockCmd ( RCC_AHB1Periph_GPIOD ,	ENABLE );

	init_leds_relay();

    init_timer_pwm();

	TIM3 -> CCR3 = SERVO_MAX;

    init_user_button();

	while (1){

        zmienna = 78;
		if(need_for_water == 1){
			TIM3->CCR3 = SERVO_MAX;
		}else{
			TIM3->CCR3 = SERVO_MIN;
		}
	}
}

void init_timer_pwm(void){
	TIM_TimeBaseInitTypeDef TIM_TimeBaseStructure ;
	TIM_TimeBaseStructure . TIM_Period = period;
	TIM_TimeBaseStructure . TIM_Prescaler = pres;
	TIM_TimeBaseStructure . TIM_ClockDivision =
	TIM_CKD_DIV1 ;
	TIM_TimeBaseStructure . TIM_CounterMode =
	TIM_CounterMode_Up ;
	TIM_TimeBaseInit (TIM3 , & TIM_TimeBaseStructure );


	TIM_OCInitTypeDef TIM_OCInitStructure ;
	TIM_OCInitStructure . TIM_OCMode = TIM_OCMode_PWM1 ;
	TIM_OCInitStructure . TIM_OutputState =
	TIM_OutputState_Enable ;
	TIM_OCInitStructure . TIM_Pulse = 0;
	TIM_OCInitStructure . TIM_OCPolarity =
	TIM_OCPolarity_High ;
	TIM_OC3Init (TIM3 , & TIM_OCInitStructure );
	TIM_OC3PreloadConfig (TIM3 , TIM_OCPreload_Enable );



        GPIO_InitTypeDef GPIO_InitStructure ;
	GPIO_InitStructure . GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStructure . GPIO_Mode = GPIO_Mode_AF ;
	GPIO_InitStructure . GPIO_OType = GPIO_OType_PP ;
	GPIO_InitStructure . GPIO_Speed = GPIO_Speed_100MHz	;
	GPIO_InitStructure . GPIO_PuPd = GPIO_PuPd_NOPULL ;
	GPIO_Init (GPIOB , & GPIO_InitStructure );
	GPIO_PinAFConfig (GPIOB , GPIO_PinSource0 ,
	GPIO_AF_TIM3 );
	TIM_Cmd (TIM3 , ENABLE );
}

void init_leds_relay(void){
	GPIO_InitTypeDef GPIO_InitLeds;
	GPIO_InitLeds . GPIO_Pin = GPIO_Pin_12 |
	GPIO_Pin_13 | GPIO_Pin_14 |
	GPIO_Pin_15 |
	//for relay
	GPIO_Pin_5 | GPIO_Pin_6;
	GPIO_InitLeds . GPIO_Mode = GPIO_Mode_OUT ;
	GPIO_InitLeds . GPIO_OType = GPIO_OType_PP ;
	GPIO_InitLeds . GPIO_Speed =
	GPIO_Speed_100MHz ;
	GPIO_InitLeds . GPIO_PuPd =
	GPIO_PuPd_NOPULL ;
	GPIO_Init ( GPIOD , & GPIO_InitLeds );
}

void init_user_button(void){
	GPIO_InitTypeDef GPIO_InitStructure2;
	GPIO_InitStructure2.GPIO_Mode =	GPIO_Mode_IN ;
	GPIO_InitStructure2.GPIO_PuPd =	GPIO_PuPd_NOPULL ;
	GPIO_InitStructure2.GPIO_Pin = GPIO_Pin_1 ;
	GPIO_Init (GPIOA , & GPIO_InitStructure2 );

	NVIC_InitTypeDef NVIC_InitStructure ;
	NVIC_InitStructure.NVIC_IRQChannel = EXTI0_IRQn;
	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x00 ;
	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x00 ;
	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE ;
	NVIC_Init (& NVIC_InitStructure );

	EXTI_InitTypeDef EXTI_InitStructure;
	EXTI_InitStructure.EXTI_Line = EXTI_Line0;
	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt ;
	EXTI_InitStructure.EXTI_Trigger =	EXTI_Trigger_Falling ;
	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
	EXTI_Init(&EXTI_InitStructure);
	SYSCFG_EXTILineConfig (EXTI_PortSourceGPIOA ,
			EXTI_PinSource0);
}
