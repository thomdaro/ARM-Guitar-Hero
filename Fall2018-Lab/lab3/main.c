#include <stm32f30x.h>  // Pull in include files for F30x standard drivers 
#include <f3d_led.h>     // Pull in include file for the local drivers
#include <f3d_user_btn.h>

// Simple looping delay function

void delay(void) {
  int i = 2000000;
  while (i-- > 0) {
    asm("nop");
  }
}

int main(void) {
  f3d_led_init();
  int counter = 0;
  while(1){
    f3d_led_all_on();
    //    if(counter < 8){
    //  f3d_led_on(counter);
    //  delay();
    //  f3d_led_off(counter);
    //  counter++;}
    //else{
    //  f3d_led_all_on();
    //  delay();
    //  f3d_led_all_off();
    //  counter = 0;
    //}
  }
}

#ifdef USE_FULL_ASSERT
void assert_failed(uint8_t* file, uint32_t line) {
/* Infinite loop */
/* Use GDB to find out why we're here */
  while (1);
}
#endif

/* main.c ends here */
