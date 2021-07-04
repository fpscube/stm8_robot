/* Simple "Hello World" UART output  */
#include <string.h>
#include <stdint.h>
#include "stm8.h"

/* Simple busy loop delay */
void delay(unsigned long count) {
    while (count--)
        nop();
}

int uart_write(const char *str) {
    char i;
    for(i = 0; i < strlen(str); i++) {
        while(!(UART1_SR & UART_SR_TXE)); // !Transmit data register empty
        UART1_DR = str[i];
    }
    return(i); // Bytes sent
}

void setPWM(int pwm1,int pwm2)
{

    TIM1_CCR3H = (pwm1 >> 8);
    TIM1_CCR3L = (pwm1 & 0xFF);
    TIM1_CCR4H = (pwm2 >> 8);
    TIM1_CCR4L = (pwm2 & 0xFF);
} 




int main(void)
{
    /* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;

    /* GPIO setup */
    // Set pin data direction as output
    PB_DDR |= (1 << 5);
    PC_DDR |= (1 << 3);
    PC_DDR |= (1 << 4);
    // Set pin as "Push-pull"
    PB_CR1 |= (1 << 5);
    PC_CR1 |= (1 << 3);
    PC_CR1 |= (1 << 4);

    // Setup UART1 (TX=D5)
    UART1_CR2 |= UART_CR2_TEN; // Transmitter enable
    // UART1_CR2 |= UART_CR2_REN; // Receiver enable
    UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
    // 9600 baud: UART_DIV = 16000000/9600 ~ 1667 = 0x0683
    UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 0x0683 coded funky way (see ref manual)


    const uint16_t tim1_prescaler = 16;
    TIM1_PSCRH = (tim1_prescaler >> 8);
    TIM1_PSCRL = (tim1_prescaler & 0xFF);

    const uint16_t tim1_auto_reload = 10000; // 10 ms = 100Hz
    TIM1_ARRH = (tim1_auto_reload >> 8);
    TIM1_ARRL = (tim1_auto_reload & 0xFF);

     uint16_t tim1_compare_reg1 = 1000; // 1ms=1000 2ms=2000
     //channel 3 compare PC3
    TIM1_CCR3H = (tim1_compare_reg1 >> 8);
    TIM1_CCR3L = (tim1_compare_reg1 & 0xFF);
     //channel 4 compare PC4
    TIM1_CCR4H = (tim1_compare_reg1 >> 8);
    TIM1_CCR4L = (tim1_compare_reg1 & 0xFF);

    TIM1_CCER2 = 0x11; // Enable compare channel 3 & 4 output
    TIM1_CCMR3 = 0x6 << 0x4; // PWM mode 1 for channel 3
    TIM1_CCMR4 = 0x6 << 0x4; // PWM mode 1 for channel 4

    TIM1_EGR |= 1; // Generate an update event to register new settings
    TIM1_BKR |= 0x1 << 0x7; // Enable TIM1 output channels
    TIM1_CR1 |= 1; // Enable the counter


    //init

    setPWM(2000,1000); // 1 0

    while(1) {
    //    PB_ODR |= (1 << 5);
    //     uart_write("C");
    //     PB_ODR &= ~(1 << 5);
    // setPWM(1500,1000); // 0.5 0.0
    // delay(1000000);
    // PB_ODR |= (1 << 5);
    // setPWM(1500,2000); // 0.5 1.0
    // delay(1000000);
    // PB_ODR &= ~(1 << 5);
    // setPWM(1000,2000); // 0.0 1.0
    // delay(1000000);
    // PB_ODR |= (1 << 5);
    // setPWM(1000,1500); // 0.0 0.5
    // delay(1000000);
    // PB_ODR &= ~(1 << 5);
    // setPWM(2000,1500); // 1.0 0.5
    // delay(1000000);
    // PB_ODR |= (1 << 5);
    // setPWM(2000,1000); // 1.0 0.0
    // delay(1000000);
    // PB_ODR &= ~(1 << 5);

    setPWM(1000,2000); // 0.0 1.0
    delay(1000000);
    PB_ODR |= (1 << 5);
    setPWM(2000,1000); // 1.0 0.0
    delay(1000000);
    PB_ODR &= ~(1 << 5);

    setPWM(1000,1000); // 0.0 1.0
    delay(1000000);
    PB_ODR |= (1 << 5);

        // LED on
        // LED off
    }
}
