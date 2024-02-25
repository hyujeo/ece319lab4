/* ECE319K_Lab4main.c
 * Traffic light FSM
 * ECE319H students must use pointers for next state
 * Jonathan Simon, Hyunho Jeong
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

/*
 * A: South,  r,y,g=pb2,1,0
 * X: West,   r,y,g=pb8,7,6
 * 0: Sensor, walk,south,west=pb17,16,15
 * When all true: South, Walk, West, South....
 */

uint32_t friendlyMask = 0xC4001C7;  // 1's in all the output bits

typedef struct traffic_t {
    uint32_t output;
    uint32_t wait; // *10ms
    uint32_t next[8];
} traffic_t;

#define goS 0
#define yellowS 1
#define Walk 2
#define goW 3
#define yellowW 4
#define allredS 5
#define allredW 6
#define allredWalk 7
#define RWalk1 8
#define OffWalk1 9
#define RWalk2 10
#define OffWalk2 11
traffic_t FSM[12] = {
        {0x4000101, 200, {goS, yellowS, goS, yellowS, yellowS, yellowS, yellowS, yellowS}}, // goS
        {0x4000102, 100, {allredS, allredS, allredS, allredS, allredS, allredS, allredS, allredS}}, // yellowS
        {0xC400104, 200, {RWalk1, RWalk1, RWalk1, RWalk1, RWalk1, RWalk1, RWalk1, RWalk1}}, // Walk
        {0x4000044, 200, {goW, goW, yellowW, yellowW, yellowW, yellowW, yellowW, yellowW}}, // goW
        {0x4000084, 100, {allredW, allredW, allredW, allredW, allredW, allredW, allredW, allredW}}, // yellowW
        {0x4000104, 100, {goW, goW, goS, goW, Walk, Walk, Walk, Walk}}, // allredS
        {0x4000104, 100, {goS, goW, goS, goS, Walk, Walk, Walk, goS}}, // allredW
        {0x4000104, 100, {goS, goW, goS, goS, Walk, goW, goS, goW}}, // allredWalk
        {0x4000104, 50, {OffWalk1, OffWalk1, OffWalk1, OffWalk1, OffWalk1, OffWalk1, OffWalk1, OffWalk1}}, // RWalk1
        {0x0000104, 50, {RWalk2, RWalk2, RWalk2, RWalk2, RWalk2, RWalk2, RWalk2, RWalk2}}, // OffWalk1
        {0x4000104, 50, {OffWalk2, OffWalk2, OffWalk2, OffWalk2, OffWalk2, OffWalk2, OffWalk2, OffWalk2}}, // RWalk2
        {0x0000104, 50, {allredWalk, allredWalk, allredWalk, allredWalk, allredWalk, allredWalk, allredWalk, allredWalk}} // OffWalk2
};
traffic_t *statePointer;

// initialize 6 LED outputs and 3 switch inputs
// assumes LaunchPad_Init resets and powers A and B
void Traffic_Init(void){ // assumes LaunchPad_Init resets and powers A and B
    // inputs
    IOMUX->SECCFG.PINCM[PB15INDEX] = 0x00040081; // west sensor
    IOMUX->SECCFG.PINCM[PB16INDEX] = 0x00040081; // south sensor
    IOMUX->SECCFG.PINCM[PB17INDEX] = 0x00040081; // walk sensor
    // outputs
    IOMUX->SECCFG.PINCM[PB0INDEX] = 0x81; // PB0 output
    IOMUX->SECCFG.PINCM[PB1INDEX] = 0x81; // PB1 output
    IOMUX->SECCFG.PINCM[PB2INDEX] = 0x81; // PB2 output
    IOMUX->SECCFG.PINCM[PB6INDEX] = 0x81; // PB6 output
    IOMUX->SECCFG.PINCM[PB7INDEX] = 0x81; // PB7 output
    IOMUX->SECCFG.PINCM[PB8INDEX] = 0x81; // PB8 output
    // walk light
    IOMUX->SECCFG.PINCM[PB22INDEX] = 0x81; // PB22 output
    IOMUX->SECCFG.PINCM[PB26INDEX] = 0x81; // PB26 output
    IOMUX->SECCFG.PINCM[PB27INDEX] = 0x81; // PB27 output

    // enable outputs
    GPIOB->DOE31_0 |= friendlyMask;
}

/* Activate LEDs
* Inputs: high_mask
* Output: none
* Feel free to change this. But, if you change the way it works, change the test programs too
* Be friendly*/
void Traffic_Out(uint32_t high_mask){ // walk, south, west
    // write this
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0) & (~friendlyMask); // being friendly
    GPIOB->DOUT31_0 = (GPIOB->DOUT31_0) | (high_mask); // set ports
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
int main1(void){ // main1
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
        Traffic_Out(0x00000001);
        Debug_Dump(0x00000001);
        Traffic_Out(0x00000002);
        Debug_Dump(0x00000002);
        Traffic_Out(0x00000004);
        Debug_Dump(0x00000004);
        Traffic_Out(0x00000008);
        Debug_Dump(0x00000008);
        Traffic_Out(0x00000010);
        Debug_Dump(0x00000010);
        Traffic_Out(0x00000020);
        Debug_Dump(0x00000020);
        Traffic_Out(0x04000000);
        Debug_Dump(0x04000000);
        Traffic_Out(0x0C400000);
        Debug_Dump(0x0C400000);

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
int main(void){// main4
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

    uint32_t Input;
    uint32_t index = goS;
    statePointer = &FSM[index];
    while(1){
        // 1) output depending on state using Traffic_Out
        // call your Debug_Dump logging your state number and output
        Traffic_Out(statePointer->output);
        uint32_t west_output = ((statePointer->output) & (0x000001C0))<<10;
        uint32_t south_output = ((statePointer->output) & (0x00000007)) << 8;
        uint32_t walk_output = ((statePointer->output) & (0x0C400000))>>22;
        Debug_Dump((index<<24) | west_output | south_output | walk_output);
        // 2) wait depending on state
        SysTick_Wait10ms(statePointer->wait);
        // 3) hard code this so input always shows all switches pressed
        Input = 7;
        // 4) next depends on state and input
        index = statePointer->next[Input];
        statePointer = &FSM[index];
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
