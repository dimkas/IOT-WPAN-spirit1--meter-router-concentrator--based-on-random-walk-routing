/**
******************************************************************************
* @file    wmbus_userInterface.c
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file includes routines for for interacting with 
*                       PC-GUI and Serial port using AT Commands.
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
#include "wmbus_userInterface.h"
#include "serial.h"
/** @defgroup WMBUS_USERINTERFACE
* @brief This file is the interface between WMBus appli and Phy layer
* @{
*/
#pragma pack(1)
/** @defgroup WMBUS_USERINTERFACE_Private_Variables
* @{
*/
int16_t CurrentTemp;
int16_t MaxMeasTemp = DEFAULT_UPPER_TEMP_LIMIT;
int16_t MinMeasTemp = DEFAULT_LOWER_TEMP_LIMIT;
int16_t LowerAlarmThres = DEFAULT_UPPER_TEMP_ALARM_THRES;
int16_t UpperAlarmThres = DEFAULT_LOWER_TEMP_ALARM_THRES;
uint8_t RadioSleepMode=0x00;
uint8_t RSSIControl;
uint8_t InstallControl;
uint8_t LEDControl;
uint8_t DataToEEPROM[26] = {0x00};
uint8_t DataToDelete[9]={0x00};
uint32_t EEPROM_address = 0x00;
/**
* @}
*/
/** @defgroup WMBUS_USERINTERFACE_Private_Functions
* @{
*/
/**
* @brief  This function will the reset the board.
* @param  uint8_t resetVar: This parameter indicate the reset type:-
*                           0x00 -- Reset All Parameters to default values
*                           0x01 -- Reset the board but preserve current Parameters
* @retval SpiritBool:S_TRUE-- Reset successful
*                     S_FALSE-- otherwise.
*                       
*/
SpiritBool WMBus_UIResetBoard(uint8_t resetVar)
{
  switch(resetVar)
  {
  case 0x00:
    /*Reset All*/
    break;
  case 0x01:
    /*Reset but conserve the current configuration*/
    break;    
  default:
    return S_FALSE;
  }
  return S_TRUE;
}
/**
* @brief  This function will the return the firmware version.
* @param  Param: None
* @retval uint8_t: The firmware version.
*                       
*/
uint8_t WMBus_GetFWVersion(void)
{
  int32_t value;
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_SW_REV, &value))
  {
    return (uint8_t)value;
  }
  else
  {
    return 0;
  }
}
/**
* @brief  This function will the return the hardware version of the board.
* @param  Param:None.
* @retval uint8_t: The hardware version.
*                       
*/
uint8_t WMBus_GetHWVersion(void)
{
  int32_t value;
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_HW_REV, &value))
  {
    return (uint8_t)value;
  }
  else
  {
    return 0;
  }
}
/**
* @brief  This function will return the Board type. 
* @param  Param: None.
* @retval SPIRIT_HWType_T: The board type
*                         0x00 -> SPIRIT_EVAL,
*                         0x10 -> CONCENTRATOR_EVAL,
*                         0x11 -> METER_EVAL,
*                         0xFF -> OTHER_HW.
*                       
*/
SPIRIT_HWType_T WMBus_GetHWType(void)
{
#ifdef WMBUS_METER_EVAL
  return METER_EVAL;
#elif WMBUS_CONCENTRATOR_EVAL
  return CONCENTRATOR_EVAL;
#elif WMBUS_SPIRIT_EVAL
  return SPIRIT_EVAL;
#else
  return OTHER_HW;
#endif
}


/**
* @brief  This function will return the temperature data of the onboard sensor.
* @param  uint8_t *buff:The current value and thresholds as described below:
*         |-------------|-------|------|-------------|-------------|
*         | Current Val | Mini. | Max. | Lower Alarm | Upper Alarm |
*         |   2 Bytes   |2 Bytes|2Bytes|   2 Bytes   | 2 bytes     |
*         |-------------|-------|------|-------------|-------------|
* @retval None.
*                       
*/
void WMBus_GetTempSensorData(uint8_t *buff)
{
  /* Read the temperature data here.*/
  *buff++ = (uint8_t)CurrentTemp;
  *buff++ = (uint8_t)(CurrentTemp>>8);
  
  *buff++ = (uint8_t)MinMeasTemp;
  *buff++ = (uint8_t)(MinMeasTemp>>8);
  *buff++ = (uint8_t)MaxMeasTemp;
  *buff++ = (uint8_t)(MaxMeasTemp>>8);
  
  *buff++ = (uint8_t)LowerAlarmThres;
  *buff++ = (uint8_t)(LowerAlarmThres>>8);
  *buff++ = (uint8_t)UpperAlarmThres;
  *buff++ = (uint8_t)(UpperAlarmThres>>8);
}

/**
* @brief  This function will set  the min, maximum measurable range, lower and 
*         upper alarm values.
* @param  uint8_t *buff: The buffer that contains the following 
*         |-------------|-------|------|-------------|-------------|
*         | Current Val | Mini. | Max. | Lower Alarm | Upper Alarm |
*         |   2 Bytes   |2 Bytes|2Bytes|   2 Bytes   | 2 bytes     |
*         |-------------|-------|------|-------------|-------------|
*         The first two bytes are ignored.
* @retval None.
*                       
*/
void WMBus_SetTempSensorData(uint8_t *buff)
{
  /* Ignore First Two bytes*/
  buff +=2;
  
  MinMeasTemp = *(buff+1);
  MinMeasTemp = MinMeasTemp<<8;
  MinMeasTemp |= (uint16_t)*buff;
  buff +=2;
  
  MaxMeasTemp = *(buff+1);
  MaxMeasTemp = MaxMeasTemp<<8;
  MaxMeasTemp |= (uint16_t)*buff;
  buff +=2;
  
  LowerAlarmThres = *(buff+1);
  LowerAlarmThres = LowerAlarmThres<<8;
  LowerAlarmThres |= (uint16_t)*buff;
  buff +=2;
  
  UpperAlarmThres = *(buff+1);
  UpperAlarmThres = UpperAlarmThres<<8;
  UpperAlarmThres |= (uint16_t)*buff;
  buff +=2;
}
/* WMBus Protocol Specific Commands*/


/**
* @brief  This function will set the WMBus device type.
* @param  WMBusDeviceType_T devtype: As shown below
*                     0x01 -> METER
*                     0x02 -> OTHER,
*                     0x03 -> ROUTER,
*                     0x04 -> SNIFFER, 
*                     0xFF -> NOT_CONFIGURED 
* @retval SpiritBool:The S_TRUE if command executed successfully, S_FALSE otherwise.
*                       
*/
SpiritBool WMBus_SetWMBusDeviceType(WMBusDeviceType_T devtype)
{
  uint32_t tempVal = devtype;
  if(LINK_STATUS_SUCCESS == WMBus_LinkSetAttribute(LINK_ATTR_ID_DEV_TYPE, (uint32_t)tempVal))
  {
    LinkAttrChanged = S_TRUE;
    return S_TRUE;
  }  
  return S_FALSE;
}

/**
* @brief  This function will return the WMBus mode.
* @param  Param:None.  
* @retval WMBusMode_T: As shown below:
*                     0x01 -> S1_MODE
*                     0x02 -> S1m_MODE,
*                     0x03 -> S2_MODE,
*                     0x04 -> T1_MODE, 
*                     0x05 -> T2_MODE,
*                     0x06 -> R2_MODE
*                     0x07 -> INVALID_MODE
*                       
*/
WMBusMode_T WMBus_GetWMBusMode(void)
{
  int32_t value=0x00;
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_MODE, &value))
  {
    return (WMBusMode_T)value;
  }
  return INVALID_MODE;
}

/**
* @brief  This function will set the WMBus mode.
* @param  WMBusMode_T mode: As shown below:
*                     0x01 -> S1_MODE
*                     0x02 -> S1m_MODE,
*                     0x03 -> S2_MODE,
*                     0x04 -> T1_MODE, 
*                     0x05 -> T2_MODE,
*                     0x06 -> R2_MODE
*                     0x07 -> INVALID_MODE
* @retval SpiritBool:The S_TRUE if command executed successfully, S_FALSE otherwise.
*                       
*/
SpiritBool WMBus_SetWMBusMode(WMBusMode_T mode)
{
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_MODE, (uint32_t)mode))
  {
    LinkAttrChanged = S_TRUE;
    return S_FALSE;
  }
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
WMBusChannel_T WMBus_GetWMBusChannel(void)
{
  int32_t value=0x00;
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_CHANNEL, &value))
  {
    return (WMBusChannel_T)value;
  }
  return Invalid_Channel;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetWMBusChannel(WMBusChannel_T channel)
{
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_CHANNEL, (uint32_t)channel))
  {
    LinkAttrChanged = S_TRUE;
    return S_FALSE;
  }
  return S_TRUE;
}

/**
* @brief  This function set the meter type.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetWMBusMeterType(uint8_t MeterType)
{
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_METER_TYPE, MeterType))
  {
    LinkAttrChanged = S_TRUE;
    return S_FALSE;
  }
  return S_TRUE;
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetWMBusResponseDelay(uint16_t delay)
{
  switch(WMBus_GetWMBusMode())
  {
  case S1_MODE:
  case S1m_MODE:
  case S2_MODE:
    if(LINK_STATUS_SUCCESS == WMBus_LinkSetAttribute(LINK_ATTR_ID_SMODE_RESPONSE_TIME_TYP, (uint32_t)delay))
    {
      return S_TRUE;
    }
    break;
  case T1_MODE:
  case T2_MODE:
    if(LINK_STATUS_SUCCESS == WMBus_LinkSetAttribute(LINK_ATTR_ID_TMODE_RESPONSE_TIME_TYP, (uint32_t)delay))
    {
      return S_TRUE;
    }
    break;
  case R2_MODE:
    if(METER == WMBus_GetWMBusDeviceType())
    {
      if(LINK_STATUS_SUCCESS == WMBus_LinkSetAttribute(LINK_ATTR_ID_RMODE_METER_RESPONSE_TIME_TYP, (uint32_t)delay))
      {
        return S_TRUE;
      }
    }
    else
    {
      if(LINK_STATUS_SUCCESS == WMBus_LinkSetAttribute(LINK_ATTR_ID_RMODE_OTHER_RESPONSE_TIME_TYP, (uint32_t)delay))
      {
        return S_TRUE;
      }
    }
    break;
  case N1_MODE:
  case N2_MODE:
    if(LINK_STATUS_SUCCESS == WMBus_LinkSetAttribute(LINK_ATTR_ID_NMODE_RESPONSE_TIME_TYP, (uint32_t)delay))
    {
      return S_TRUE;
    }
    break;
  default:
    return S_FALSE;
  }
  return S_FALSE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
uint16_t WMBus_GetWMBusResponseDelay(void)
{
  int32_t delay;
  switch(WMBus_GetWMBusMode())
  {
  case S1_MODE:
  case S1m_MODE:
  case S2_MODE:
    if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_SMODE_RESPONSE_TIME_TYP, &delay))
    {
      return delay;
    }
    break;
  case T1_MODE:
  case T2_MODE:
    if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_TMODE_RESPONSE_TIME_TYP, &delay))
    {
      return delay;
    }
    break;
  case R2_MODE:
    if(METER == WMBus_GetWMBusDeviceType())
    {
      if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_RMODE_METER_RESPONSE_TIME_TYP,&delay))
      {
        return delay;
      }
    }
    else
    {
      if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_RMODE_OTHER_RESPONSE_TIME_TYP, &delay))
      {
        return delay;
      }
    }
    break;
  case N1_MODE:
  case N2_MODE:
    if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_NMODE_RESPONSE_TIME_TYP, &delay))
    {
      return delay;
    }
    break;
  default:
    return 0x0000;
  }
  return 0x0000;
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetDataTimeIntervals(uint32_t txInterval, uint32_t RxTimeout)
{
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_RX_TIMEOUT, RxTimeout))
  {
    return S_FALSE;
  }
  
  switch(WMBus_GetWMBusMode())
  {
  case S1_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S1_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
  case S1m_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S1m_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
  case S2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S2_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
  case T1_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_T1_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
  case T2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_T2_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
  case R2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_R2_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
    
  case N1_MODE:
  case N2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_N_TX_INTERVAL, txInterval))
    {
      return S_FALSE;
    }
    break;
  default:
    return S_FALSE;
  }
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_GetDataTimeIntervals(uint32_t *txInterval, uint32_t *RxTimeout)
{
  int32_t value;
  if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_RX_TIMEOUT, &value))
  {
    return S_FALSE;
  }
  *RxTimeout = value;
  
  switch(WMBus_GetWMBusMode())
  {
  case S1_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S1_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
    *txInterval = value;
    break;
  case S1m_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S1m_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
     *txInterval = value;
    break;
  case S2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S2_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
     *txInterval = value;
    break;
  case T1_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_T1_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
     *txInterval = value;
    break;
  case T2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_T2_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
     *txInterval = value;
    break;
  case R2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_R2_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
     *txInterval = value;
    break;
  case N1_MODE:
  case N2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_N_TX_INTERVAL, &value))
    {
      return S_FALSE;
    }
     *txInterval = value;
    break;
  default:
    return S_FALSE;
  }
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetPreamblePostambleLen(uint16_t preambleLen, uint8_t postambleLen)
{
  preambleLen /= 2;
  postambleLen /=2;
  switch(WMBus_GetWMBusMode())
  {
  case S1_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S1_PREAMBLE_LEN, (uint32_t)preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S1_POSTAMBLE_LEN, (uint32_t)postambleLen))
    {
      return S_FALSE;
    }
    break;
  case S1m_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S1m_PREAMBLE_LEN, (uint32_t)preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S1m_POSTAMBLE_LEN, (uint32_t)postambleLen))
    {
      return S_FALSE;
    }
    break;
  case S2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S2_PREAMBLE_LEN, (uint32_t)preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_S2_POSTAMBLE_LEN, (uint32_t)postambleLen))
    {
      return S_FALSE;
    }
    break;
  case T1_MODE:
  case T2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_T_PREAMBLE_LEN, (uint32_t)preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_T_POSTAMBLE_LEN, (uint32_t)postambleLen))
    {
      return S_FALSE;
    }
    break;
  case R2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_R2_PREAMBLE_LEN, (uint32_t)preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_R2_POSTAMBLE_LEN, (uint32_t)postambleLen))
    {
      return S_FALSE;
    }
    break;
  case N1_MODE:
  case N2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_N_PREAMBLE_LEN, (uint32_t)preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_N_POSTAMBLE_LEN, (uint32_t)postambleLen))
    {
      return S_FALSE;
    }
    break;
  default:
    return S_FALSE;
  }
  LinkAttrChanged = S_TRUE;
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_GetPreamblePostambleLen(uint16_t *preamLen, uint8_t *postamLen)
{
  int32_t preambleLen, postambleLen;
  
  switch(WMBus_GetWMBusMode())
  {
  case S1_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S1_PREAMBLE_LEN, &preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S1_POSTAMBLE_LEN, &postambleLen))
    {
      return S_FALSE;
    }
    break;
  case S1m_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S1m_PREAMBLE_LEN, &preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S1m_POSTAMBLE_LEN, &postambleLen))
    {
      return S_FALSE;
    }
    break;
  case S2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S2_PREAMBLE_LEN, &preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_S2_POSTAMBLE_LEN, &postambleLen))
    {
      return S_FALSE;
    }
    break;
  case T1_MODE:
  case T2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_T_PREAMBLE_LEN, &preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_T_POSTAMBLE_LEN, &postambleLen))
    {
      return S_FALSE;
    }
    break;
  
  case R2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_R2_PREAMBLE_LEN, &preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_R2_POSTAMBLE_LEN, &postambleLen))
    {
      return S_FALSE;
    }
    break;
    
  case N1_MODE:
  case N2_MODE:
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_N_PREAMBLE_LEN, &preambleLen))
    {
      return S_FALSE;
    }
    if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_N_POSTAMBLE_LEN, &postambleLen))
    {
      return S_FALSE;
    }
    break;
    
  default:
    return S_FALSE;
  }
  *preamLen = (uint16_t)(preambleLen * 2);
  *postamLen = (uint8_t)(postambleLen * 2);
  return S_TRUE;
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_GetRFPowerdBm(float *rfPower)
{
  int32_t temp;
  if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_RF_POWER, &temp))
  {
    return S_FALSE;
  }
  *rfPower = (float)temp/10.0;
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetRFPowerdBm(float rfPower)
{
  int32_t temp = (int32_t)(rfPower*10.00);
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_RF_POWER, temp))
  {
    return S_FALSE;
  }
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_GetEncryptionKey(uint8_t *buff, uint8_t *keytype, uint8_t *keyseqnum)
{
  EncryptionKey_t keydata;
  WMBus_AppliGetEncryptKey(&keydata);
  memcpy(buff, &keydata.EnKey[0], 16);
  *keytype = keydata.KeyType;
  *keyseqnum = keydata.keySeqNum;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_SetEncryptionKey(void *mfrID, uint8_t *buff, uint8_t keytype, uint8_t keyseqnum)
{
  EncryptionKey_t keydata;  
  memcpy(&keydata.EnKey[0], buff, 16);
  keydata.KeyType = keytype;
  keydata.keySeqNum = keyseqnum;
  WMBus_AppliSetEncryptKey(mfrID, keydata);  
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_SetSerialPortParam(uint8_t *tempPtr)
{
  SerialBaudRate = *(tempPtr+3);//58
  SerialBaudRate <<=8;
  SerialBaudRate |= *(tempPtr+2);//57
  SerialBaudRate <<=8;
  SerialBaudRate |= *(tempPtr+1);//56
  SerialBaudRate <<=8;
  SerialBaudRate |= *tempPtr;//55
  *tempPtr +=4;
  
  switch(*tempPtr)
  {     
  case 0x01:
    SerialHardwareFlowControl = USART_HardwareFlowControl_RTS;
    break;
  case 0x02:
    SerialHardwareFlowControl = USART_HardwareFlowControl_CTS;
    break;
  case 0x03:
    SerialHardwareFlowControl = USART_HardwareFlowControl_RTS_CTS;
    break;
  default:
    SerialHardwareFlowControl = USART_HardwareFlowControl_None;
  }
  tempPtr++;
  
  if(0x01 == *tempPtr)
  {
    SerialParity = USART_Parity_Even;
  }
  else if(0x02 == *tempPtr)
  {
    SerialParity = USART_Parity_Odd;
  }
  else
  {
    SerialParity = USART_Parity_No;
  }
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_GetSerialPortParam(uint8_t *ATSendData)
{
  *ATSendData = (uint8_t)SerialBaudRate;
  *(ATSendData+1) = (uint8_t)(SerialBaudRate>>8);
  *(ATSendData+2) = (uint8_t)(SerialBaudRate>>16);
  *(ATSendData+3) = (uint8_t)(SerialBaudRate>>24);
  if(SerialHardwareFlowControl == USART_HardwareFlowControl_None)
  {
    *(ATSendData+4) = (uint8_t)0x00;
  }
  else if(SerialHardwareFlowControl == USART_HardwareFlowControl_RTS)
  {
    *(ATSendData+4) = (uint8_t)0x01;
  }
  else if(SerialHardwareFlowControl == USART_HardwareFlowControl_CTS)
  {
    *(ATSendData+4) = (uint8_t)0x02;
  }
  else if(SerialHardwareFlowControl == USART_HardwareFlowControl_RTS_CTS)
  {
    *(ATSendData+4) = (uint8_t)0x03;
  }
  if(SerialParity == USART_Parity_No)
  {
    *(ATSendData+5) = (uint8_t)0x00;
  }
  else if(SerialParity == USART_Parity_Even)
  {
    *(ATSendData+5) = (uint8_t)0x01;
  }
  else if(SerialParity == USART_Parity_Odd)
  {
    *(ATSendData+5) = (uint8_t)0x02;
  }
}
/**
* @brief  This function will the return the WMBus stack configuration.
* @param  Param: The generic pointer, at which the data is filled by the routine. 
* @retval None:
*                       
*/
void WMBus_GetWMBusConfig(void *vptr)
{
  uint8_t *cptr;
  uint16_t *sptr;
  uint32_t *iptr;
  int32_t *iptr2;
  float power;
  cptr = (uint8_t*)vptr;
  
  sptr = (uint16_t*)vptr;
  iptr = (uint32_t*)vptr;
  iptr2 = (int32_t*)vptr;
  *cptr = (uint8_t)WMBus_GetWMBusDeviceType();
  cptr++;
  *cptr = (uint8_t)WMBus_AppliGetOperationMode();
  cptr++;
  *cptr = (uint8_t)WMBus_GetWMBusMode();
  cptr++;
  *cptr = (uint8_t)WMBus_GetWMBusChannel();
  cptr++;
  iptr2++;
  WMBus_GetRFPowerdBm(&power);
  *iptr2 = (int32_t)((power)*10);
  iptr +=2;
  WMBus_GetDataTimeIntervals(iptr, (iptr+1));
  iptr +=2;
  WMBus_GetTime(iptr, (iptr+1));
  sptr +=12;
  *sptr++ = WMBus_GetWMBusResponseDelay();
  
  WMBus_GetPreamblePostambleLen(sptr, (cptr+24));
  
  WMBus_GetBoardMnfID(cptr+25);
  
  WMBus_GetEncryptionKey(cptr+33, (cptr+49), (cptr+50));
  WMBus_GetSerialPortParam(cptr+51); 
  
  WMBus_GetRSSILEDConfig(cptr+57);
  
  if(FRAME_FORMAT_B == WMBus_AppliGetFrameFormat())
  {
    *(cptr+57) |=0x80;
  }
  else
  {
    *(cptr+57) |=0x40;
  }
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_SetWMBusConfig(uint8_t *buff)
{
  int32_t param, param2;
  
  WMBus_SetWMBusDeviceType((WMBusDeviceType_T)*buff);
  WMBus_AppliSetOperationMode((WMBusOperationMode_t)*(buff+1));
  WMBus_SetWMBusMode((WMBusMode_T)*(buff+2));
  WMBus_SetWMBusChannel((WMBusChannel_T)*(buff+3));
  
  param = *(buff+7);
  param <<=8;
  param |= *(buff+6);
  param <<=8;
  param |= *(buff+5);
  param <<=8;
  param |= *(buff+4);
  
  WMBus_SetRFPowerdBm((float)((float)param/10.00));
  
  param = *(buff+11);
  param <<=8;
  param |= *(buff+10);
  param <<=8;
  param |= *(buff+9);
  param <<=8;
  param |= *(buff+8);
  
  param2 = *(buff+15);
  param2 <<=8;
  param2 |= *(buff+14);
  param2 <<=8;
  param2 |= *(buff+13);
  param2 <<=8;
  param2 |= *(buff+12);
  WMBus_SetDataTimeIntervals(param, param2);
  
  param = *(buff+19);
  param <<=8;
  param |= *(buff+18);
  param <<=8;
  param |= *(buff+17);
  param <<=8;
  param |= *(buff+16);
  
  param2 = *(buff+23);
  param2 <<=8;
  param2 |= *(buff+22);
  param2 <<=8;
  param2 |= *(buff+21);
  param2 <<=8;
  param2 |= *(buff+20);
  WMBus_SetTime(param, param2);
  
  param2 = *(buff+25);
  param2 <<=8;
  param2 |= *(buff+24);
  WMBus_SetWMBusResponseDelay((uint16_t)param2);
  
  param = *(buff+28);
  param2 = *(buff+27);
  param2 <<=8;
  param2 |= *(buff+26);
  
  WMBus_SetPreamblePostambleLen(param2, param);
  
  WMBus_SetBoardMnfID(buff+29);
  WMBus_SetEncryptionKey((void*)(buff+29),buff+37, *(buff+53), *(buff+54));
  WMBus_SetSerialPortParam(buff+55); 
  
  WMBus_SetRSSILEDConfig(*(buff+61));  
  if(0x80 == ((*(buff+61))&0x80))
  {
    WMBus_AppliSetFrameFormat(FRAME_FORMAT_B);
  }
  else  if(0x40 == ((*(buff+61))&0x40))
  {
    WMBus_AppliSetFrameFormat(FRAME_FORMAT_A);
  }
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_ReadSpecificMeter(void *buff, uint64_t MeterID)
{
  if(S_TRUE != WMBus_AppliReadMeter(MeterID, buff))
  {
    memset(buff,0x00, sizeof(WMBusDatabaseFrame_t));
  }
}


/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_GetRSSILEDConfig(uint8_t *val)
{
  *val = (RSSIControl?0x01:0x00);
  *val |= (InstallControl?0x02:0x00);
  *val |= (LEDControl?0x04:0x00);
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_GetRSSIThresholddBm(int *RSSIThreshold)
{
  int temp;
  if(LINK_STATUS_SUCCESS != WMBus_LinkGetAttribute(LINK_ATTR_ID_RSSI_THRESHOLD, &temp))
  {
    return S_FALSE;
  }
  *RSSIThreshold = temp;
  return S_TRUE;
}

SpiritBool WMBus_SetRSSIThresholddBm(int RSSIThreshold)
{
  
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_RSSI_THRESHOLD,RSSIThreshold))
  {
    return S_FALSE;
  }
  
  return S_TRUE;
}

/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
void WMBus_SetRSSILEDConfig(uint8_t val)
{
  RSSIControl = ((val &0x01)?0x01:0x00);
  InstallControl = ((val &0x02)?0x01:0x00);
  LEDControl = ((val &0x04)?0x01:0x00);
}
/**
* @brief  This function will Add/Delete/update meter attributes to the 
*          concentrator database
* @param  void *buff:The buffer pointer that contains meter attributes. 
* @retval uint8_t: The status of execution(0x00--> OK).
*                       
*/
uint8_t WMBus_AddUpdateMeter(void *buff)
{
  uint8_t *cptr;
  uint8_t *sptr;
  cptr = buff;  
  sptr = buff;
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
     if(0x04 == *sptr)/*Add meter*/
    {
    /*writing to eeprom the meter id and enkey added*/
      DataToEEPROM[1] = 0x01;
      DataToEEPROM[2] = ((uint8_t)*(cptr+9));
      DataToEEPROM[3] = ((uint8_t)*(cptr+8));
      DataToEEPROM[4] = ((uint8_t)*(cptr+7));
      DataToEEPROM[5] = ((uint8_t)*(cptr+6));
      DataToEEPROM[6] = ((uint8_t)*(cptr+5));
      DataToEEPROM[7] = ((uint8_t)*(cptr+4));
      DataToEEPROM[8] = ((uint8_t)*(cptr+3));
      DataToEEPROM[9] = ((uint8_t)*(cptr+2));

    cptr +=10;/* Skip Meter ID and serial number */

    cptr++;

    /*to send to eeprom*/
    memcpy(&DataToEEPROM[10],cptr,16);
    
    cptr +=16;/* Skip Meter Encryption key */
  
    cptr++;
    DataToEEPROM[0] = *cptr;

      /*writing data to eeprom*/
      WMBus_PhyWriteMeterDatabase(DataToEEPROM, 26, EEPROM_address);

    }
    else if(0x01 == *sptr)/*Delete meter*/
    {  
      DataToDelete[1] = ((uint8_t)*(cptr+9));
      DataToDelete[2] = ((uint8_t)*(cptr+8));
      DataToDelete[3] = ((uint8_t)*(cptr+7));
      DataToDelete[4] = ((uint8_t)*(cptr+6));
      DataToDelete[5] = ((uint8_t)*(cptr+5));
      DataToDelete[6] = ((uint8_t)*(cptr+4));
      DataToDelete[7] = ((uint8_t)*(cptr+3));
      DataToDelete[8] = ((uint8_t)*(cptr+2));
      DataToDelete[0] = ((uint8_t)*(cptr+10));
      /*need address to be deleted*/
      WMBus_EraseMeterDatabaseFromEEPROM(DataToDelete, EEPROM_address, 8);
      
    }
    else if(0x02 == *sptr)/*Update meter attributes*/
    {
          /*writing to eeprom the meter id and enkey added*/
      DataToEEPROM[1] = 0x01;
      DataToEEPROM[2] = ((uint8_t)*(cptr+9));
      DataToEEPROM[3] = ((uint8_t)*(cptr+8));
      DataToEEPROM[4] = ((uint8_t)*(cptr+7));
      DataToEEPROM[5] = ((uint8_t)*(cptr+6));
      DataToEEPROM[6] = ((uint8_t)*(cptr+5));
      DataToEEPROM[7] = ((uint8_t)*(cptr+4));
      DataToEEPROM[8] = ((uint8_t)*(cptr+3));
      DataToEEPROM[9] = ((uint8_t)*(cptr+2));
      cptr +=10;/* Skip Meter ID and serial number */
      cptr++;/*device type*/
      /*to send to eeprom*/
      memcpy(&DataToEEPROM[10],cptr,16);
      cptr +=16;/* Skip Meter Encryption key */
      cptr++;/*Key Type*/
      DataToEEPROM[0] = *cptr;
      
      /*writing data to eeprom*/
      WMBus_PhyWriteMeterDatabase(DataToEEPROM, 26, EEPROM_address);
    }
    else
    {
      return 0xFF;/* Invalid attributes*/
    }
  }
  return 0x00;
}

/**
* @brief  This function will update the PC GUI about the Event type.
* @param  Param:None. 
* @retval uint8_t: The Event type:
*                   0x00-> No Event
*                   0x01-> Bind Request
*                   0x02-> New data available for Sniffer
*                   0x03-> Alarming condition from Meter/Concentrator
*                       
*/
uint8_t WMBus_GetEventType(void)
{
  if(S_TRUE == bNewSnifferdataRxd)
  {
    return 0x02;
  }
  else if(S_TRUE == bNewdatareceived)
  {
    return 0x04;
  }
  else
    return 0x00;
}
/**
* @}
*/
/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/