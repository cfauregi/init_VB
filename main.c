#include "fonction.h" 
#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/adc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>

int main()
{
int msk;
int b=0,c=0;
volatile int i;
char le_test[80];
le_test[80]='\n';
//char data_freq_min2[80];


msk=(1<<11)|(1<<12)|(1<<13);
clock_setup();
init_gpio();
usart2_setup();
adc_setup();

 for (i=0; i<150000;i++){}
while(1)
  {
	  if (b<1500000)
		  led_set(msk);
	  else 
		  led_clr(msk);
	b++;
	  if (b==3000000)
		  b=0;
    }
	
return 0;
}
