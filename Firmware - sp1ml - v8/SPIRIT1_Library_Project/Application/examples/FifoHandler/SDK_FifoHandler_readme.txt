/**
  @page Fifo_Handler_example Fifo Handler example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    examples/FifoHandler/SDK_FifoHandler_readme.txt
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   BASIC packets with payload longer than 96 bytes transmission/reception example.
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
in order to do a basic packets transmission.
In deviation to SDK_BasicGeneric, in this example packets can have a size higher than 
the FIFO physical size (>96 bytes).
The intent is to show how to manage the ALMOST FULL and ALMOST EMPTY thresholds 
and the associated IRQs.
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
the receiver will write both their encrypted and decrypted buffers on video every
time a Tx or an Rx is performed

The preprocessor variable COLERIDGE can be used to change the payload to send. If it is defined the a famous rhime
paragraph will be sent, else the payload will be fit with the first nPayloadLength integer numbers.

@par Directory contents

  - SDK_FifoHandler_A.c			 		 Transmitter code
  
  - SDK_FifoHandler_B.c      		 		 Receiver code


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

