#include "interrupt.h"

void BTN_Intr_Handler(void *InstancePtr)
{
    XGpio_InterruptDisable(&BTNInst, BTN_INT);

    if ((XGpio_InterruptGetStatus(&BTNInst) & BTN_INT) != BTN_INT)
    {
        xil_printf("No button interrupt\n");
        XGpio_InterruptEnable(&BTNInst, BTN_INT);
        return;
    }

    btn_value = XGpio_DiscreteRead(&BTNInst, 1);

    (void)XGpio_InterruptClear(&BTNInst, BTN_INT);

    XGpio_InterruptEnable(&BTNInst, BTN_INT);
}

void Timer_InterruptHandler(XTmrCtr *data, u8 TmrCtrNumber)
{
    XTmrCtr_Stop(data, TmrCtrNumber);
    XTmrCtr_Reset(data, TmrCtrNumber);
    // Update Stuff
    TIMER_INTR_FLG = true;
}

int SetUpInterruptSystem(XScuGic *XScuGicInstancePtr)
{
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_INT,
                                 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                                 XScuGicInstancePtr);
    Xil_ExceptionEnable();
    return XST_SUCCESS;
}

/*Setup all interrupts of the system*/
int ScuGicInterrupt_Init(u16 DeviceId, XTmrCtr *TimerInstancePtr)
{
    int Status;
    GicConfig = XScuGic_LookupConfig(DeviceId);
    if (NULL == GicConfig)
    {
        return XST_FAILURE;
    }
    Status = XScuGic_CfgInitialize(&InterruptController, GicConfig,
                                   GicConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    Status = SetUpInterruptSystem(&InterruptController);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }
    /*Connect a device driver handler that will be called when an interrupt for the device occurs, the device driver handler performs the specific interrupt processing for the device*/
    Status = XScuGic_Connect(&InterruptController,
                             61,
                             (Xil_ExceptionHandler)XTmrCtr_InterruptHandler,
                             (void *)TimerInstancePtr);
    if (Status != XST_SUCCESS)
    {
        return XST_FAILURE;
    }

    return XST_SUCCESS;
}
