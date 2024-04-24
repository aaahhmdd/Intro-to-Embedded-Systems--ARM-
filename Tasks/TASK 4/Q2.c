# include"tm4c123gh6pm.h"
# include"io.h"
#include <cstdint>
# include "string.h" 
# include <stdint.h>  

# define CR 0x0D // "enter"
# define bufferlen  1

                                            // UART part //

void UART0_Int(){
    SYSCTL_RCGCUART_R  |= SYSCTL_RCGCUART_R0 ;  // enable clock of UART
    SYSCTL_RCGCGPIO_R  |= SYSCTL_RCGCUART_R0 ;  // enable clock of GPIO_A
    // no delay required
    UART0_CTL_R &= ~ UART_CTL_UARTEN ; // disable UART
    // set BR = 9600 bits/sec
    UART0_IBRD_R = 104 ;
    UART0_FBRD_R = 11  ;
    UART0_LCRH_R = ( UART_LCRH_WLEN_8 | UART_LCRH_FEN ) ; // enable FIFO (8 bit word)
    UART0_CTL_R  |= (UART_CTL_UARTEN | UART_CTL_RXE | UART_CTL_TXE) ; // enable TX and RX and UART

    // connection with GPIO

    GPIO_PORTA_AFSEL_R |= 0x03 ; // bits 0 , 1 will be enabled in ALT.FN
    GPIO_PORTA_PCTL_R  = (GPIO_PORTA_PCTL_R & ~ 0xff) | ( GPIO_PCTL_PA0_U0RX | GPIO_PCTL_PA1_U0TX ) ; // first clear LS 8 bits then enable ALT.FN in PCTL
    GPIO_PORTA_DEN_R   |= 0x03 ; // enable Digital Function
    GPIO_PORTA_AHB_AMSEL_R &= ~ 0x03 ; // disable Analog Function
}

                                            // GPIO part // 

void portF_Int (){
    SYSCTL_RCGCGPIO_R |= PF_mask ; // port F clock enable
    while ( (SYSCTL_RCGCGPIO_R & PF_mask) == 0 ) ; // wait untill it is enabled
    GPIO_PORTF_AMSEL_R &= ~ PF123_mask ; // disable analog function
    GPIO_PORTF_AFSEL_R &= ~ PF123_mask ; // no ALT.FN
    GPIO_PORTF_PCTL_R &= ~ 0x0000fff0 ; // clear bit PCTL   
    GPIO_PORTF_DIR_R |= PF123_mask ; // enable output dir
    GPIO_PORTF_DEN_R |= PF123_mask ; // enable digital
    GPIO_PORTF_DATA_R &= ~ PF123_mask ; // initialize LEDs to be off
}

                                            // SYS-tick part //

void SysTick_Init(void){
NVIC_ST_CTRL_R = 0; // 1) disable SysTick during setup
NVIC_ST_RELOAD_R = 0x00FFFFFF; // 2) maximum reload value
NVIC_ST_CURRENT_R = 0; // 3) any write to current clears it
NVIC_ST_CTRL_R = 0x00000005; // 4) enable SysTick with core clock 101
}

/*
// The delay parameter is in units of the 80 MHz core clock(12.5 ns)
void SysTick_Wait(uint32_t delay){
    NVIC_ST_RELOAD_R = delay-1; 
    NVIC_ST_CURRENT_R = 0;
    // number of counts
    // any value written to CURRENT clears
    while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for flag
    }
}
*/
                                            // Functions //

void leds_on(uint8_t mask){ // function off a specified led accourding to the mask
    mask &= 0xE ;
    GPIO_PORTF_DATA_R |= mask ;
}

void leds_off(uint8_t mask){ // // function on a specified led accourding to the mask
    mask &= 0xE ;
    GPIO_PORTF_DATA_R &= ~ mask ;
}

/*
uint8_t UART0_Read_AVAILABLE(void){ // function to know if fifo is empty
    return ((UART0_FR_R&UART_FR_RXFE) == UART_FR_RXFE) ? 0 : 1 ; // return true if R.H.S not equal L.H.S
}

uint8_t UART0_Write_AVAILABLE(void){ // function to know if fifo is full
    return ( (UART0_FR_R&UART_FR_TXFE) != 0 ) ? 1 : 0 ; // return true if R.H.S not equal L.H.S
}
*/

char UART0_read(){
    while((UART0_FR_R&0x10)!=0);   //(UART0_Read_AVAILABLE()!=1) ; // stay in this line untill it is available
    return (char) UART0_DR_R ;     //( UART0_DR_R & 0xFF ) ;
}

void UART0_write(char c){
    while((UART0_FR_R&0x20)!=0);   //(UART0_Write_AVAILABLE()!=0) ; // stay in this line untill it is available
    UART0_DR_R = c ; 
}

void get_command(char * command , int len){
    char c ;
    int i ;
    for (i = 0; i < len; i=i+1){
        c=UART0_read();
        if (c != CR){               // check if it is not enter char 
            command[i]=c;
            UART0_write(command[i]);
        }
        else if (c==CR||i==len) break;
    }
}

// The delay parameter is in units of the 12 MHz core clock(83.333 ns)
void Wait(uint32_t delay){
    NVIC_ST_RELOAD_R = delay-1;               // number of counts 
    NVIC_ST_CURRENT_R = 0;                   // any value written to CURRENT clears
    while((NVIC_ST_CTRL_R & NVIC_ST_CTRL_COUNT)==0){  // wait for flag
    }
}
// Call this routine to wait for delay*10ms 
void delay(uint32_t delay){ 
    unsigned long i;
    for(i=0; i<delay; i++){ 
    Wait(120000); // wait 10ms
    }
}
                                                // Main //

int main (){
    char command [bufferlen] = {0} ;
    UART0_Int();
    portF_Int();
    SysTick_Init();

    while (1)
    {
        get_command(command,bufferlen)  ;

        switch (command[0])
        {
        case 'A':
            leds_off(0x04|0x08|0x02) ; // all leds are off
            delay(6000);    // wait 1 min.
            leds_on(0x02);              // turn on red led
            break;

        case 'B':
            leds_off(0x04|0x08|0x02) ; // all leds are off
            delay(3000);    // wait 0.5 min.
            leds_on(0x04);              // blue led is on
            break;

        case 'D':
            leds_off(0x04|0x08|0x02) ;   // all leds are off
            delay(12000);               // wait 2 min.
            leds_on(0x08);             // green led is on
            break;

        default:
            leds_off(0x04|0x08|0x02) ;
            memset(command,0,bufferlen);
            UART0_write('\n');
            break;
        }
    }
}










