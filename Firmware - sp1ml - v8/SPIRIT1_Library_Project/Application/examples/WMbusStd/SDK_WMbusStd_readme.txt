/**
  @page WMBUS_Standard_example WMBUS Standard example readme
  
  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    examples/WMbusStd/SDK_WMbusStd_readme.txt
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   Standard WMBUS packets transmission/reception example.
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

This is a demo implementation of the standard WMBUS protocol. It takes in account the physical and data link layers
defined by the standard.
The example shows how to configure the packets in order to transmit or receive MBUS packets (format A only).

Since the Spirit1 supports the S1, S2, T1, T2 and R2 physical submodes but not the N1, N2 two different implementation
approaches have been used for the physical layer.

The preprocessor variable SUBMODE_N is used the the code in order to manage the N submode. If this variable is undefined
the configured packet is the native Spirit WMBUS and the user can select the submode using the MbusInit structure.
If the SUBMODE_N variable is defined workarounds are used: on the Tx side, the Direct Tx mode is configured and the
packet is transmitted with the standard preamble (0x5555) and the standard sync word (0xF68D) at the beginning and the other
fields after.
On the Rx side, the Basic packet is configured to have 2 bytes of preamble and the 2 bytes of sync 0xF68D, so the reception
can be managed as the one of a normal basic packet.

Regarding the data-link layer all the fields defined by the protocol are inserted into the payload of the packets and the CRC
blocks computation is managed by firmware.
Moreover, on the Rx side the algorithm uses the L-field of the MBUS packet to correctly stop the reception making use of
the FIFO threshold IRQs.

The radio configuration according to the standards has to be totally managed by the user.

A Virtual Com stream can be opened by defining the preprocessing
environment variable USE_VCOM. In this case both the transmitter and
the receiver will write both their encrypted and decrypted buffers on video every
time a Tx or an Rx is performed.


@par Directory contents

  - SDK_WMbusStd_A.c		        Transmitter code
  
  - SDK_WMbusStd_B.c      		Receiver code


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

