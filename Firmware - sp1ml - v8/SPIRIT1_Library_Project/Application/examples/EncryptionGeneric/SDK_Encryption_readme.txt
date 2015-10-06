/**
  @page AES_generic_example AES generic example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    examples/EncryptionGeneric/SDK_Encryption_readme.txt
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   Ping Pong of encrypted data example.
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

This example explains how to use the AES module.
In this example there are two programs and each one is switched between
the transmission and the reception state.
The AES key is in common and well known by the two programs.
Every time a device performs a Tx or a Rx it writes Tx/Rx data and its
decrypted versions.
As in the SDK_BasicGeneric, LED1 is toggled on a TX_DATA_DONE IRQ and LED2 is toggled on a
RX_DATA_READY IRQ.
Encryption and decryption of data are notified by an AES_END IRQ.
Moreover a Virtual Com stream can be opened by defining the preprocessing
environment variable USE_VCOM. In this case both the transmitter and
the receiver will write both their encrypted and decrypted buffers on video every
time a Tx or an Rx is performed.

@par Directory contents

  - SDK_Encryption_A.c		 A Encryption transmitter of Basic packets code
  
  - SDK_Encryption_B.c      	 B Encryption receiver of Basic packets code


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

