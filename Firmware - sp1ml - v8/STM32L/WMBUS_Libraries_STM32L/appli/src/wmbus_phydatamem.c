/**
******************************************************************************
* @file    wmbus_phydatamem.c
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  The source file EEPROM data read/write.
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
#include "wmbus_phydatamem.h"
#include "wmbus_appli.h"

uint8_t EEpromMtrData[26] = {0x00};
uint8_t METER_Enkey_DB[26] = {0x00};
uint64_t EEPROM_MTR_ID = 0x00;
extern EncryptionKey_t EncryptionKey;
/** @defgroup WMBUS_PHYDATAMEM
* @brief This file is the interface between WMBus Phy and EEPROM
* @{
*/

/** @defgroup WMBUS_PHYDATAMEM_Private_Functions
* @{
*/
/**
* @brief  This function writes data to internal EEPROM.
* @param  uint8_t *NVMparam: The pointer to the data to be written.
*         - uint8_t length: The length of the data to be written.  
* @retval None.
*/
void WMBus_PhyWriteNVMParam(uint8_t *NVMparam, uint8_t length)
{
  DATA_EEPROM_Unlock();
  for(uint8_t temp=0x00; temp<length;temp++)
    DATA_EEPROM_ProgramByte(EEPROM_NVM_BASE+temp, *(NVMparam+temp));
  DATA_EEPROM_Lock();
}
/**
* @brief  This function will read the data from the EEPROM.
* @param  uint8_t *NVMparam: The pointer to the data buffer.
*         - uint8_t length: The length of the data to be read. 
* @retval None
*/
void WMBus_PhyReadNVMParam(uint8_t *NVMparam, uint8_t length)
{
  memcpy(NVMparam, (const uint32_t*)EEPROM_NVM_BASE, length);
}
/**
* @brief  This function will erase the memory region.
* @param  uint32_t *NVMparam: The pointer to the memory region.
*         uint8_t Wordlength: The word length.
* @retval None
*/
void WMBus_PhyEraseNVMParam(uint32_t *NVMparam, uint8_t Wordlength)
{
  DATA_EEPROM_Unlock();
  for(uint8_t temp=0x00; temp<Wordlength;temp++)
    DATA_EEPROM_EraseWord(((uint32_t)EEPROM_NVM_BASE)+temp);
  DATA_EEPROM_Lock();
}
/**
* @brief  This function writes data to internal EEPROM.
* @param  uint8_t *buff: The pointer to the data to be written.
*         - uint16_t length: The length of the data to be written.  
* @retval None.
*/
void WMBus_PhyWriteMeterDatabase(uint8_t *buff, uint16_t length, uint32_t Saddress)
{
  for(uint8_t eindex=0; eindex<10; eindex++)
  {
    switch(eindex)
    {
    case 0:
      WMBus_PhyReadMeterDatabase(EEPROM_METER1_DATABASE_START,26, EEPROM_METER1_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER1_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER1_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 1:
      WMBus_PhyReadMeterDatabase(EEPROM_METER2_DATABASE_START, 26, EEPROM_METER2_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER2_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER2_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 2:
      WMBus_PhyReadMeterDatabase(EEPROM_METER3_DATABASE_START, 26, EEPROM_METER3_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER3_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER3_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 3:
      WMBus_PhyReadMeterDatabase(EEPROM_METER4_DATABASE_START, 26, EEPROM_METER4_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER4_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER4_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 4:
      WMBus_PhyReadMeterDatabase(EEPROM_METER5_DATABASE_START, 26, EEPROM_METER5_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER5_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER5_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 5:
      WMBus_PhyReadMeterDatabase(EEPROM_METER6_DATABASE_START, 26, EEPROM_METER6_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER6_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER6_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 6:
      WMBus_PhyReadMeterDatabase(EEPROM_METER7_DATABASE_START, 26, EEPROM_METER7_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER7_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER7_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }      
      break;    
    case 7:
      WMBus_PhyReadMeterDatabase(EEPROM_METER8_DATABASE_START, 26, EEPROM_METER8_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER8_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER8_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }      
      break;
    case 8:
      WMBus_PhyReadMeterDatabase(EEPROM_METER9_DATABASE_START, 26, EEPROM_METER9_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER9_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER9_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }      
      break;
    case 9:
      WMBus_PhyReadMeterDatabase(EEPROM_METER_10_DATABASE_START, 26, EEPROM_METER_10_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER_10_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }
      if((!EEpromMtrData[0])&&(!EEpromMtrData[1]))
      {
        Saddress = EEPROM_METER_10_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint16_t temp=0x00; temp<length;temp++)
          DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
        DATA_EEPROM_Lock();
        return;
      }      
      break;      
    default:
      break;
    }
  }
}
/**
* @brief  This function will read the data from the EEPROM.
* @param  uint8_t *buff: The pointer to the data buffer.
*         - uint16_t length: The length of the data to be read. 
* @retval SpiritBool: S_TRUE: Successful Read
*                     S_FALSE: Reading beyond the EEPROM area.
*/
uint8_t* WMBus_PhyReadMeterDatabase(uint32_t Saddress, uint16_t length, uint32_t Eaddress)
{
  if((Saddress+length)>=Eaddress)
  {
    return 0;
  }
  else
  {
    memcpy(&EEpromMtrData[0], (const uint32_t*)(Saddress), length);
    if(EEpromMtrData[1]==0x01)
    {
      EEPROM_MTR_ID = EEpromMtrData[2];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[3];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[4];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[5];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[6];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[7];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[8];
      EEPROM_MTR_ID = (EEPROM_MTR_ID<<8) | EEpromMtrData[9];
      
    }
    else
    {
      EEPROM_MTR_ID = 0x00;
    }
    return (&EEpromMtrData[0]);
  }
}
/**
* @brief  This function writes data to internal EEPROM.
* @param  uint8_t *buff: The pointer to the data to be written.
*         - uint16_t length: The length of the data to be written.  
* @retval None.
*/
void WMBus_WriteMeterDatabaseToEEPROM(uint8_t *buff, uint16_t length, uint32_t Saddress)
{
  DATA_EEPROM_Unlock();
  for(uint16_t temp=0x00; temp<length;temp++)
    DATA_EEPROM_ProgramByte(Saddress+temp, *(buff+temp));
  DATA_EEPROM_Lock();
}
/**
* @brief  This function will read the data from the EEPROM.
* @param  uint8_t *buff: The pointer to the data buffer.
*         - uint16_t length: The length of the data to be read. 
* @retval SpiritBool: S_TRUE: Successful Read
*                     S_FALSE: Reading beyond the EEPROM area.
*/
uint8_t* WMBus_ReadMeterEnkeyFromEEPROM(uint32_t Saddress, uint16_t length, uint32_t Eaddress)
{
  if((Saddress+length)>=Eaddress)
  {
    return 0;
  }
  else
  {
    memcpy(METER_Enkey_DB, (const uint32_t*)(Saddress), length);
    return (&METER_Enkey_DB[0]);
  }
}

/**
* @brief  This function will erase the memory region.
* @param  uint32_t Saddress: The strating address to perform action on.
*         uint8_t length: The data length to be deleted.
* @retval None
*/
void WMBus_EraseMeterDatabaseFromEEPROM1(uint32_t Saddress, uint8_t Wordlength)
{
  DATA_EEPROM_Unlock();
  for(uint8_t temp=0x00; temp<Wordlength;temp++)
  {
    DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
  }
  
  DATA_EEPROM_Lock();
}
/**
* @brief  This function will erase the memory region.
* @param  uint32_t Saddress: The strating address to perform action on.
*         uint8_t length: The data length to be deleted.
* @retval None
*/
void WMBus_EraseMeterDatabaseFromEEPROM(uint8_t *buff, uint32_t Saddress, uint8_t Wordlength)
{
  for(uint8_t eeindex=0; eeindex<10; eeindex++)
  {
    switch(eeindex)
    {
    case 0:
      WMBus_PhyReadMeterDatabase(EEPROM_METER1_DATABASE_START,26, EEPROM_METER1_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER1_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 1:
      WMBus_PhyReadMeterDatabase(EEPROM_METER2_DATABASE_START, 26, EEPROM_METER2_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER2_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 2:
      WMBus_PhyReadMeterDatabase(EEPROM_METER3_DATABASE_START, 26, EEPROM_METER3_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER3_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 3:
      WMBus_PhyReadMeterDatabase(EEPROM_METER4_DATABASE_START, 26, EEPROM_METER4_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER4_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 4:
      WMBus_PhyReadMeterDatabase(EEPROM_METER5_DATABASE_START, 26, EEPROM_METER5_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER5_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 5:
      WMBus_PhyReadMeterDatabase(EEPROM_METER6_DATABASE_START, 26, EEPROM_METER6_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER6_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 6:
      WMBus_PhyReadMeterDatabase(EEPROM_METER7_DATABASE_START, 26, EEPROM_METER7_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER7_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 7:
      WMBus_PhyReadMeterDatabase(EEPROM_METER8_DATABASE_START, 26, EEPROM_METER8_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER8_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 8:
      WMBus_PhyReadMeterDatabase(EEPROM_METER9_DATABASE_START, 26, EEPROM_METER9_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER9_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;
    case 9:
      WMBus_PhyReadMeterDatabase(EEPROM_METER_10_DATABASE_START, 26, EEPROM_METER_10_DATABASE_END);
      if(EEpromMtrData[0] == buff[0])
      {
        Saddress = EEPROM_METER_10_DATABASE_START;
        DATA_EEPROM_Unlock();
        for(uint8_t temp=0x00; temp<Wordlength;temp++)
        {
          DATA_EEPROM_EraseWord(((uint32_t)Saddress)+(temp*4));
        }
        DATA_EEPROM_Lock();
        return;
      }
      break;      
    default:
      break;
    }
  }
}
/**
* @brief  This function will read the memory region to obtain correct encryption 
          key corresponding to the meter id received.
* @param  uint8_t* buff: ptr to the receive meter id i.e. Rev_METER_ID.
* @retval None
*/
void Read_EEPROM_Mtr_DataBase(uint64_t buff)
{
      for(uint8_t eeindex=0; eeindex<10; eeindex++)
      {
        switch(eeindex)
        {
        case 0:
          WMBus_PhyReadMeterDatabase(EEPROM_METER1_DATABASE_START,26, EEPROM_METER1_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 1:
          WMBus_PhyReadMeterDatabase(EEPROM_METER2_DATABASE_START, 26, EEPROM_METER2_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 2:
          WMBus_PhyReadMeterDatabase(EEPROM_METER3_DATABASE_START, 26, EEPROM_METER3_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 3:
          WMBus_PhyReadMeterDatabase(EEPROM_METER4_DATABASE_START, 26, EEPROM_METER4_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 4:
          WMBus_PhyReadMeterDatabase(EEPROM_METER5_DATABASE_START, 26, EEPROM_METER5_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 5:
          WMBus_PhyReadMeterDatabase(EEPROM_METER6_DATABASE_START, 26, EEPROM_METER6_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 6:
          WMBus_PhyReadMeterDatabase(EEPROM_METER7_DATABASE_START, 26, EEPROM_METER7_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 7:
          WMBus_PhyReadMeterDatabase(EEPROM_METER8_DATABASE_START, 26, EEPROM_METER8_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 8:
          WMBus_PhyReadMeterDatabase(EEPROM_METER9_DATABASE_START, 26, EEPROM_METER9_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;
        case 9:
          WMBus_PhyReadMeterDatabase(EEPROM_METER_10_DATABASE_START, 26, EEPROM_METER_10_DATABASE_END);
          if(EEPROM_MTR_ID == buff)
          {
            for(uint8_t ctr=0; ctr<16; ctr++)
            {
              EncryptionKey.EnKey[ctr] = EEpromMtrData[ctr+10];
            }
          }
          break;  
        default:
          break;
        }
      }
}
/**
* @}
*/

/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
