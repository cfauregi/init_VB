#include <libopencm3/stm32/rcc.h>
#include <libopencm3/stm32/gpio.h>
#include <libopencm3/stm32/usart.h>
#include <libopencm3/stm32/timer.h>
#include <libopencm3/stm32/flash.h> // definitions du timer
#include <libopencm3/stm32/spi.h>
#include <libopencm3/cm3/nvic.h>
#include <libopencm3/stm32/adc.h>


// APB2 max=84 MHz but when the APB prescaler is NOT 1, the interface clock is fed
// twice the frequency => Sysclk = 140 MHz, APB2=2 but Timers are driven at twice that is 140.
const struct rcc_clock_scale rcc_hse_20mhz_3v3 = {
                .pllm = 20,                           // 20/20=1 MHz
                .plln = 280,                          // 1*280/2=140 MHz
                .pllp = 2,                            //       ^
                .pllq = 6,
		.pllr=0,
		.pll_source = RCC_CFGR_PLLSRC_HSE_CLK, // 190913 mandatory to use external clock (!=HSI)
                .hpre = RCC_CFGR_HPRE_DIV_NONE,
                .ppre1 = RCC_CFGR_PPRE_DIV_4,
                .ppre2 = RCC_CFGR_PPRE_DIV_2,
		//.voltage_scale = PWR_SCALE1,
                .flash_config = FLASH_ACR_ICEN | FLASH_ACR_DCEN |
                                FLASH_ACR_LATENCY_4WS, // 4 WS d'apres configuration par ST
                .ahb_frequency  = 140000000,  
                .apb1_frequency = 35000000,    
                .apb2_frequency = 70000000,
        };

/**
  * @file    system_stm32f4xx.c
  * @author  MCD Application Team
  *=============================================================================
  *        Supported STM32F40xx/41xx/427x/437x devices
  *        System Clock source                    | PLL (HSE)
  *        SYSCLK(Hz)                             | 140000000
  *        HCLK(Hz)                               | 140000000
  *        AHB Prescaler                          | 1
  *        APB1 Prescaler                         | 4
  *        APB2 Prescaler                         | 2
  *        HSE Frequency(Hz)                      | 20000000
  *        PLL_M                                  | 20
  *        PLL_N                                  | 280
  *        PLL_P                                  | 2
  *        PLL_Q                                  | 6
  *        Flash Latency(WS)                      | 4
  *        Require 48MHz for USB OTG FS,          | Disabled
  *        SDIO and RNG clock                     |
*/



void init_gpio(void)
{ /* Setup GPIO for LED. */

	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO12|GPIO15);
	gpio_mode_setup(GPIOD, GPIO_MODE_OUTPUT, GPIO_PUPD_NONE, GPIO13|GPIO14);
}

void clock_setup(void)
{
// for STM32F4-Disco
 	//rcc_clock_setup_hse_3v3(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]); //deprecated 
 	rcc_clock_setup_pll(&rcc_hse_8mhz_3v3[RCC_CLOCK_3V3_168MHZ]); // Mettre en place rcc_clock_setup_pll

// for DDS-based project
//	core_clock_setup();

	/* Enable GPIOD clock for LED & USARTs. */
	rcc_periph_clock_enable(RCC_GPIOD);
	rcc_periph_clock_enable(RCC_GPIOA);


	/* Enable clocks for USART2. */
	rcc_periph_clock_enable(RCC_USART2);
}


void led_set(int msk)
{

gpio_set(GPIOD, GPIO15|GPIO12);
	gpio_set(GPIOD, GPIO13|GPIO14);

}

void led_clr(int msk)
{
	gpio_clear(GPIOD, GPIO15|GPIO12);
	gpio_clear(GPIOD, GPIO13|GPIO14);
}


void adc_setup(void)
{
	uint8_t channel_array[16];

	rcc_periph_clock_enable(RCC_ADC1);
	
	rcc_periph_clock_enable(RCC_GPIOC);
	gpio_mode_setup(GPIOC,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO0);
	gpio_mode_setup(GPIOC,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO1);
	gpio_mode_setup(GPIOC,GPIO_MODE_ANALOG,GPIO_PUPD_NONE,GPIO2);

	adc_power_off(ADC1);
	adc_enable_scan_mode(ADC1);
	adc_set_single_conversion_mode(ADC1);
	adc_set_right_aligned(ADC1);
	adc_set_sample_time_on_all_channels(ADC1,ADC_SMPR_SMP_28CYC);

	//ADC_CCR = ADC_CCR | 0x00000015; // Comon control register  
	adc_power_on(ADC1);

	/* 10=> PC0
	 * 11 => PC1
	 * 12 => PC2
	 */
	channel_array[0] = 11; 
	adc_set_regular_sequence(ADC1, 1 , channel_array);
}
