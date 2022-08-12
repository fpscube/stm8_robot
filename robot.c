/* Simple "Hello World" UART output  */
#include <string.h>
#include <stdint.h>
#include "stm8.h"
#include "robotUtils.h"



void setPWM1(int val) // C6
{
    TIM1_CCR1H = (val >> 8);
    TIM1_CCR1L = (val & 0xFF);
} 

void setPWM2(int val) // C7
{
    TIM1_CCR2H = (val >> 8);
    TIM1_CCR2L = (val & 0xFF);
} 

void setPWM3(int val) // C3
{
    TIM1_CCR3H = (val >> 8);
    TIM1_CCR3L = (val & 0xFF);
} 

void setPWM4(int val) // C4
{
    TIM1_CCR4H = (val >> 8);
    TIM1_CCR4L = (val & 0xFF);
} 

void setPWM5(int val) // D4
{
    TIM2_CCR1H = (val >> 8);
    TIM2_CCR1L = (val & 0xFF);
} 

void setPWM6(int val) // D3
{
    TIM2_CCR2H = (val >> 8);
    TIM2_CCR2L = (val & 0xFF);
} 

void setPWM7(int val) // D2
{
    TIM2_CCR3H = (val >> 8);
    TIM2_CCR3L = (val & 0xFF);
} 


int uart_write(const char *str) {
    char i;
    for(i = 0; i < strlen(str); i++) {
        while(!(UART1_SR & UART_SR_TXE)); // !Transmit data register empty
        UART1_DR = str[i];
    }
    return(i); // Bytes sent
}

void uart_writeChar(char c) {
    static char cBuffer[32];
    static char cBufferWrPtr=0;
    static char cBufferRdPtr=0;
    
    cBuffer[cBufferWrPtr] = c;
    cBufferWrPtr = (cBufferWrPtr + 1) % 32;

    if(UART1_SR & UART_SR_TXE) // Transmit data register empty
    {
        if(cBufferWrPtr!=cBufferRdPtr)
        {
            UART1_DR = cBuffer[cBufferRdPtr];
            cBufferRdPtr  = (cBufferRdPtr + 1) % 32; 
        }
    } 
    
}

int uart_readChar(char *c) {
  
    if(UART1_SR & UART_SR_RXNE) // Receive data register empty
    {
        *c = UART1_DR;
        return(1);
    }
    else
    {
        return(0);
    }
}


static int gUsCounter=0;

/* TIM2 Update/Overflow interrupt handling routine */
void TIM2_update(void) __interrupt(TIM2_OVR_UIF_IRQ) {
    gUsCounter+=10;
    // Clear Timer 2 Status Register 1 Update Interrupt Flag (UIF) (bit 0)
    TIM2_SR1 &= ~TIM_SR1_UIF;
}

int main(void)
{
    /* Set clock to full speed (16 Mhz) */
    CLK_CKDIVR = 0;

    /* GPIO setup */
    // Set pin data direction as output
    // led
    PB_DDR |= (1 << 5);
    // pwm
    PC_DDR |= (1 << 3);
    PC_DDR |= (1 << 4);
    PC_DDR |= (1 << 6);
    PC_DDR |= (1 << 7);
    PD_DDR |= (1 << 2);
    PD_DDR |= (1 << 3);
    PD_DDR |= (1 << 4);
    // Set pin as "Push-pull"
    // led
    PB_CR1 |= (1 << 5);
    // pwm
    PC_CR1 |= (1 << 3);
    PC_CR1 |= (1 << 4);
    PC_CR1 |= (1 << 6);
    PC_CR1 |= (1 << 7);
    PD_CR1 |= (1 << 2);
    PD_CR1 |= (1 << 3);
    PD_CR1 |= (1 << 4);

    /* timer 1  init for PWM*/ 
    {

        // processor is working at 16Mhz => time clock is 1Mhz => 1us
        const uint16_t tim1_prescaler = 16;
        TIM1_PSCRH = (tim1_prescaler >> 8);
        TIM1_PSCRL = (tim1_prescaler & 0xFF);

        const uint16_t tim1_auto_reload = 10000; // 10 ms = 100Hz
        TIM1_ARRH = (tim1_auto_reload >> 8);
        TIM1_ARRL = (tim1_auto_reload & 0xFF);

        uint16_t tim1_compare_reg1 = 1000; // 1ms=1000 2ms=2000
        //channel 1 compare PC3
        TIM1_CCR1H = (tim1_compare_reg1 >> 8);
        TIM1_CCR1L = (tim1_compare_reg1 & 0xFF);
        //channel 2 compare PC3
        TIM1_CCR2H = (tim1_compare_reg1 >> 8);
        TIM1_CCR2L = (tim1_compare_reg1 & 0xFF);
        //channel 3 compare PC3
        TIM1_CCR3H = (tim1_compare_reg1 >> 8);
        TIM1_CCR3L = (tim1_compare_reg1 & 0xFF);
        //channel 4 compare PC4
        TIM1_CCR4H = (tim1_compare_reg1 >> 8);
        TIM1_CCR4L = (tim1_compare_reg1 & 0xFF);

        TIM1_CCER1 = 0x11; // Enable compare channel 1 & 2 output
        TIM1_CCER2 = 0x11; // Enable compare channel 3 & 4 output
        TIM1_CCMR1 = 0x6 << 0x4; // PWM mode 1 for channel 1
        TIM1_CCMR2 = 0x6 << 0x4; // PWM mode 1 for channel 2
        TIM1_CCMR3 = 0x6 << 0x4; // PWM mode 1 for channel 3
        TIM1_CCMR4 = 0x6 << 0x4; // PWM mode 1 for channel 4

        TIM1_EGR |= 1; // Generate an update event to register new settings
        TIM1_BKR |= 0x1 << 0x7; // Enable TIM1 output channels
        TIM1_CR1 |= 1; // Enable the counter
    }
    /* timer 2  init for PWM*/
    {
        // processor is working at 16Mhz => time clock is 1Mhz => 1us
        TIM2_PSCR =  4 ; // 16 2⁴

        const uint16_t tim2_auto_reload = 10000; // 10 ms = 100Hz
        TIM2_ARRH = (tim2_auto_reload >> 8);
        TIM2_ARRL = (tim2_auto_reload & 0xFF);

        uint16_t tim2_compare_reg1 = 1000; // 1ms=1000 2ms=2000
        //channel 1 compare PC3
        TIM2_CCR1H = (tim2_compare_reg1 >> 8);
        TIM2_CCR1L = (tim2_compare_reg1 & 0xFF);
        //channel 2 compare PC3
        TIM2_CCR2H = (tim2_compare_reg1 >> 8);
        TIM2_CCR2L = (tim2_compare_reg1 & 0xFF);
        //channel 3 compare PC3
        TIM2_CCR3H = (tim2_compare_reg1 >> 8);
        TIM2_CCR3L = (tim2_compare_reg1 & 0xFF);

        TIM2_CCER1 = 0x11; // Enable compare channel 1 & 2 output
        TIM2_CCER2 = 0x01; // Enable compare channel 3  output
        TIM2_CCMR1 = 0x6 << 0x4; // PWM mode 1 for channel 1
        TIM2_CCMR2 = 0x6 << 0x4; // PWM mode 1 for channel 2
        TIM2_CCMR3 = 0x6 << 0x4; // PWM mode 1 for channel 3

        // TIM2_IER (Interrupt Enable Register), Update interrupt (UIE) (bit 0)
        TIM2_IER |= TIM_IER_UIE;

        TIM2_EGR |= 1; // Generate an update event to register new settings
        TIM2_CR1 |= 1; // Enable the counter
    }
   
    /* Setup UART1 (TX=D5 RX=D6) */
    {
        UART1_CR2 |= UART_CR2_TEN; // Transmitter enable
        UART1_CR2 |= UART_CR2_REN; // Receiver enable
        //UART1_CR3 |= UART_CR3_STOP1;
       // UART1_CR3 |= UART_CR3_STOP2; // 1.5 stop bit
        UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
        // 9600 baud: UART_DIV = 16000000/9600 ~ 1667 = 0x0683
        // UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 0x0683 coded funky way (see ref manual)
        // 1200 baud: UART_DIV = 16000000/1200 ~ 13333 = 0x3415
        UART1_BRR2 = 0x35; UART1_BRR1 = 0x41; // 0x3415 coded funky way (see ref manual)
    }


    setPWM1(1500);
    setPWM2(1500);
    setPWM3(1500);
    setPWM4(1500);
    setPWM5(1500);
    setPWM6(1500);
    setPWM7(1500);

    static int lAutoAnim=1;
    static T_robotFrame lRobotFrame;
    static T_robotData lRobotData;
    static T_robotCmdData lRobotCmdDataOut;
    static T_robotCmdData lRobotCmdDataIn;
    PB_ODR |= (1 << 5);
    enableInterrupts();
    while(1) 
    {
        

        // uint8_t c=(uint8_t)'A';
        // if(uart_readChar(&c))
        // {
        //     PB_ODR &= ~(1 << 5);

            // uart_writeChar((char)c);
            // uint16_t cmd = c*4+1000;
            // setPWM3(cmd);
            // setPWM4(cmd);
            // setPWM5(cmd);
            // setPWM6(cmd);
            // setPWM7(cmd);

        //     PB_ODR |= (1 << 5);
        // }


        int deltaTimeUs = gUsCounter;
        gUsCounter=0;
       
        uint8_t lByte;
        if(uart_readChar(&lByte))
        {
            int ret = robotFrameDecodeByByte(lByte,&lRobotCmdDataOut);
            if(ret==1) lAutoAnim=0;
        }

        if(lAutoAnim==1)
        {
            // robotAutoAnim(&lRobotCmdDataIn,deltaTimeUs);
            // robotFrameEncode(&lRobotCmdDataIn,&lRobotFrame);
            // for(int i=0;i<sizeof(T_robotFrame);i++)
            // {
            //     uint8_t  lByte = ((uint8_t*)&lRobotFrame)[i];
            //     int ret = robotFrameDecodeByByte(lByte,&lRobotCmdDataOut);
            // }
            robotAutoAnim(&lRobotCmdDataOut,deltaTimeUs);
        }

        robotUpdateData(&lRobotCmdDataOut,&lRobotData,deltaTimeUs);

        setPWM3(lRobotData.motor[0].pwm);
        setPWM4(lRobotData.motor[1].pwm);
        setPWM5(lRobotData.motor[2].pwm);
        setPWM6(lRobotData.motor[3].pwm);
        setPWM7(lRobotData.motor[4].pwm);
    }
}