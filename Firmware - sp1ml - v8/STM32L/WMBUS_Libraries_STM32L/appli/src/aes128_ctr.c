/**
******************************************************************************
* @file    aes_ctr.c
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief   This file contains AES routines 
*
******************************************************************************
* @attention
*
* <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
*
* Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
* You may not use this file except in compliance with the License.
* You may obtain a copy of the License at:
*
*        http://www.st.com/software_license_agreement_liberty_v2
*
* Unless required by applicable law or agreed to in writing, software 
* distributed under the License is distributed on an "AS IS" BASIS, 
* WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
* See the License for the specific language governing permissions and
* limitations under the License.
*
*******************************************************************************/

/* Includes ------------------------------------------------------------------*/

#include "aes128.h"
#include "wmbus_appli.h"

extern uint8_t AES_InitnalizationVector_Tx[64];
extern uint8_t AES_InitnalizationVector_Rx[64];
/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

/* User's secret key */


/* Expanded key */
u32 exp_key[AES_EXPKEY_SIZE];

/* plaitext blocks */




/* ciphertext blocks for temporary buffering */
/* Don't make nonce as const because in decryption it needs to reload itself*/
u32 nonce=0x5cb44a05;  
u32 ctr[4];
u32 aes_out[4];
unsigned char remain[64];
unsigned char temp[64];
unsigned char RevBuff[255];
unsigned char buff[64];
int index, n_blocks, n_remain;

uint8_t CipherBlocklen = 0x00;
/*******************************************************************************
* Function Name  : bitwise_xor
* Description    : For Xor operation of blocks
* Input          : - input_pointer: input block a address
*                  - input_pointer: input block b address 
* Output         : output_pointer: output block address
* Return         : None
*******************************************************************************/
void bitwise_xor(unsigned char *ina, unsigned char *inb, unsigned char *out)
{
  int index;
  for (index=0; index<16; index++)
  {
    out[index] = ina[index] ^ inb[index];
  }
}





/*******************************************************************************
* Function Name  : add_counter
* Description    : It increment counter by one upon encryption of each block
* Input          : - input_pointer: counter address
* Output         : None
* Return         : None
*******************************************************************************/
void add_counter(u32 *ctr)
{
  int value;
  value = ctr[3] & 0xffffffff;
  value ++;
  ctr[3] = value & 0xffffffff;
}


/*******************************************************************************
* Function Name  : encrypt_pdu
* Description    : Encrypts the incoming stream
* Input          : - input_pointer: expended key address
*                  - input_pointer: input plain data address
*                  - int: length
* Output         : output_pointer: output cipher data address
* Return         : None
*******************************************************************************/
u32 encrypt_pdu(u32 *exp_key, unsigned char *plain, int len, unsigned char *cipher)
{
  int out_len = 0;
  out_len += 4;
  n_blocks = len / 16;
  n_remain = len % 16;
  
  
  
  /*Encryption starts here..................*/
  for ( index=0; index< n_blocks; index++ ) {
    AES_encrypt(AES_InitnalizationVector_Tx,(u32*)aes_out,exp_key); 
    bitwise_xor((unsigned char*)(aes_out), &plain[index*16], &cipher[index*16]);
    add_counter((uint32_t*)AES_InitnalizationVector_Tx);
    out_len += 16;
  }/*n_blocks have been encrypted, each with different counter value*/
  
  
  for ( index=0; index<16; index++ ) {
    remain[index] = 0;
  }
  
  /*Storing remaining words in remain buffer*/
  for ( index=0; index<n_remain; index++ ) {
    remain[index] = plain[n_blocks*16+index];
  }
  
  /*Encryption of the last block starts here*/
  AES_encrypt(/*(u32*)*/AES_InitnalizationVector_Tx,(u32*)aes_out,(u32*)exp_key); 
  bitwise_xor((unsigned char*)(aes_out),&remain[0], &temp[0]);
  
  /*putting remaining result into cipher buffer*/
  for ( index=0; index<n_remain; index++ ) {
    cipher[n_blocks*16+index] = temp[index];
  }
  
  out_len += n_remain;
  return nonce;
}

/*******************************************************************************
* Function Name  : decrypt_pdu
* Description    : Decrypts the incoming stream
* Input          : - input_pointer: expended key address
*                  - input_pointer: input cipher data address
*                  - int: length
* Output         : output_pointer: output plain data address
* Return         : None
*******************************************************************************/
u32 decrypt_pdu(u32 *exp_key, unsigned char *cipher,int len, unsigned char *plain )
{
  int out_len = 0;
  
  /*Getting the value of Nonce by capturing first word from incoming data*/
  n_blocks = len / 16;
  n_remain = len % 16;
  
  
  /*Decryption starts here..................*/
  for ( index=0; index< n_blocks; index++ ) {
    AES_encrypt(AES_InitnalizationVector_Rx,(u32*)aes_out,(u32*)exp_key); 
    bitwise_xor((unsigned char*)(aes_out), &cipher[index*16], &plain[index*16]);
    add_counter((u32*)AES_InitnalizationVector_Rx);
    out_len += 16;
  }
  
  
  for ( index=0; index<16; index++ ) {
    remain[index] = 0;
  }
  
  /*Storing remaining words in remain buffer*/
  for ( index=0; index<n_remain; index++ ) {
    remain[index] = cipher[n_blocks*16+index];
  }
  
  /*Decryption of the last block starts here*/
  AES_encrypt(AES_InitnalizationVector_Rx,(u32*)aes_out,(u32*)exp_key);  
  
  /*Notice that AES_encrypt is called for decryption instead of AES_decrypt*/
  bitwise_xor((unsigned char*)(aes_out),&remain[0], &temp[0]);
  
  /*putting remaining result into plain buffer*/
  for ( index=0; index<n_remain; index++ ) {
    plain[n_blocks*16+index] = temp[index];
  }
  out_len += n_remain;
  return nonce;
}




/*******************************************************************************
* Function Name  : decrypt_pdu_cbc
* Description    : Decrypts the incoming stream
* Input          : - input_pointer: expended key address
*                  - input_pointer: input cipher data address
*                  - int: length
* Output         : output_pointer: output plain data address
* Return         : None
*******************************************************************************/
void decrypt_pdu_cbc(u32 *exp_key, unsigned char *cipher,int len, unsigned char *plain)
{
  n_blocks = len / 16;
  n_remain = len % 16;
  uint8_t tempbuff[64];
  
  /*Decryption starts here..................*/
  memcpy( RevBuff, cipher, len );
  for ( index=0; index<n_blocks; index++ )
  {
    for(uint8_t i=0; i<16; i++)
    {
      buff[i] = RevBuff[(index*16)+i];
    }
    AES_decrypt(buff,(u32*)aes_out,exp_key);
    memcpy( tempbuff, aes_out, 16 );    
    for(uint8_t i =0; i<16; i++)
    {
      for(uint8_t ctr =0; ctr<4;ctr++)
      {
        temp[ctr+i*4] =   tempbuff[(3-ctr)+(i*4)];
      }
    }
    
    bitwise_xor(temp,&AES_InitnalizationVector_Rx[0], &plain[index*16]);
    memcpy( AES_InitnalizationVector_Rx, buff, 16 );
  }/*decription of n_blocks completed*/
}

/*******************************************************************************
* Function Name  : encrypt_pdu_cbc
* Description    : Encrypts the incoming stream
* Input          : - input_pointer: expended key address
*                  - input_pointer: input plain data address
*                  - int: length
* Output         : output_pointer: output cipher data address
* Return         : None
*******************************************************************************/
void encrypt_pdu_cbc(u32 *exp_key, unsigned char *plain, int len, unsigned char *cipher)
{
  n_blocks = len / 16;
  n_remain = len % 16;
  uint8_t tempbuff[64];
  CipherBlocklen = 0x00;
  
  /*Encryption starts here..................*/
  for (index=0; index< n_blocks; index++ ) 
  {
    bitwise_xor((unsigned char*)AES_InitnalizationVector_Tx, &plain[index*16], &temp[0] );
    AES_encrypt(temp,aes_out,exp_key); 
    memcpy( tempbuff, aes_out, 16 );
    
    for(uint8_t i =0; i<16; i++)
    {
      for(uint8_t ctr =0; ctr<4;ctr++)
      {
        AES_InitnalizationVector_Tx[ctr+i*4] =   tempbuff[(3-ctr)+(i*4)];
      }
    }
    
    for(uint8_t i=0; i<16; i++)
    {
      cipher[(index*16)+i] = AES_InitnalizationVector_Tx[i];
    }
    CipherBlocklen +=16;
  }/*n_blocks have been encrypted*/ 
  if(n_remain!=0)
  {
    for ( index=0; index<16; index++ ) {
      remain[index] = 0;
    }
    
    /*Storing remaining words in remain buffer*/
    for ( index=0; index<n_remain; index++ ) 
    {
      remain[index] = plain[n_blocks*16+index];
    }
    if(n_remain < 16)
    {
      for ( index=n_remain; index<16; index++ ) 
      {
        remain[index] = 0x2F;
      }
    }
    
    /*Encryption of the last block starts here*/ 
    bitwise_xor((unsigned char*)AES_InitnalizationVector_Tx, &remain[0], &temp[0] );
    AES_encrypt(temp,(u32*)aes_out,(u32*)exp_key);
    /*putting remaining result into cipher buffer*/
    memcpy( tempbuff, aes_out, 16 );
    
    for(uint8_t i =0; i<16; i++)
    {
      for(uint8_t ctr =0; ctr<4;ctr++)
      {
        cipher[ctr+i*4] =   tempbuff[(3-ctr)+(i*4)];
      }
    }
    
    CipherBlocklen += 16;    
  }
}

/*******************************************************************************
* Function Name  : WMBus_AppliEncrypt
* Description    : Does key expension before encryption starts
*                  and put the data stream back into payload buffer
* Input          : - input_pointer: input payload address
*                  - input pointer: encryption key 
*                  - int: length
* Output         : None
* Return         : None
*******************************************************************************/
uint8_t WMBus_AppliEncrypt(u8 *key, unsigned char *payload, int length)
{
  /*******************************************************/
  /*                   AES CTR ENCRYPTION                */
  /*******************************************************/       
  
  /* Encryption key scheduling, to be done once */
  AES_keyschedule_enc((u8*)key,(u32*)exp_key); 
  /* encryption starts here*/
  encrypt_pdu_cbc((u32*)exp_key,payload,length,payload); 
  return(CipherBlocklen);
}

/*******************************************************************************
* Function Name  : WMBus_AppliDecrypt
* Description    : Does key expension before decryption starts
*                  and put the data stream back into payload buffer
* Input          : - input_pointer: input payload address
*                  - input pointer: encryption key 
*                  - int: length
* Output         : None
* Return         : None
*******************************************************************************/
void WMBus_AppliDecrypt(u8 *key, unsigned char *payload, int length)
{
  
  /*******************************************************/
  /*                   AES CTR DECRYPTION                */
  /*******************************************************/
  
  /* Decryption key scheduling, to be done once */
  AES_keyschedule_dec((u8*)key,(u32*)exp_key);
  /* decryption */
  decrypt_pdu_cbc((u32*)exp_key,payload,length,payload);
}
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
