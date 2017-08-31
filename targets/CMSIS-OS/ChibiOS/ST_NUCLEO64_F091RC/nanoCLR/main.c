//
// Copyright (c) 2017 The nanoFramework project contributors
// See LICENSE file in the project root for full license information.
//

#include <ch.h>
#include <hal.h>
#include <cmsis_os.h>

#include <swo.h>
#include <targetHAL.h>
#include <CLR_Startup_Thread.h>
#include <WireProtocol_ReceiverThread.h>
#include <nanoCLR_Application.h>
#include <nanoPAL_BlockStorage.h>

// need this definition here because it depends on the specifics of the target (how many INT lines exist)
#if (HAL_USE_EXT == TRUE)
EXTConfig extInterruptsConfiguration = {
    {{EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL},
     {EXT_CH_MODE_DISABLED, NULL}}};
#endif

///////////////////////////////////////////////////////////////////////////////////////////
// RAM vector table declaration (valid for GCC only)
__IO uint32_t vectorTable[48] __attribute__((section(".RAMVectorTable")));

#define SYSCFG_MemoryRemap_SRAM     ((uint8_t)0x03)

///////////////////////////////////////////////////////////////////////////////////////////

// need to declare the Receiver thread here
osThreadDef(ReceiverThread, osPriorityNormal, 2048, "ReceiverThread");
// declare CLRStartup thread here 
osThreadDef(CLRStartupThread, osPriorityNormal, 2048, "CLRStartupThread"); 

//  Application entry point.
int main(void) {

  // HAL initialization, this also initializes the configured device drivers
  // and performs the board-specific initializations.
  halInit();

  // init SWO as soon as possible to make it available to output ASAP
  #if (SWO_OUTPUT == TRUE)  
  SwoInit();
  #endif
  
  // relocate the vector table to RAM
  // Copy the vector table from the Flash (mapped at the base of the application
  // load address) to the base address of the SRAM at 0x20000000.
  for(int i = 0; i < 48; i++)
  {
    vectorTable[i] = *(__IO uint32_t*)((uint32_t)&__nanoImage_start__ + (i<<2));
  } 

  // set CFGR1 register MEM_MODE bits value as "memory remap to SRAM"
  SYSCFG->CFGR1 |= SYSCFG_MemoryRemap_SRAM;

  // The kernel is initialized but not started yet, this means that
  // main() is executing with absolute priority but interrupts are already enabled.
  osKernelInitialize();

  // Prepares the serial driver 2 using UART2
  sdStart(&SD2, NULL);
  palSetPadMode(GPIOA, 2, PAL_MODE_ALTERNATE(1)); // USART2 TX
  palSetPadMode(GPIOA, 3, PAL_MODE_ALTERNATE(1)); // USART2 RX

  // create the receiver thread
  osThreadCreate(osThread(ReceiverThread), NULL);
  // create the CLR Startup thread 
  osThreadCreate(osThread(CLRStartupThread), NULL); 

  // EXT driver needs to be started from main   
  #if (HAL_USE_EXT == TRUE)
  extStart(&EXTD1, &extInterruptsConfiguration);
  #endif

  // start kernel, after this main() will behave like a thread with priority osPriorityNormal
  osKernelStart();

  while (true) { 
    osDelay(100);
  }
}