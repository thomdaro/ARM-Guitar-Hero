/* declarations for f3d_led.c */
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>

//intializes the port and pins for the leds on the board
void f3d_led_init(void);
/*Turns on the appropriate led as specified by the parameter 0 - 7.*/
void f3d_led_on(int led);
/*Turns off the approiate led as specified by the parameter 0 - 7*/ 
void f3d_led_off(int led);
/*Turns on all LEDs*/
void f3d_led_all_on(void);
/*Turns off all LEDs*/
void f3d_led_all_off(void);



