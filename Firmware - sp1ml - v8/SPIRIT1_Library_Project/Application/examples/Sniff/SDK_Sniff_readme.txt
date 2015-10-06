/**
  @page Sniff_example Sniff example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   Sniff mode example.
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
in order to use Spirit in sniff mode.
There are to devices: the device A is configured as a transmitter and the device B as a
receiver. The program consists to transmit a fixed length sequence from A to B using a low power mode
on B. This low power mode keep B in sleep as much time as possible and wake up it for a very short time
in order to sense the power in the air with an RSSI measurement. Thus, if a packet is in the air,
B will be able to sense the carrier during the preamble and to wait for a sync in this case.

Every time A performs a transmission it toggles the LED1.
B communicates his state to the user by toggling its leds:
- LED2 to say that a reception has been done
- LED4 to say that a sync word has been detected
Every action of this kind is managed through a managed IRQ.
So, if all works correctly the user should see: 
- LED 1 of A at each transmission.
- LEDs 4 of B toggling almost simultaneously if A is transmitting.
Moreover a Virtual Com stream can be opened by defining the preprocessing
environment variable USE_VCOM. In this case both the transmitter and
the receiver will write their own buffer on video every time a Tx or an
Rx is performed.
In the code of SDK_Sniff_A, the define SYNC_WRONG_ON_PURPOSE can be used
to simulate a SYNC error on B (every 5 transmitted frames) causing the timer to expire
in order to recover the IDLE low power state.

@par Directory contents

  - SDK_Sniff_A.c		A packet transmitter
  
  - SDK_Sniff_B.c      	 	B receiver in sniff mode


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

