#ifndef REG_H
#define REG_H

//REG OPERATIONS
#define UINT32_1 ((uint32_t)1)


//傳回指向記憶體中數值的數值
#define REG(addr) (*((volatile uint32_t *)(addr)))
/*
(volatile uint32_t *)(addr)     :指向記憶體位置的指標
*((volatile uint32_t *)(addr))  :指標的指標，所以是數值
*/

//shift bit bit 數
#define SET_BIT(addr, bit) (REG(addr) |= UINT32_1 << (bit)) //Set 特定bit的macro
/*
UINT32_1 << (bit) : 將(uint32_t)1 shift bit個bit  ex: 00001-> 00100
|= :取or運算
假設REG(addr)內部除存的數值是:10001
(100001)|=00100 = 100101
*/


#define READ_BIT(addr,bit) (REG(addr) &= UINT32_1<<(bit))

//Clear特定bit:把目標bit轉成0
#define CLEAR_BIT(addr, bit) (REG(addr) &= ~(UINT32_1 << (bit))) 
/*
先shift bit後做not 再做and
~(UINT32_1 << (bit)):shift完後反轉再取and
假設:
REG(addr)=100010
~(UINT32_1 << (bit))=10000
(100010)&=(100000)=100000
*/

//RCC(Reset and clock control, RCC所在的位置)
#define RCC_BASE 0x40023800

//Define offset
#define RCC_AHB1ENR_OFFSET 0x30
#define GPIO_EN_BIT(port) (port)

//GPIO
//Macro will be put into the code directly
#define GPIO_PORTA 0
#define GPIO_PORTB 1
#define GPIO_PORTC 2
#define GPIO_PORTD 3

//Macro function 
//實際上在preprocessor會直接copy過去
#define GPIO_BASE(port) (0x40020000 + 0x400 * (port))

#define GPIOx_MODER_OFFSET 0x00 //不需要設定offset，所以是0x00
#define MODERy_1_BIT(y) ((y)*2 + 1) //15*2+1=31
#define MODERy_0_BIT(y) ((y)*2)    //15*2=30=30  
/*注意((y)*2)要加上層層括弧，避免像是傳入y=a+2的時候先乘除後加減的結果變成:
y+2*2
*/

//OTYPER: Output type register()
#define GPIOx_OTYPER_OFFSET 0x04
#define OTy_BIT(y) (y)

//OSPEEDR: Output speeder register
#define GPIOx_OSPEEDR_OFFSET 0x08
#define OSPEEDRy_1_BIT(y) ((y)*2 + 1) 
#define OSPEEDRy_0_BIT(y) ((y)*2) 

//pull-up/pull-down register
#define GPIOx_PUPDR_OFFSET 0x0C
#define PUPDRy_1_BIT(y) ((y)*2 + 1) 
#define PUPDRy_0_BIT(y) ((y)*2)

//Bit set/reset register
#define GPIOx_BSRR_OFFSET 0x18
#define BRy_BIT(y) ((y) + 16) //Reset
#define BSy_BIT(y) (y) //Set
#define BGy_BIT(y) (y) //Get


#define GPIOx_IDR_OFFSET 0x10
#endif
