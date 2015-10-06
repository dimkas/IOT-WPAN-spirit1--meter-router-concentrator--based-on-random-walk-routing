/**
  @page STack_generic_example STack generic example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   STack generic example.
  ******************************************************************************
  * THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
  * TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
  ******************************************************************************
   @endverbatim

@par Example Description

This example explains how to use the STack packets with filtering.
In this example there are two programs and each one is switched between
the transmission and the reception state.
The user can modify the defines on both side in order to modify the filtering rules.
If the receiver discards the packet, it prints also the reason of this discard.
Every time A performs a transmission it toggles the LED1.
B communicates his state to the user by toggling its leds:
- LED2 to say that a reception has been correctly done.
- LED1 to say that the RX timeout has expired or packet discarded.
Every action of this kind is managed through a managed IRQ.
So, if all works correctly the user should see:
- LED1 of A at the end of each transmission.
- LED2 of B every time the packet has been received.
Moreover a Virtual Com stream can be opened by defining the preprocessing
environment variable USE_VCOM. In this case both the transmitter and
the receiver will write their own buffer on video every time a Tx or an
Rx is performed.

@par Directory contents

  - SDK_StackGeneric_A.c		A STack transmitter
  
  - SDK_StackGeneric_B.c      	 	B STack receiver


@par Hardware and Software environment


  - This example runs on STM32L1xx Ultra Low Power Medium-Density Devices.

  - This example has been tested with STMicroelectronics SDK-EVAL evaluation
    board and can be easily tailored to any other supported device and
    development board.

  - SPIRIT Set-up
    - Connect SPIRIT to the SPI connectors in the SDK-EVAL board.

@par How to use it ?

In order to make the program work, you must do the following :
 - Open your preferred toolchain and import the .c files in your workspace
 - Rebuild all files and load your image into target memory
 - Run the example

@note
- Ultra Low Power Medium-density devices are STM32L151xx and STM32L152xx
  microcontrollers where the Flash memory density ranges between 64 and 128 Kbytes.

 * <h3><center>&copy; COPYRIGHT 2015 STMicroelectronics</center></h3>
 */

