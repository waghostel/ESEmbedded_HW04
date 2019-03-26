#include "blink.h"

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
