#include "io.h"

void RGB_LEDs (void) {
    SYSCTL_RCGCGPIO_R |= PF_mask ;
    while ( (SYSCTL_RCGCGPIO_R & PF_mask) == 0 ) ;
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY ;
    GPIO_PORTF_CR_R |= PF123_mask ; // allow changes to pins 1,2,3 for port_f
    GPIO_PORTF_AMSEL_R &= ~ PF123_mask ;
    GPIO_PORTF_AFSEL_R &= ~ PF123_mask ;
    GPIO_PORTF_PCTL_R &= ~ 0x0000fff0 ;
    GPIO_PORTF_DIR_R |= PF123_mask ; // enable output dir
    GPIO_PORTF_DEN_R |= PF123_mask ;
    GPIO_PORTF_DATA_R &= ~ PF123_mask ;
}

void SW_1 (void) {  // intializing switch_1///////////////////
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY ;
    GPIO_PORTF_CR_R |= PFSW1_mask ; // allow changes to pin 4 for port_f
    GPIO_PORTF_AMSEL_R &= ~ PFSW1_mask ;
    GPIO_PORTF_AFSEL_R &= ~ PFSW1_mask ;
    GPIO_PORTF_PCTL_R &= ~ 0x000f0000 ;
    GPIO_PORTF_DIR_R &= ~ PFSW1_mask ; // enable input dir
    GPIO_PORTF_DEN_R |= PFSW1_mask ;
    GPIO_PORTF_PUR_R |=  PFSW1_mask ; // by default 1
}

void SW_2 (void) {  // intializing switch_2/////////////////////
    GPIO_PORTF_LOCK_R = GPIO_LOCK_KEY ;
    GPIO_PORTF_CR_R |= PFSW2_mask ; // allow changes to pin 0 for port_f
    GPIO_PORTF_AMSEL_R &= ~ PFSW2_mask ;
    GPIO_PORTF_AFSEL_R &= ~ PFSW2_mask ;
    GPIO_PORTF_PCTL_R &= ~ 0x000f0000 ;
    GPIO_PORTF_DIR_R &= ~ PFSW2_mask ; // enable input dir
    GPIO_PORTF_DEN_R |= PFSW2_mask ;
    GPIO_PORTF_PUR_R |=  PFSW2_mask ; // by default 1
}

unsigned SW_1_2_INPUT (void){       // fn to return data input
    GPIO_PORTF_DATA_R &= ~ 0x11 ;
    return GPIO_PORTF_DATA_R & 0x11 ; 
}

void RGB_otput (unsigned char data) { // fn to return outputs on leds
    GPIO_PORTF_DATA_R &= ~ PF123_mask ;
    GPIO_PORTF_DATA_R |= data ;
}

unsigned char button_in;
unsigned char led_out = 0x0; // by default leds are off

int main () {

    RGB_LEDs();
    SW_1();
    SW_2();

    while (1) // always on
    {
        button_in = SW_1_2_INPUT() ;

        if ( button_in != 0x010 && button_in == 0x01 ) // if sw1 is pressed and sw2 not
        {
            led_out = 0x04; // blue led is on
        }

        else if ( button_in != 0x01 && button_in == 0x10 ) // if sw2 is pressed and sw1 not
        {
            led_out = 0x08; // green led is on
        }

        else if ( button_in != 0x11 ) // if both are pressed
        {
            led_out = 0x02; // red led is on
        }

        RGB_otput(led_out);
    }
    
}

