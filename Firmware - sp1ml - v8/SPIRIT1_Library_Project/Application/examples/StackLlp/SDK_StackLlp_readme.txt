/**
  @page STack_LLP_example STack LLP example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    examples/StackGeneric/SDK_StackLlp_readme.txt
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   STack with autoack packets transmission/reception example.
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

This examples explains how to configure one node as a transmitter and the other as a receiver
in order to do a STack packet transmission with auto-ack and auto-retransmission functions.
There are to devices: the device A is configured as a transmitter and the device B as a
receiver. The program consists to transmit a fixed length sequence from A and to control
if B has received them correctly.
Every time A performs an acknolewdged transmission (i.e. it transmits a packet and receive an ACK or ACK
with piggybacking payload) it toggles the LED1. If the max number of transmission is reached the LED2 is toggled.
B communicates his state to the user by toggling its leds:
- LED2 to say that a packet has been received
- LED1 to say that the RX timeout (set to 1 sec) has expired
Every action of this kind is managed through a managed IRQ.
So, if all works correctly the user should see:
- LED 2 of B toggling if A is transmitting.
- LED 1 of A toggling if the message has been correctly received by B.
(i.e. if it's off)
The code can also support the use of piggybacking, the user can enable this feature using the
define PIGGYBACKING at the beginning of the program (both A and B).
Moreover a Virtual Com stream can be opened by defining the preprocessing
environment variable USE_VCOM. In this case both the transmitter and
the receiver will write both their encrypted and decrypted buffers on video every
time a Tx or an Rx is performed.

@par Directory contents

  - SDK_StackLlp_A.c		 			Transmitter code
  
  - SDK_StackLlp_B.c      		                Receiver code


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

