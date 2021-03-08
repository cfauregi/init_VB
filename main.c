#include "fonction.h"

int main()
{
int msk;
int b=0;

msk=(1<<11)|(1<<12)|(1<<13);
clock_setup();
init_gpio();
adc_setup();

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

return 0 ;
}
