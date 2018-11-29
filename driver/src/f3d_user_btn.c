/*
 Ben Staples --> f3d_user_btn.c
 *********************************
 *f3d_user_btn.c 
 *contains the init and read functions for the User Button
 ********************************
*/
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>



/*Initialization of the UserButton*/
void f3d_user_btn_init(void){
  //must enable clock on GPIO port before init.
  RCC_AHBPeriphClockCmd(RCC_AHBPeriph_GPIOA, ENABLE);

  GPIO_InitTypeDef GPIO_InitStructure;

  GPIO_StructInit(&GPIO_InitStructure);
  //input button is on pin 0
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
  //taking input from button
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN;
  GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
  
  GPIO_Init(GPIOA, &GPIO_InitStructure);
}

/*reads the User Button*/
int user_btn_read(void){
  return GPIO_ReadInputDataBit(GPIOA,GPIO_Pin_0);
}
