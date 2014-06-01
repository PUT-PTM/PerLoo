#include "DHT.h"
#include "servo_relay_light.h"
#include "stm32f4xx_tim.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_adc.h"

int temperature;
int humidity;
int light;

/*
 * DHT DATA temperature - PA5
 * DHT DATA humidity - PA6
 * SERVO DATA - PB0
 * RELAY IN temperature - PD5
 * RELAY IN leds - PD6
 * LIGHT SENSOR - PA1
 */
int humidity_counter = 1200;
int main(void)
{
	    //SystemInit ();
	    SystemCoreClockUpdate ();
	    //inicjalizacja czujnika temperatury i wilgoci
		DHT_Init();
		int err;
		//czekanie na ustabilizowanie stanu ukladu
	    wait_ms(2000);
	    temperature = 0;
		humidity = 0;

		//inicjalizacja serwomechanizmu, przekaznikow oraz czujnika oswietlenia
		servo_relay_light_init();

	    while (1)
	    {
	    	//pobranie wartosci z czujnika, w przypadku bledu zwracany jest odpowiedni kod
	        err = DHT_readData();
	        if (err == 0) {
	        	//odczytwanie temperatury z uprzedniego odczytu
	        	temperature = przeliczTemp();
	        	//odczytanie wilgotnosci
				humidity = przeliczWilgotnosc();
	        }
            wait_ms(3000);

            //specjalna zmienna, dzieki ktorej regulowany jest interwal analizy wilgotnosci
            humidity_counter++;


	        if(humidity_counter >= 3){
	        	TIM3->CCR3 = SERVO_MIN;

	        }

	        if(humidity < 70 && humidity_counter >8){
	        	TIM3->CCR3 = SERVO_MAX;
	        	humidity_counter = 0;
	        }

	        //jesli temperatura jest nie wieksza niz 28 stopni, to przelaczany jest stan
	        //przekaznika co uruchamia grzalke i nawiew
	        //jesli temperatura wzrasta powyzej 30 stopni to uklad jest wylaczany
	        if(temperature <= 28){
	        	GPIO_SetBits(GPIOD, GPIO_Pin_5);
	        } else if(temperature > 30){
	        	GPIO_ResetBits(GPIOD, GPIO_Pin_5);
	        }

	        //odczytanie wartosci czujnika oswietlenia przy pomocy ADC
	        ADC_SoftwareStartConv(ADC1);
	        while(ADC_GetFlagStatus(ADC1, ADC_FLAG_EOC ) == RESET);
	        light = ADC_GetConversionValue(ADC1);

	        //w przypadku zbyt malego oswietlenia przelaczany jest przekaznik
	        //co uruchamia diody LED
	        //jesli oswietlenie wzrasta do odpowiedniego poziomu, diody LED sa wylaczane
	        if(light < 1000){
	        	GPIO_SetBits(GPIOD, GPIO_Pin_6);
	        } else if(light > 1300){
	        	GPIO_ResetBits(GPIOD, GPIO_Pin_6);
	        }


	    }

}
