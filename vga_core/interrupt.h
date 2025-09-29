#ifndef INTERRUPT_H
#define INTERRUPT_H

#include <stdbool.h>
#include <xtmrctr.h>
#include <xscugic.h>
#include <xgpio.h>
#include <xil_exception.h>

#define BTN_INT XGPIO_IR_CH1_MASK

extern volatile bool TIMER_INTR_FLG;

void Timer_InterruptHandler(XTmrCtr *data, u8 TmrCtrNumber);
int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr);
int ScuGicInterrupt_Init(u16 DeviceId, XTmrCtr *TimerInstancePtr);
void BTN_Intr_Handler(void *InstancePtr);

extern XGpio BTNInst;
extern XScuGic InterruptController; /* Instance of the Interrupt Controller */
extern XScuGic_Config *GicConfig;   /* The configuration parameters of the controller */
extern int btn_value;

#endif
