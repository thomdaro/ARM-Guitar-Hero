/* declarations for f3d_user_btn.c */
#include <stm32f30x.h>
#include <stm32f30x_gpio.h>
#include <stm32f30x_rcc.h>

/*Initialization of the UserButton*/
void f3d_user_btn_init(void);
/*reads the User Button*/
int user_btn_read(void);


