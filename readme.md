HW04
===
This is the hw04 sample. Please follow the steps below.

# Build the Sample Program

1. Fork this repo to your own github account.

2. Clone the repo that you just forked.

3. Under the hw04 dir, use:

	* `make` to build.

	* `make flash` to flash the binary file into your STM32F4 device.

	* `make clean` to clean the ouput files.

# Build Your Own Program

1. Edit or add files if you need to.

2. Make and run like the steps above.

3. Please avoid using hardware dependent C standard library functions like `printf`, `malloc`, etc.

# HW04 Requirements

1. Please practice to reference the user manuals mentioned in [Lecture 04], and try to use the user button (the blue one on the STM32F4DISCOVERY board).

2. After reset, the device starts to wait until the user button has been pressed, and then starts to blink the blue led forever.

3. Try to define a macro function `READ_BIT(addr, bit)` in reg.h for reading the value of the user button.

4. Push your repo to your github. (Use .gitignore to exclude the output files like object files, executable files, or binary files)

5. The TAs will clone your repo, build from your source code, and flash to see the result.

[Lecture 04]: http://www.nc.es.ncku.edu.tw/course/embedded/04/

--------------------

- [ ] **If you volunteer to give the presentation (demo) next week, check this.**

--------------------

Take your note here if you want. (Optional)


## **1. Push Button - GPIO port/pin**

為了找到按鈕走的GPIO port，參考 *UM1472 User manual Discovery kit with STM32F407VG MCU*手冊，可在第6.4章找到Push buttons的相關資訊，按鈕 ****`B1 USER`經由I/O `PA0`通訊。

![](https://d2mxuefqeaa7sj.cloudfront.net/s_4CF687D3288E5A7F0FB42018E24FCEB8FDD59602DDCE9D3F33A09359C11BA63D_1553521475197_image.png)

![](/static/img/pixel.gif)

----------
## **2. Enable Reset and clock control (RCC) bit 0**

Port A經由**AHB1**控制，在接收GPIO A0腳位置前，為了GPIO A port 能夠順利傳輸，需要將**Reset and clock control(RCC)** register的 bit 0設定成1。 


![](https://d2mxuefqeaa7sj.cloudfront.net/s_4CF687D3288E5A7F0FB42018E24FCEB8FDD59602DDCE9D3F33A09359C11BA63D_1553522293296_image.png)


撰寫開啟GPIO Port A的程式如下:

    #define GPIO_PORTA 0
    SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));




----------
## **3. 下拉電阻(Pull-down resister)**

為了判斷按鈕的電路設計類型，參考 *UM1472 User manual Discovery kit with STM32F407VG MCU*手冊，由p31 線路圖中可看出按鈕電路為**下拉電阻(pull-down resister)**。

![](https://d2mxuefqeaa7sj.cloudfront.net/s_4CF687D3288E5A7F0FB42018E24FCEB8FDD59602DDCE9D3F33A09359C11BA63D_1553521743133_image.png)





----------
## 4. **設定Port A 初始參數**

綜合上述資訊，參考*RM0090 Reference manual* p 269 的Table 35，將初始化參數設定如下:

**MODER:** `00(input mode)`
******PUPDR:** `10(Input, pull-down mode)`

OTYPER與OSPEEDER在input狀態則不需要額外設定。

![](https://d2mxuefqeaa7sj.cloudfront.net/s_4CF687D3288E5A7F0FB42018E24FCEB8FDD59602DDCE9D3F33A09359C11BA63D_1553522929885_image.png)


btn_Init()函式的綜合上述初始化功能，包含打開RCC的 port A，及分別設定MODER和PUPDR為`00`和`10`。


    void btn_Init(unsigned int btn){
        //Set PORT A open       
        SET_BIT(RCC_BASE + RCC_AHB1ENR_OFFSET, GPIO_EN_BIT(GPIO_PORTA));
        
        //MODER: led pin = 00 => Input mode
        CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_1_BIT(btn));
        CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_MODER_OFFSET, MODERy_0_BIT(btn));
        
        //PUPDR: led pin = 10=> pull-down
        SET_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_1_BIT(btn));
        CLEAR_BIT(GPIO_BASE(GPIO_PORTD) + GPIOx_PUPDR_OFFSET, PUPDRy_0_BIT(btn));
    }




----------


## 5. **讀取PA0的數據**

在初始化`PA0`後，可開始讀取按鈕資料。參考*M0090 Reference manual* 8.4.5章，讀取`PA0`需透過GPIO port input data resiter。 在資料讀取上需要加上0x10的 address offset。

![](https://d2mxuefqeaa7sj.cloudfront.net/s_4CF687D3288E5A7F0FB42018E24FCEB8FDD59602DDCE9D3F33A09359C11BA63D_1553521175250_image.png)


Read_BIT macro可讀取addr的記憶體位置，並且判斷第bit個位置是否為1

    #define READ_BIT(addr,bit) (REG(addr) &= UINT32_1<<(bit))

READ_BIT函數讀取bit，判斷是否按鈕被按下。

    #define GPIOx_IDR_OFFSET 0x10
    #define READ_BIT(addr,bit) (REG(addr) &= UINT32_1<<(bit))
    #define BUTTON_USER 0
    curruntStatus=READ_BIT(GPIO_BASE(GPIO_PORTA) + GPIOx_IDR_OFFSET, BGy_BIT(BUTTON_USER));


主體main函式會在按鈕被按下，並且目前沒有閃爍的狀態閃爍LED燈5下，在閃爍完成後，重新設定`blinkStatus`為0。

main.c

    int main(void)
    {
        int blinkStatus=0; //Is LED blinking now?
        int btnTmp=0; //Is btn clicked?
        while(1){
        //Determine if button has been clicked  
        btnTmp=getInput(BUTTON_USER);
        //If button clicked
        if (btnTmp==1 && blinkStatus==0){
        blink_count(LED_BLUE,5);
        blinkStatus=1;
        }
        //Turn to button unclicked status
        else if(blinkStatus==1){
            blinkStatus=0;
        }
        
        }   
    }
