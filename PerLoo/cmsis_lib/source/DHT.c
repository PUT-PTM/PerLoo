#include "DHT.h"
#include "stm32f4xx_gpio.h"
#include "stm32f4xx_rcc.h"
int wynik[40];

void wait_us(volatile int us)
{
    while(us--);
}

void wait_ms(volatile int ms)
{
    wait_us(ms*1000);
}


void DHT11pinIn(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void DHT11pinOut(void){
  GPIO_InitTypeDef GPIO_InitStructure;
  GPIO_InitStructure.GPIO_Pin =  GPIO_Pin_5;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
  GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_UP;
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}
void PinInit(){

	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);
	DHT11pinOut();
}

void DHT_Init(void)
{
    PinInit();
}
int DHT_readData(void)
{
    eError err = ERROR_NONE;
    int retryCount = 0;
    // wait 1s
    wait_ms(1000);
    //zaczynamy transmisje - resetujemy
    DHT11pinOut();
    GPIO_ResetBits(GPIOA, GPIO_Pin_5);
    wait_ms(18);

    //podniesienie napiecia i poczekanie na odpowiedz 40ms
    GPIO_SetBits(GPIOA, GPIO_Pin_5);
    wait_us(40);

    //przelaczenie sie na nasluch
    DHT11pinIn();
    retryCount = 0;
    do {
        if (retryCount > 50)  {
            err = ERROR_NOT_PRESENT;
            return err;
        }
        retryCount++;
        wait_us(1);
    } while ((GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1));

    if (err != ERROR_NONE) {
        return err;
    }
    //sygnal niski - pierwsza odpowiedz
    wait_us(80);

    //sygnal wysoki - druga odpowiedz
    wait_us(80);
    int bitCount = 0;

    //odbierz 40 bitow
    for(bitCount=0;bitCount<40;bitCount++){
    	int proba = 0;

    	//sprawdz, czy jest 50ms preambula  -stan niski
    	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 0){
    		proba++;
    		if(proba>55) return ERROR_DATA_TIMEOUT;;
    		wait_us(1);
    	}
    	proba = 0;
    	//sprawdz jak dlugo trwa stan wysoki - jezeli powyzej 28ms to jest 1, inaczej 0.
    	while(GPIO_ReadInputDataBit(GPIOA, GPIO_Pin_5) == 1){
    		proba++;
    		if(proba>7) wynik[bitCount] = 1;
    		else wynik[bitCount] = 0;
    		wait_us(1);
    	}
    	//powtorz cykl dla kolejnego z 40 bitow.
    }
    return err;

}
int przeliczTemp(){
	int i = 0;
	int mnoznik = 256;
	int output = 0;
	for(i=16;i<24;i++){
		output += wynik[i]*mnoznik;
		mnoznik /= 2;
	}
	return output;
}

int przeliczWilgotnosc(){
	int i = 0;
	int mnoznik = 256;
	int output = 0;
	for(i=0;i<8;i++){
		output += wynik[i]*mnoznik;
		mnoznik /= 2;
	}
	return output;
}