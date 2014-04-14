#include "DHT.h"
int temperature;

int main(void)
{
		DHT_Init();
		int err;
		//czekanie na ustabilizowanie stanu ukladu
	    wait_ms(2000);
	    temperature = 0;
	    while (1)
	    {
	        err = DHT_readData();
	        if (err == 0) {
	        	temperature = przeliczTemp();
	        }

	        wait_ms(3000);
	    }

}
