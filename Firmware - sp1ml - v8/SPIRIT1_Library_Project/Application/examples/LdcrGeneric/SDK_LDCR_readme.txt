/**
  @page LDCR_example LDCR example readme

  @verbatim
  ******************** (C) COPYRIGHT 2015 STMicroelectronics *******************
  * @file    examples/LdcrGeneric/SDK_LDCR_readme.txt
  * @author  VMA division - AMS
  * @version 3.2.0
  * @date    February 1, 2015
  * @brief   BASIC packets transmission/reception in LDCR mode.
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

Here is used the embedded LDCR feature of Spirit.
Two different programs are used for the transmitter (device A) and the receiver
(device B).
In the program A the wake up timer is configured and, since the embedded LDCR system
automatically  manages the transmission strobe, only a FIFO reload has to be done on
the TX done IRQ.
On the receiver side (program B), there are some important notes:
 1. At the beginning of the communication the device is always in continous Rx (infinite Rx timeout).
 2. On the first reception the Rx timeout has to be set to be a not infinite value.
 3. Since the embedded LDCR system automatically manages the reception strobe the only thing to do is a Rx FIFO reading on the RX_DATA_READY interrupt.

Due to clock mismatches, the devices must be synchronized in order to ensure the matching between Tx and Rx active
slots and avoid that the Tx transmits out of the Rx active slot.
Infact, even if the Tx and Rx wake up timers are nominally the same, they're misured on different time
bases (considering the RC oscillators of the two devices that are the not the same because of tolerances).
This means that there must exist a way to synchronize the devices in order to have not loss of data.
A simple way to do that is reload the wakeup timer at the sync event on the Rx device.

 @image html "$PROJ_DIR$\\..\Images\ldcr1.png" "LDCR time scheme with synchro mechanism disabled"

 @image html "$PROJ_DIR$\\..\Images\ldcr2.png" "LDCR time scheme with synchro mechanism enabled"

In this example the auto reload on SYNC mechanism is enabled, so the Wake Up Timer Reload value is automatically
reloaded at Sync detection in order to synchronize the two devices.

A and B communicates their state to the user through its leds.
For device A:
- LED1 toggle to say that the first packet has been transmitted.

For device B:
- LED1 on to say that the first reception happened.
- LED2 toggle to say that a packet has been correctly received.
- LED3 toggle to say that a packet has been discarded.



@par Directory contents

  - SDK_LDCR_A.c		Transmitter code
  
  - SDK_LDCR_B.c      		Receiver code


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

