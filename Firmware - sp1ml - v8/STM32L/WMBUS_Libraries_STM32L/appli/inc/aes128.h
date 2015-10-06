/******************** (C) COPYRIGHT 2008 STMicroelectronics ********************
* File Name          : aes128.h
* Author             : MCD Application Team
* Version            : V1.0.0
* Date               : 10/06/2008
* Description        : aes128 header file
********************************************************************************
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE TIME.
* AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY DIRECT,
* INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING FROM THE
* CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE CODING
* INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*******************************************************************************/

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __AES_128_H
#define __AES_128_H

/* Includes ------------------------------------------------------------------*/
#include "stm32l1xx.h"

typedef uint32_t u32;
typedef uint32_t vu32;
typedef uint8_t u8;

#define CRL_AES_ALGORITHM	2
/* Exported types ------------------------------------------------------------*/
/* Exported constants --------------------------------------------------------*/
#define AES_BLOCK_SIZE  4  /* Number of 32 bit words to store an AES128 block. */
#define AES_KEY_SIZE    4  /* Number of 32 bit words to store an AES128 key. */
#define AES_EXPKEY_SIZE 44 /* Number of 32bits words to store in an AES128 expanded key. */
                           /* The expanded key is the key after the keyschedule. */

/* Exported macro ------------------------------------------------------------*/
/* Exported functions ------------------------------------------------------- */

/* According to key computes the expanded key exp for AES128 encryption. */
void AES_keyschedule_enc(u8* key, u32* exp);

/* According to key computes the expanded key exp for AES128 decryption. */
void AES_keyschedule_dec(u8* key, u32* exp);


/* Encrypts, according to the expanded key expkey, one block of 16 bytes 
   at address 'input_pointer' into the block at address 'output_pointer'.
   They can be the same. */
void AES_encrypt(u8* input_pointer, u32* output_pointer, u32* expkey);


/* Decrypts, according to the expanded key expkey, one block of 16 bytes 
   at address 'input_pointer' into the block at address 'output_pointer'.
   They can be the same.*/
void AES_decrypt(u8* input_pointer, u32* output_pointer, u32* expkey);

uint8_t WMBus_AppliEncrypt(u8 *key, unsigned char *payload, int length);

void WMBus_AppliDecrypt(u8 *key, unsigned char *payload, int length);


#endif /* __AES_128_H */

/******************* (C) COPYRIGHT 2008 STMicroelectronics *****END OF FILE****/
