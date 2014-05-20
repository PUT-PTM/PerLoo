#include "DHT.h"
#include "servo_relay.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
int temperature;
int humidity;

/*
 * DHT DATA temperature - PA5
 * DHT DATA humidity - PA6
 * SERVO DATA - PB0
 * RELAY IN temperature - PD5
 * RELAY IN leds - PD6
 */
int state = 0;
int humidity_counter = 1200;
int main(void)
{
	    //SystemInit ();
	    SystemCoreClockUpdate ();
		DHT_Init();
		int err;
		//czekanie na ustabilizowanie stanu ukladu
	    wait_ms(2000);
	    temperature = 0;
		humidity = 0;

		servo_relay_init();
		//init_user_button();

	    while (1)
	    {
	        err = DHT_readData();
	        if (err == 0) {
	        	temperature = przeliczTemp();
				humidity = przeliczWilgotnosc();
	        }
            wait_ms(3000);
            humidity_counter++;

	        if(humidity_counter >= 3){
	        	TIM3->CCR3 = SERVO_MIN;

	        }

	        if(humidity < 70 && humidity_counter >8){
	        	TIM3->CCR3 = SERVO_MAX;
	        	humidity_counter = 0;
	        }

	        if(temperature <= 28){
	        	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	        } else if(temperature > 30){
	        	GPIO_ResetBits(GPIOD, GPIO_Pin_5);
	        }

	        state = GPIO_ReadInputDataBit(GPIOD, GPIO_Pin_5);
	    }

}
