                                      1 ;--------------------------------------------------------
                                      2 ; File Created by SDCC : free open source ANSI-C Compiler
                                      3 ; Version 4.0.0 #11528 (Linux)
                                      4 ;--------------------------------------------------------
                                      5 	.module robot
                                      6 	.optsdcc -mstm8
                                      7 	
                                      8 ;--------------------------------------------------------
                                      9 ; Public variables in this module
                                     10 ;--------------------------------------------------------
                                     11 	.globl _main
                                     12 	.globl _setPWM
                                     13 	.globl _uart_write
                                     14 	.globl _delay
                                     15 	.globl _strlen
                                     16 ;--------------------------------------------------------
                                     17 ; ram data
                                     18 ;--------------------------------------------------------
                                     19 	.area DATA
                                     20 ;--------------------------------------------------------
                                     21 ; ram data
                                     22 ;--------------------------------------------------------
                                     23 	.area INITIALIZED
                                     24 ;--------------------------------------------------------
                                     25 ; Stack segment in internal ram 
                                     26 ;--------------------------------------------------------
                                     27 	.area	SSEG
      FFFFFF                         28 __start__stack:
      FFFFFF                         29 	.ds	1
                                     30 
                                     31 ;--------------------------------------------------------
                                     32 ; absolute external ram data
                                     33 ;--------------------------------------------------------
                                     34 	.area DABS (ABS)
                                     35 
                                     36 ; default segment ordering for linker
                                     37 	.area HOME
                                     38 	.area GSINIT
                                     39 	.area GSFINAL
                                     40 	.area CONST
                                     41 	.area INITIALIZER
                                     42 	.area CODE
                                     43 
                                     44 ;--------------------------------------------------------
                                     45 ; interrupt vector 
                                     46 ;--------------------------------------------------------
                                     47 	.area HOME
      008000                         48 __interrupt_vect:
      008000 82 00 80 07             49 	int s_GSINIT ; reset
                                     50 ;--------------------------------------------------------
                                     51 ; global & static initialisations
                                     52 ;--------------------------------------------------------
                                     53 	.area HOME
                                     54 	.area GSINIT
                                     55 	.area GSFINAL
                                     56 	.area GSINIT
      008007                         57 __sdcc_gs_init_startup:
      008007                         58 __sdcc_init_data:
                                     59 ; stm8_genXINIT() start
      008007 AE 00 00         [ 2]   60 	ldw x, #l_DATA
      00800A 27 07            [ 1]   61 	jreq	00002$
      00800C                         62 00001$:
      00800C 72 4F 00 00      [ 1]   63 	clr (s_DATA - 1, x)
      008010 5A               [ 2]   64 	decw x
      008011 26 F9            [ 1]   65 	jrne	00001$
      008013                         66 00002$:
      008013 AE 00 00         [ 2]   67 	ldw	x, #l_INITIALIZER
      008016 27 09            [ 1]   68 	jreq	00004$
      008018                         69 00003$:
      008018 D6 80 23         [ 1]   70 	ld	a, (s_INITIALIZER - 1, x)
      00801B D7 00 00         [ 1]   71 	ld	(s_INITIALIZED - 1, x), a
      00801E 5A               [ 2]   72 	decw	x
      00801F 26 F7            [ 1]   73 	jrne	00003$
      008021                         74 00004$:
                                     75 ; stm8_genXINIT() end
                                     76 	.area GSFINAL
      008021 CC 80 04         [ 2]   77 	jp	__sdcc_program_startup
                                     78 ;--------------------------------------------------------
                                     79 ; Home
                                     80 ;--------------------------------------------------------
                                     81 	.area HOME
                                     82 	.area HOME
      008004                         83 __sdcc_program_startup:
      008004 CC 80 A4         [ 2]   84 	jp	_main
                                     85 ;	return from main will return to caller
                                     86 ;--------------------------------------------------------
                                     87 ; code
                                     88 ;--------------------------------------------------------
                                     89 	.area CODE
                                     90 ;	robot.c: 7: void delay(unsigned long count) {
                                     91 ;	-----------------------------------------
                                     92 ;	 function delay
                                     93 ;	-----------------------------------------
      008024                         94 _delay:
      008024 52 08            [ 2]   95 	sub	sp, #8
                                     96 ;	robot.c: 8: while (count--)
      008026 16 0D            [ 2]   97 	ldw	y, (0x0d, sp)
      008028 17 07            [ 2]   98 	ldw	(0x07, sp), y
      00802A 1E 0B            [ 2]   99 	ldw	x, (0x0b, sp)
      00802C                        100 00101$:
      00802C 1F 01            [ 2]  101 	ldw	(0x01, sp), x
      00802E 7B 07            [ 1]  102 	ld	a, (0x07, sp)
      008030 6B 03            [ 1]  103 	ld	(0x03, sp), a
      008032 7B 08            [ 1]  104 	ld	a, (0x08, sp)
      008034 16 07            [ 2]  105 	ldw	y, (0x07, sp)
      008036 72 A2 00 01      [ 2]  106 	subw	y, #0x0001
      00803A 17 07            [ 2]  107 	ldw	(0x07, sp), y
      00803C 24 01            [ 1]  108 	jrnc	00117$
      00803E 5A               [ 2]  109 	decw	x
      00803F                        110 00117$:
      00803F 4D               [ 1]  111 	tnz	a
      008040 26 08            [ 1]  112 	jrne	00118$
      008042 16 02            [ 2]  113 	ldw	y, (0x02, sp)
      008044 26 04            [ 1]  114 	jrne	00118$
      008046 0D 01            [ 1]  115 	tnz	(0x01, sp)
      008048 27 03            [ 1]  116 	jreq	00104$
      00804A                        117 00118$:
                                    118 ;	robot.c: 9: nop();
      00804A 9D               [ 1]  119 	nop
      00804B 20 DF            [ 2]  120 	jra	00101$
      00804D                        121 00104$:
                                    122 ;	robot.c: 10: }
      00804D 5B 08            [ 2]  123 	addw	sp, #8
      00804F 81               [ 4]  124 	ret
                                    125 ;	robot.c: 12: int uart_write(const char *str) {
                                    126 ;	-----------------------------------------
                                    127 ;	 function uart_write
                                    128 ;	-----------------------------------------
      008050                        129 _uart_write:
      008050 52 03            [ 2]  130 	sub	sp, #3
                                    131 ;	robot.c: 14: for(i = 0; i < strlen(str); i++) {
      008052 0F 03            [ 1]  132 	clr	(0x03, sp)
      008054                        133 00106$:
      008054 1E 06            [ 2]  134 	ldw	x, (0x06, sp)
      008056 89               [ 2]  135 	pushw	x
      008057 CD 81 7D         [ 4]  136 	call	_strlen
      00805A 5B 02            [ 2]  137 	addw	sp, #2
      00805C 1F 01            [ 2]  138 	ldw	(0x01, sp), x
      00805E 5F               [ 1]  139 	clrw	x
      00805F 7B 03            [ 1]  140 	ld	a, (0x03, sp)
      008061 97               [ 1]  141 	ld	xl, a
      008062 13 01            [ 2]  142 	cpw	x, (0x01, sp)
      008064 24 14            [ 1]  143 	jrnc	00104$
                                    144 ;	robot.c: 15: while(!(UART1_SR & UART_SR_TXE)); // !Transmit data register empty
      008066                        145 00101$:
      008066 C6 52 30         [ 1]  146 	ld	a, 0x5230
      008069 2A FB            [ 1]  147 	jrpl	00101$
                                    148 ;	robot.c: 16: UART1_DR = str[i];
      00806B 5F               [ 1]  149 	clrw	x
      00806C 7B 03            [ 1]  150 	ld	a, (0x03, sp)
      00806E 97               [ 1]  151 	ld	xl, a
      00806F 72 FB 06         [ 2]  152 	addw	x, (0x06, sp)
      008072 F6               [ 1]  153 	ld	a, (x)
      008073 C7 52 31         [ 1]  154 	ld	0x5231, a
                                    155 ;	robot.c: 14: for(i = 0; i < strlen(str); i++) {
      008076 0C 03            [ 1]  156 	inc	(0x03, sp)
      008078 20 DA            [ 2]  157 	jra	00106$
      00807A                        158 00104$:
                                    159 ;	robot.c: 18: return(i); // Bytes sent
      00807A 7B 03            [ 1]  160 	ld	a, (0x03, sp)
      00807C 5F               [ 1]  161 	clrw	x
      00807D 97               [ 1]  162 	ld	xl, a
                                    163 ;	robot.c: 19: }
      00807E 5B 03            [ 2]  164 	addw	sp, #3
      008080 81               [ 4]  165 	ret
                                    166 ;	robot.c: 21: void setPWM(int pwm1,int pwm2)
                                    167 ;	-----------------------------------------
                                    168 ;	 function setPWM
                                    169 ;	-----------------------------------------
      008081                        170 _setPWM:
                                    171 ;	robot.c: 24: TIM1_CCR3H = (pwm1 >> 8);
      008081 1E 03            [ 2]  172 	ldw	x, (0x03, sp)
      008083 4F               [ 1]  173 	clr	a
      008084 5D               [ 2]  174 	tnzw	x
      008085 2A 01            [ 1]  175 	jrpl	00103$
      008087 4A               [ 1]  176 	dec	a
      008088                        177 00103$:
      008088 01               [ 1]  178 	rrwa	x
      008089 9F               [ 1]  179 	ld	a, xl
      00808A C7 52 69         [ 1]  180 	ld	0x5269, a
                                    181 ;	robot.c: 25: TIM1_CCR3L = (pwm1 & 0xFF);
      00808D 7B 04            [ 1]  182 	ld	a, (0x04, sp)
      00808F C7 52 6A         [ 1]  183 	ld	0x526a, a
                                    184 ;	robot.c: 26: TIM1_CCR4H = (pwm2 >> 8);
      008092 1E 05            [ 2]  185 	ldw	x, (0x05, sp)
      008094 4F               [ 1]  186 	clr	a
      008095 5D               [ 2]  187 	tnzw	x
      008096 2A 01            [ 1]  188 	jrpl	00104$
      008098 4A               [ 1]  189 	dec	a
      008099                        190 00104$:
      008099 01               [ 1]  191 	rrwa	x
      00809A 9F               [ 1]  192 	ld	a, xl
      00809B C7 52 6B         [ 1]  193 	ld	0x526b, a
                                    194 ;	robot.c: 27: TIM1_CCR4L = (pwm2 & 0xFF);
      00809E 7B 06            [ 1]  195 	ld	a, (0x06, sp)
      0080A0 C7 52 6C         [ 1]  196 	ld	0x526c, a
                                    197 ;	robot.c: 28: } 
      0080A3 81               [ 4]  198 	ret
                                    199 ;	robot.c: 33: int main(void)
                                    200 ;	-----------------------------------------
                                    201 ;	 function main
                                    202 ;	-----------------------------------------
      0080A4                        203 _main:
                                    204 ;	robot.c: 36: CLK_CKDIVR = 0;
      0080A4 35 00 50 C6      [ 1]  205 	mov	0x50c6+0, #0x00
                                    206 ;	robot.c: 40: PB_DDR |= (1 << 5);
      0080A8 72 1A 50 07      [ 1]  207 	bset	20487, #5
                                    208 ;	robot.c: 41: PC_DDR |= (1 << 3);
      0080AC 72 16 50 0C      [ 1]  209 	bset	20492, #3
                                    210 ;	robot.c: 42: PC_DDR |= (1 << 4);
      0080B0 72 18 50 0C      [ 1]  211 	bset	20492, #4
                                    212 ;	robot.c: 44: PB_CR1 |= (1 << 5);
      0080B4 72 1A 50 08      [ 1]  213 	bset	20488, #5
                                    214 ;	robot.c: 45: PC_CR1 |= (1 << 3);
      0080B8 72 16 50 0D      [ 1]  215 	bset	20493, #3
                                    216 ;	robot.c: 46: PC_CR1 |= (1 << 4);
      0080BC 72 18 50 0D      [ 1]  217 	bset	20493, #4
                                    218 ;	robot.c: 49: UART1_CR2 |= UART_CR2_TEN; // Transmitter enable
      0080C0 72 16 52 35      [ 1]  219 	bset	21045, #3
                                    220 ;	robot.c: 51: UART1_CR3 &= ~(UART_CR3_STOP1 | UART_CR3_STOP2); // 1 stop bit
      0080C4 C6 52 36         [ 1]  221 	ld	a, 0x5236
      0080C7 A4 CF            [ 1]  222 	and	a, #0xcf
      0080C9 C7 52 36         [ 1]  223 	ld	0x5236, a
                                    224 ;	robot.c: 53: UART1_BRR2 = 0x03; UART1_BRR1 = 0x68; // 0x0683 coded funky way (see ref manual)
      0080CC 35 03 52 33      [ 1]  225 	mov	0x5233+0, #0x03
      0080D0 35 68 52 32      [ 1]  226 	mov	0x5232+0, #0x68
                                    227 ;	robot.c: 57: TIM1_PSCRH = (tim1_prescaler >> 8);
      0080D4 4F               [ 1]  228 	clr	a
      0080D5 C7 52 60         [ 1]  229 	ld	0x5260, a
                                    230 ;	robot.c: 58: TIM1_PSCRL = (tim1_prescaler & 0xFF);
      0080D8 A6 10            [ 1]  231 	ld	a, #0x10
      0080DA C7 52 61         [ 1]  232 	ld	0x5261, a
                                    233 ;	robot.c: 61: TIM1_ARRH = (tim1_auto_reload >> 8);
      0080DD A6 27            [ 1]  234 	ld	a, #0x27
      0080DF C7 52 62         [ 1]  235 	ld	0x5262, a
                                    236 ;	robot.c: 62: TIM1_ARRL = (tim1_auto_reload & 0xFF);
      0080E2 A6 10            [ 1]  237 	ld	a, #0x10
      0080E4 C7 52 63         [ 1]  238 	ld	0x5263, a
                                    239 ;	robot.c: 66: TIM1_CCR3H = (tim1_compare_reg1 >> 8);
      0080E7 A6 03            [ 1]  240 	ld	a, #0x03
      0080E9 C7 52 69         [ 1]  241 	ld	0x5269, a
                                    242 ;	robot.c: 67: TIM1_CCR3L = (tim1_compare_reg1 & 0xFF);
      0080EC A6 E8            [ 1]  243 	ld	a, #0xe8
      0080EE C7 52 6A         [ 1]  244 	ld	0x526a, a
                                    245 ;	robot.c: 69: TIM1_CCR4H = (tim1_compare_reg1 >> 8);
      0080F1 A6 03            [ 1]  246 	ld	a, #0x03
      0080F3 C7 52 6B         [ 1]  247 	ld	0x526b, a
                                    248 ;	robot.c: 70: TIM1_CCR4L = (tim1_compare_reg1 & 0xFF);
      0080F6 A6 E8            [ 1]  249 	ld	a, #0xe8
      0080F8 C7 52 6C         [ 1]  250 	ld	0x526c, a
                                    251 ;	robot.c: 72: TIM1_CCER2 = 0x11; // Enable compare channel 3 & 4 output
      0080FB 35 11 52 5D      [ 1]  252 	mov	0x525d+0, #0x11
                                    253 ;	robot.c: 73: TIM1_CCMR3 = 0x6 << 0x4; // PWM mode 1 for channel 3
      0080FF 35 60 52 5A      [ 1]  254 	mov	0x525a+0, #0x60
                                    255 ;	robot.c: 74: TIM1_CCMR4 = 0x6 << 0x4; // PWM mode 1 for channel 4
      008103 35 60 52 5B      [ 1]  256 	mov	0x525b+0, #0x60
                                    257 ;	robot.c: 76: TIM1_EGR |= 1; // Generate an update event to register new settings
      008107 72 10 52 57      [ 1]  258 	bset	21079, #0
                                    259 ;	robot.c: 77: TIM1_BKR |= 0x1 << 0x7; // Enable TIM1 output channels
      00810B 72 1E 52 6D      [ 1]  260 	bset	21101, #7
                                    261 ;	robot.c: 78: TIM1_CR1 |= 1; // Enable the counter
      00810F 72 10 52 50      [ 1]  262 	bset	21072, #0
                                    263 ;	robot.c: 83: setPWM(2000,1000); // 1 0
      008113 4B E8            [ 1]  264 	push	#0xe8
      008115 4B 03            [ 1]  265 	push	#0x03
      008117 4B D0            [ 1]  266 	push	#0xd0
      008119 4B 07            [ 1]  267 	push	#0x07
      00811B CD 80 81         [ 4]  268 	call	_setPWM
      00811E 5B 04            [ 2]  269 	addw	sp, #4
                                    270 ;	robot.c: 85: while(1) {
      008120                        271 00102$:
                                    272 ;	robot.c: 108: setPWM(1000,2000); // 0.0 1.0
      008120 4B D0            [ 1]  273 	push	#0xd0
      008122 4B 07            [ 1]  274 	push	#0x07
      008124 4B E8            [ 1]  275 	push	#0xe8
      008126 4B 03            [ 1]  276 	push	#0x03
      008128 CD 80 81         [ 4]  277 	call	_setPWM
      00812B 5B 04            [ 2]  278 	addw	sp, #4
                                    279 ;	robot.c: 109: delay(1000000);
      00812D 4B 40            [ 1]  280 	push	#0x40
      00812F 4B 42            [ 1]  281 	push	#0x42
      008131 4B 0F            [ 1]  282 	push	#0x0f
      008133 4B 00            [ 1]  283 	push	#0x00
      008135 CD 80 24         [ 4]  284 	call	_delay
      008138 5B 04            [ 2]  285 	addw	sp, #4
                                    286 ;	robot.c: 110: PB_ODR |= (1 << 5);
      00813A 72 1A 50 05      [ 1]  287 	bset	20485, #5
                                    288 ;	robot.c: 111: setPWM(2000,1000); // 1.0 0.0
      00813E 4B E8            [ 1]  289 	push	#0xe8
      008140 4B 03            [ 1]  290 	push	#0x03
      008142 4B D0            [ 1]  291 	push	#0xd0
      008144 4B 07            [ 1]  292 	push	#0x07
      008146 CD 80 81         [ 4]  293 	call	_setPWM
      008149 5B 04            [ 2]  294 	addw	sp, #4
                                    295 ;	robot.c: 112: delay(1000000);
      00814B 4B 40            [ 1]  296 	push	#0x40
      00814D 4B 42            [ 1]  297 	push	#0x42
      00814F 4B 0F            [ 1]  298 	push	#0x0f
      008151 4B 00            [ 1]  299 	push	#0x00
      008153 CD 80 24         [ 4]  300 	call	_delay
      008156 5B 04            [ 2]  301 	addw	sp, #4
                                    302 ;	robot.c: 113: PB_ODR &= ~(1 << 5);
      008158 72 1B 50 05      [ 1]  303 	bres	20485, #5
                                    304 ;	robot.c: 115: setPWM(1000,1000); // 0.0 1.0
      00815C 4B E8            [ 1]  305 	push	#0xe8
      00815E 4B 03            [ 1]  306 	push	#0x03
      008160 4B E8            [ 1]  307 	push	#0xe8
      008162 4B 03            [ 1]  308 	push	#0x03
      008164 CD 80 81         [ 4]  309 	call	_setPWM
      008167 5B 04            [ 2]  310 	addw	sp, #4
                                    311 ;	robot.c: 116: delay(1000000);
      008169 4B 40            [ 1]  312 	push	#0x40
      00816B 4B 42            [ 1]  313 	push	#0x42
      00816D 4B 0F            [ 1]  314 	push	#0x0f
      00816F 4B 00            [ 1]  315 	push	#0x00
      008171 CD 80 24         [ 4]  316 	call	_delay
      008174 5B 04            [ 2]  317 	addw	sp, #4
                                    318 ;	robot.c: 117: PB_ODR |= (1 << 5);
      008176 72 1A 50 05      [ 1]  319 	bset	20485, #5
      00817A 20 A4            [ 2]  320 	jra	00102$
                                    321 ;	robot.c: 122: }
      00817C 81               [ 4]  322 	ret
                                    323 	.area CODE
                                    324 	.area CONST
                                    325 	.area INITIALIZER
                                    326 	.area CABS (ABS)
