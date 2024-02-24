/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * ECE319K students can use indices or pointers for next state
 * Put your names here or look silly
  */

#include <ti/devices/msp/msp.h>
#include "../inc/LaunchPad.h"
#include "../inc/Clock.h"
#include "../inc/UART.h"
#include "../inc/Timer.h"
#include "../inc/Dump.h"  // student's Lab 3
#include <stdio.h>
#include <string.h>
// put both EIDs in the next two lines
const char EID1[] = "HJ8379"; //  ;replace abc123 with your EID
const char EID2[] = "JES7539"; //  ;replace abc123 with your EID


// initialize 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
 // write this
}
/* Activate LEDs
* Inputs: data1,data2,data3
*   specify what these means
* Output: none
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(uint32_t data1, uint32_t south, uint32_t walk){
 // write this

}
/* Read sensors
 * Input: none
 * Output: sensor values
*   specify what these means
* Feel free to change this. But, if you change the way it works, change the test programs too
 */
uint32_t Traffic_In(void){
 // write this
 return 42; // replace this line
}
// use main1 to determine Lab4 assignment
void Lab4Grader(int mode);
void Grader_Init(void);
int main(void){ // main1
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Lab4Grader(0); // print assignment, no grading
  while(1){
  }
}
// use main2 to debug LED outputs
int main2(void){ // main2
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();  // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  if((GPIOB->DOE31_0 & 0x20)==0){
    UART_OutString("access to GPIOB->DOE31_0 should be friendly.\n\r");
  }
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  UART_OutString("Lab 4, Spring 2024, Step 1. Debug LEDs\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    //write debug code to test your Traffic_Out
	// Call Traffic_Out testing all LED patterns
	// Lab 3 dump to record output values
	
    Clock_Delay(40000000); // 0.5s
    if((GPIOB->DOUT31_0&0x20) == 0){
      UART_OutString("DOUT not friendly\n\r");
    }
  }
}
// use main3 to debug the three input switches
int main3(void){ // main3
  uint32_t last=0,now;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2024, Step 2. Debug switches\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
  while(1){
    now = Traffic_In(); // Your Lab4 input
    if(now != last){ // change
      UART_OutString("Switch= 0x"); UART_OutUHex(now); UART_OutString("\n\r");
      Debug_Dump(now);
    }
    last = now;
    Clock_Delay(800000); // 10ms, to debounce switch
  }
}
// use main4 to debug using your dump
// proving your machine cycles through all states
int main4(void){// main4
uint32_t input;
  Clock_Init80MHz(0);
  LaunchPad_Init();
  LaunchPad_LED1off();
  Traffic_Init(); // your Lab 4 initialization
  Debug_Init();   // Lab 3 debugging
  UART_Init();
  __enable_irq(); // UART uses interrupts
  UART_OutString("Lab 4, Spring 2024, Step 3. Debug FSM cycle\n\r");
  UART_OutString("EID1= "); UART_OutString((char*)EID1); UART_OutString("\n\r");
  UART_OutString("EID2= "); UART_OutString((char*)EID2); UART_OutString("\n\r");
// initialize your FSM
  SysTick_Init();   // Initialize SysTick for software waits

  while(1){
    // 1) output depending on state using Traffic_Out
    // call your Debug_Dump logging your state number and output
    // 2) wait depending on state
    // 3) hard code this so input always shows all switches pressed
    // 4) next depends on state and input
  }
}
// use main5 to grade
int main5(void){// main5
  Clock_Init80MHz(0);
  LaunchPad_Init();
  Grader_Init();  // execute this line before your code
  Traffic_Init(); // your Lab 4 initialization
  SysTick_Init(); // Initialize SysTick for software waits
  Debug_Init();
  // initialize your FSM
  Lab4Grader(1); // activate UART, grader and interrupts
  while(1){
    // 1) output depending on state using Traffic_Out
    // call your Debug_Dump logging your state number and output
    // 2) wait depending on state
    // 3) input from switches 
    // 4) next depends on state and input
  }
}

