/**
  @page Basic_generic_example	Basic generic example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    examples/BasicGeneric/SDK_BasicGeneric_readme.txt
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   BASIC packets transmission/reception example.
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

This example explains how to configure one node as a transmitter and the other as a receiver
in order to do a simple basic packet transmission.
There are two devices: the device A is configured as a transmitter and the device B as a
receiver. The program consists to transmit a fixed length sequence from A and to control
if B has received them correctly.
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

  - SDK_BasicGeneric_A.c		 Transmitter code
  
  - SDK_BasicGeneric_B.c      		 Receiver code


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

 /*! @} */
