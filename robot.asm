;--------------------------------------------------------
; File Created by SDCC : free open source ANSI-C Compiler
; Version 4.0.0 #11528 (Linux)
;--------------------------------------------------------
	.module robot
	.optsdcc -mstm8
	
;--------------------------------------------------------
; Public variables in this module
;--------------------------------------------------------
	.globl _main
	.globl _setPWM
	.globl _uart_write
	.globl _delay
	.globl _strlen
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area DATA
;--------------------------------------------------------
; ram data
;--------------------------------------------------------
	.area INITIALIZED
;--------------------------------------------------------
; Stack segment in internal ram 
;--------------------------------------------------------
	.area	SSEG
__start__stack:
	.ds	1

;--------------------------------------------------------
; absolute external ram data
;--------------------------------------------------------
	.area DABS (ABS)

; default segment ordering for linker
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area CONST
	.area INITIALIZER
	.area CODE

;--------------------------------------------------------
; interrupt vector 
;--------------------------------------------------------
	.area HOME
__interrupt_vect:
	int s_GSINIT ; reset
;--------------------------------------------------------
; global & static initialisations
;--------------------------------------------------------
	.area HOME
	.area GSINIT
	.area GSFINAL
	.area GSINIT
__sdcc_gs_init_startup:
__sdcc_init_data:
; stm8_genXINIT() start
	ldw x, #l_DATA
	jreq	00002$
00001$:
	clr (s_DATA - 1, x)
	decw x
	jrne	00001$
00002$:
	ldw	x, #l_INITIALIZER
	jreq	00004$
00003$:
	ld	a, (s_INITIALIZER - 1, x)
	ld	(s_INITIALIZED - 1, x), a
	decw	x
	jrne	00003$
00004$:
; stm8_genXINIT() end
	.area GSFINAL
	jp	__sdcc_program_startup
;--------------------------------------------------------
; Home
;--------------------------------------------------------
	.area HOME
	.area HOME
__sdcc_program_startup:
	jp	_main
;	return from main will return to caller
;--------------------------------------------------------
; code
;--------------------------------------------------------
	.area CODE
;	robot.c: 7: void delay(unsigned long count) {
;	-----------------------------------------
;	 function delay
;	-----------------------------------------
_delay:
	sub	sp, #8
;	robot.c: 8: while (count--)
	ldw	y, (0x0d, sp)
	ldw	(0x07, sp), y
	ldw	x, (0x0b, sp)
00101$:
	ldw	(0x01, sp), x
	ld	a, (0x07, sp)
	ld	(0x03, sp), a
	ld	a, (0x08, sp)
	ldw	y, (0x07, sp)
	subw	y, #0x0001
	ldw	(0x07, sp), y
	jrnc	00117$
	decw	x
00117$:
	tnz	a
	jrne	00118$
	ldw	y, (0x02, sp)
	jrne	00118$
	tnz	(0x01, sp)
	jreq	00104$
00118$:
;	robot.c: 9: nop();
	nop
	jra	00101$
00104$:
;	robot.c: 10: }
	addw	sp, #8
	ret
;	robot.c: 12: int uart_write(const char *str) {
;	-----------------------------------------
;	 function uart_write
;	-----------------------------------------
_uart_write:
	sub	sp, #3
;	robot.c: 14: for(i = 0; i < strlen(str); i++) {
	clr	(0x03, sp)
00106$:
	ldw	x, (0x06, sp)
	pushw	x
	call	_strlen
	addw	sp, #2
	ldw	(0x01, sp), x
	clrw	x
	ld	a, (0x03, sp)
	ld	xl, a
	cpw	x, (0x01, sp)
	jrnc	00104$
;	robot.c: 15: while(!(UART1_SR & UART_SR_TXE)); // !Transmit data register empty
00101$:
	ld	a, 0x5230
	jrpl	00101$
;	robot.c: 16: UART1_DR = str[i];
	clrw	x
	ld	a, (0x03, sp)
	ld	xl, a
	addw	x, (0x06, sp)
	ld	a, (x)
	ld	0x5231, a
;	robot.c: 14: for(i = 0; i < strlen(str); i++) {
	inc	(0x03, sp)
	jra	00106$
00104$:
;	robot.c: 18: return(i); // Bytes sent
	ld	a, (0x03, sp)
	clrw	x
	ld	xl, a
;	robot.c: 19: }
	addw	sp, #3
	ret
;	robot.c: 21: void setPWM(int pwm1,int pwm2)
;	-----------------------------------------
;	 function setPWM
;	-----------------------------------------
_setPWM:
;	robot.c: 24: TIM1_CCR3H = (pwm1 >> 8);
	ldw	x, (0x03, sp)
	clr	a
	tnzw	x
	jrpl	00103$
	dec	a
00103$:
	rrwa	x
	ld	a, xl
	ld	0x5269, a
;	robot.c: 25: TIM1_CCR3L = (pwm1 & 0xFF);
	ld	a, (0x04, sp)
	ld	0x526a, a
;	robot.c: 26: TIM1_CCR4H = (pwm2 >> 8);
	ldw	x, (0x05, sp)
	clr	a
	tnzw	x
	jrpl	00104$
	dec	a
00104$:
	rrwa	x
	ld	a, xl
	ld	0x526b, a
;	robot.c: 27: TIM1_CCR4L = (pwm2 & 0xFF);
	ld	a, (0x06, sp)
	ld	0x526c, a
;	robot.c: 28: } 
	ret
;	robot.c: 33: int main(void)
;	-----------------------------------------
;	 function main
;	-----------------------------------------
_main:
;	robot.c: 36: CLK_CKDIVR = 0;
	mov	0x50c6+0, #0x00
;	robot.c: 40: PB_DDR |= (1 << 5);
	bset	20487, #5
;	robot.c: 41: PC_DDR |= (1 << 3);
	bset	20492, #3
;	robot.c: 42: PC_DDR |= (1 << 4);
	bset	20492, #4
;	robot.c: 44: PB_CR1 |= (1 << 5);
	bset	20488, #5
;	robot.c: 45: PC_CR1 |= (1 << 3);
	bset	20493, #3
;	robot.c: 46: PC_CR1 |= (1 << 4);
	bset	20493, #4
;	robot.c: 49: UART1_CR2 |= UART_CR2_TEN; // Transmitter enable
	bset	21045, #3
;	robot.c: 51: UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
	ld	a, 0x5236
	and	a, #0xcf
	ld	0x5236, a
;	robot.c: 53: UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 0x0683 coded funky way (see ref manual)
	mov	0x5233+0, #0x03
	mov	0x5232+0, #0x68
;	robot.c: 57: TIM1_PSCRH = (tim1_prescaler >> 8);
	clr	a
	ld	0x5260, a
;	robot.c: 58: TIM1_PSCRL = (tim1_prescaler & 0xFF);
	ld	a, #0x10
	ld	0x5261, a
;	robot.c: 61: TIM1_ARRH = (tim1_auto_reload >> 8);
	ld	a, #0x27
	ld	0x5262, a
;	robot.c: 62: TIM1_ARRL = (tim1_auto_reload & 0xFF);
	ld	a, #0x10
	ld	0x5263, a
;	robot.c: 66: TIM1_CCR3H = (tim1_compare_reg1 >> 8);
	ld	a, #0x03
	ld	0x5269, a
;	robot.c: 67: TIM1_CCR3L = (tim1_compare_reg1 & 0xFF);
	ld	a, #0xe8
	ld	0x526a, a
;	robot.c: 69: TIM1_CCR4H = (tim1_compare_reg1 >> 8);
	ld	a, #0x03
	ld	0x526b, a
;	robot.c: 70: TIM1_CCR4L = (tim1_compare_reg1 & 0xFF);
	ld	a, #0xe8
	ld	0x526c, a
;	robot.c: 72: TIM1_CCER2 = 0x11; // Enable compare channel 3 & 4 output
	mov	0x525d+0, #0x11
;	robot.c: 73: TIM1_CCMR3 = 0x6 << 0x4; // PWM mode 1 for channel 3
	mov	0x525a+0, #0x60
;	robot.c: 74: TIM1_CCMR4 = 0x6 << 0x4; // PWM mode 1 for channel 4
	mov	0x525b+0, #0x60
;	robot.c: 76: TIM1_EGR |= 1; // Generate an update event to register new settings
	bset	21079, #0
;	robot.c: 77: TIM1_BKR |= 0x1 << 0x7; // Enable TIM1 output channels
	bset	21101, #7
;	robot.c: 78: TIM1_CR1 |= 1; // Enable the counter
	bset	21072, #0
;	robot.c: 83: setPWM(2000,1000); // 1 0
	push	#0xe8
	push	#0x03
	push	#0xd0
	push	#0x07
	call	_setPWM
	addw	sp, #4
;	robot.c: 85: while(1) {
00102$:
;	robot.c: 108: setPWM(1000,2000); // 0.0 1.0
	push	#0xd0
	push	#0x07
	push	#0xe8
	push	#0x03
	call	_setPWM
	addw	sp, #4
;	robot.c: 109: delay(1000000);
	push	#0x40
	push	#0x42
	push	#0x0f
	push	#0x00
	call	_delay
	addw	sp, #4
;	robot.c: 110: PB_ODR |= (1 << 5);
	bset	20485, #5
;	robot.c: 111: setPWM(2000,1000); // 1.0 0.0
	push	#0xe8
	push	#0x03
	push	#0xd0
	push	#0x07
	call	_setPWM
	addw	sp, #4
;	robot.c: 112: delay(1000000);
	push	#0x40
	push	#0x42
	push	#0x0f
	push	#0x00
	call	_delay
	addw	sp, #4
;	robot.c: 113: PB_ODR &= ~(1 << 5);
	bres	20485, #5
;	robot.c: 115: setPWM(1000,1000); // 0.0 1.0
	push	#0xe8
	push	#0x03
	push	#0xe8
	push	#0x03
	call	_setPWM
	addw	sp, #4
;	robot.c: 116: delay(1000000);
	push	#0x40
	push	#0x42
	push	#0x0f
	push	#0x00
	call	_delay
	addw	sp, #4
;	robot.c: 117: PB_ODR |= (1 << 5);
	bset	20485, #5
	jra	00102$
;	robot.c: 122: }
	ret
	.area CODE
	.area CONST
	.area INITIALIZER
	.area CABS (ABS)
