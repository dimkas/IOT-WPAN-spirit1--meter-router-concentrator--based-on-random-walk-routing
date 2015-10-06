/**
******************************************************************************
* @file    wmbus_link.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file contains header for wmbus_link.c
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
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __WMBUS_LINK_H
#define __WMBUS_LINK_H

/* Includes ------------------------------------------------------------------*/
#include "wmbus_phy.h"
#include "wmbus_crc.h"

/**
* @defgroup WMBUS_Libraries
* @{
*/
/**
* @defgroup SPIRIT_WMBus_Link WMBus Link
* @brief Configuration and management of WMBus Link layer.
* @details See the file <i>@ref wmbus_link.h</i> for more details.
* @{
*/
/**
* @defgroup SPIRIT_WMBus_Link WMBus Link Exported defines.
* @{
*/
#define LINK_TX_FCB_TABLE_ENTRIES_MAX     5/*!< Number of TX frame count bit table entries based on M and A fields */
#define LINK_ATTR_ID_START               0x65/*!< Link Layer Attribute Start index; */
#define LINK_USER_MAX_TX_DATA_FIELD_SIZE     245/*!< Link Layer Payload maximum size; */
#define LINK_AUTO_RESP_DATA_MAX              LINK_USER_MAX_TX_DATA_FIELD_SIZE/*!< Link Layer Response Payload maximum size; */
#define SIZE_MFR_ATTR                        2 /*!< MFR_ID attribute size; */
#define ADDR_ATTR_SIZE                       6/*!< Indentification, Device Type and version number attribute size; */
/**
*@}
*/

/**
* @defgroup WMBus_Link_Exported_Types Exported Types
* @{
*/
typedef uint8_t MFR_ATTR[SIZE_MFR_ATTR];/*!< MFR_ID attribute typedef; */
typedef uint8_t ADDR_ATTR[ADDR_ATTR_SIZE];/*!< Indentification, Device Type and version number attribute typedef; */
/**
*@}
*/
/**
* @defgroup WMBus_Link_Exported_Types Exported Enum Types
* @{
*/
/**
* @brief  SPIRIT1 WMBus Link Status definition.
*/
typedef enum LINK_STATUS
{
  LINK_STATUS_SUCCESS = 0x00,
  LINK_STATUS_PHY_ERROR,/*!<Indicates PHY Radio error */                
  LINK_STATUS_PHY_ERROR_TIMEOUT,/*!< Link Service is not invoked in proper time */            
  LINK_STATUS_PHY_ERROR_READ_ONLY_ADDRESS,/*!<The attribute is read only*/
  LINK_STATUS_PHY_ERROR_INVALID_ADDRESS,/*!<Link Status for invalid PHY address*/
  LINK_STATUS_PHY_ERROR_INVALID_VALUE,/*!<Link Status for invalid PHY value*/
  LINK_STATUS_PHY_ERROR_INVALID_STATE,/*!<Link Status for invalid PHY state*/
  LINK_STATUS_PHY_ERROR_INVALID_LENGTH,/*!<Frame size is not valid, length-field is smaller than the min value*/
  LINK_STATUS_PHY_ERROR_NOTHING_RECEIVED,/*!<Indicates response when frame is not received*/
  LINK_STATUS_PHY_ERROR_DECODING,/*!<Link Status for invalid decoding of PHY packet*/
  LINK_STATUS_PHY_ERROR_POR_TIMEOUT,/*!<Link Status for PHY timeout*/
  LINK_STATUS_PHY_ERROR_XTAL_TIMEOUT,/*!<Link Status for PHY XTAL timeout*/
  LINK_STATUS_PHY_ERROR_UNSUPPORTED_RADIO,/*!<Link Status for unsupported PHY Radio*/
  LINK_STATUS_PHY_ERROR_TX_TIMEOUT,/*!<Link Status for PHY transmission timeout*/
  LINK_STATUS_PHY_ERROR_RX_INVALID_LENGTH,/*!< Received frame size is not valid*/
  LINK_STATUS_PHY_ERROR_RX_PACKET_INCOMPLETE,/*!<Link Status for incomplete received packet*/
  LINK_STATUS_PHY_STATE_ERROR,/*!<Service request is unsuccessful*/
  /* LINK layer status codes */
  LINK_STATUS_INVALID_SIZE = 0x40,/*!<The data-field buffer is too large*/
  LINK_STATUS_INVALID_ATTR,/*!< The attribute ID is invalid*/
  LINK_STATUS_INVALID_ATTR_VAL,/*!< The attribute value is invalid.*/
  LINK_STATUS_NOT_PERMITTED,/*!<Link Status for no permission*/
  LINK_STATUS_CRC_ERROR,/*!<CRC for first block is invalid*/
  LINK_STATUS_FRAME_ERROR,/*!<There is not enough data to parse the first block */
  LINK_STATUS_TX_RETRY_ERROR,/*!<Link Status for re-transmission timeout*/
  LINK_STATUS_UNKNOWN_ERROR = 0xFF/*!<Data is not received within the timeout period specified*/
}LINK_STATUS;

/**
* @brief  SPIRIT1 WMBus Link Layer Attributes.
*/
typedef enum LINK_ATTR_ID
{
  LINK_ATTR_ID_DEV_TYPE = LINK_ATTR_ID_START,/*!< The Device Type, For details refer  WMBusDeviceType_T structure*/
  LINK_ATTR_ID_MODE,/*!< The WMBus Mode, For details refer  WMBusMode_T structure*/
  LINK_ATTR_ID_CHANNEL,/*!< The WMBus Channel, For details refer  WMBusChannel_T structure*/
  LINK_ATTR_ID_RF_POWER,/*!< The RF Transmit Power*/
  LINK_ATTR_ID_RSSI_THRESHOLD,/*!< The RSSI threshold */
  LINK_ATTR_ID_S1_PREAMBLE_LEN,/*!< The Preamble length for S1 Mode */
  LINK_ATTR_ID_S1m_PREAMBLE_LEN,/*!< The Preamble length of S1m mode */
  LINK_ATTR_ID_S2_PREAMBLE_LEN,/*!< The Preamble length of S2 Mode */
  LINK_ATTR_ID_T_PREAMBLE_LEN,/*!< The Preamble length of T mode */
  LINK_ATTR_ID_R2_PREAMBLE_LEN,/*!< The Preamble length of R2 Mode */
  LINK_ATTR_ID_N_PREAMBLE_LEN,/*!< The Preamble length of N Mode */
  LINK_ATTR_ID_C_PREAMBLE_LEN,/*!< The Preamble length of C Mode */
  
  LINK_ATTR_ID_TX_FCB,/*!< The TX Frame Control Bit, Not to be written*/
  LINK_ATTR_ID_RX_FCB,/*!< The RX Frame Control Bit, Not to be written*/
  
  LINK_ATTR_ID_S1_POSTAMBLE_LEN,/*!< The Postamble length of S1 Mode*/
  LINK_ATTR_ID_S1m_POSTAMBLE_LEN,/*!< The Postamble length for S1m mode*/
  LINK_ATTR_ID_S2_POSTAMBLE_LEN,/*!< The  Postamble length for S2 mode */
  LINK_ATTR_ID_T_POSTAMBLE_LEN,/*!< The  Postamble length for T mode*/
  LINK_ATTR_ID_R2_POSTAMBLE_LEN,/*!< The  Postamble length for R2 mode*/
  LINK_ATTR_ID_N_POSTAMBLE_LEN,/*!< The  Postamble length for N mode*/
  
  LINK_ATTR_ID_TMODE_RESPONSE_TIME_MIN,/*!< The T Mode Minimum response time */
  LINK_ATTR_ID_TMODE_RESPONSE_TIME_MAX,/*!< The T Mode Maximum response time*/
  LINK_ATTR_ID_SMODE_RESPONSE_TIME_MIN,/*!< The S Mode Minimum response time*/
  LINK_ATTR_ID_SMODE_RESPONSE_TIME_MAX,/*!< The S Mode Maximum response time*/
  LINK_ATTR_ID_RMODE_METER_RESPONSE_TIME_MIN,/*!< The R Mode meter minimum response time*/
  LINK_ATTR_ID_RMODE_METER_RESPONSE_TIME_MAX,/*!< The R Mode meter Maximum response time*/
  LINK_ATTR_ID_RMODE_OTHER_RESPONSE_TIME_MIN,/*!< The R Mode Other minimum response time*/
  LINK_ATTR_ID_RMODE_OTHER_RESPONSE_TIME_MAX,/*!< The R Mode Other Maximum response time*/
  LINK_ATTR_ID_NMODE_RESPONSE_TIME_MIN,/*!< The N Mode minimum response time*/
  LINK_ATTR_ID_NMODE_RESPONSE_TIME_MAX,/*!< The N Mode maximum response time*/
  LINK_ATTR_ID_CMODE_OTHER_RESPONSE_TIME_MIN,
  LINK_ATTR_ID_CMODE_OTHER_RESPONSE_TIME_MAX,
  LINK_ATTR_ID_CMODE_METER_RESPONSE_TIME_MIN,
  LINK_ATTR_ID_CMODE_METER_RESPONSE_TIME_MAX,
  
  LINK_ATTR_ID_NMODE_RESPONSE_TIME_TYP,/*!< The N Mode typical Response time*/
  LINK_ATTR_ID_TMODE_RESPONSE_TIME_TYP,/*!< The T Mode typical Response time*/
  LINK_ATTR_ID_SMODE_RESPONSE_TIME_TYP,/*!< The S Mode typical Response time*/
  LINK_ATTR_ID_RMODE_METER_RESPONSE_TIME_TYP,/*!< The R Mode typical Meter Response time*/
  LINK_ATTR_ID_RMODE_OTHER_RESPONSE_TIME_TYP, /*!< The R Mode typical Other Response time*/
  LINK_ATTR_ID_CMODE_RESPONSE_TIME_TYP,
  
    
  LINK_ATTR_ID_CI_FIELD,/*!< The CI-Field*/
  LINK_ATTR_ID_IDENTIFICATION,/*!< The Identification Field */
  LINK_ATTR_ID_MFR_ID,/*!< The Manufacturer ID Field*/
  LINK_ATTR_ID_METER_VERSION,/*!< The Meter Version*/
  LINK_ATTR_ID_METER_TYPE,/*!< The Meter type, for details refer EN 13757-3*/
  
  LINK_ATTR_ID_S1_TX_INTERVAL,/*!< The S1 Meter data Sent Interval*/
  LINK_ATTR_ID_S1m_TX_INTERVAL,/*!< The S1m Meter data Sent Interval*/
  LINK_ATTR_ID_S2_TX_INTERVAL,/*!< The S2 Meter data Sent Interval*/
  LINK_ATTR_ID_T1_TX_INTERVAL,/*!< The T1 Meter data Sent Interval*/
  LINK_ATTR_ID_T2_TX_INTERVAL,/*!< The T2 Meter data Sent Interval*/
  LINK_ATTR_ID_N_TX_INTERVAL,/*!< The N Meter data Sent Interval*/
  LINK_ATTR_ID_R2_TX_INTERVAL, /*!< The R2 Meter data Sent Interval*/
  LINK_ATTR_ID_RX_TIMEOUT,/*!< The Receive Timeout Power*/
  
  LINK_ATTR_ID_HW_REV,/*!< The Hardware Revision */
  LINK_ATTR_ID_SW_REV,/*!< The Firmware Revision */
  LINK_ATTR_ID_DIR,/*!< The Direction: 0- Meter to other, 1- Other to meter */
  LINK_ATTR_ID_FRAME_FORMAT,/*!< The Frame Format, for details refer FrameFormatType_t*/
  LINK_ATTR_ID_OFFSET /*!<Carrier Frequency offset*/
}  LINK_ATTR_ID;
/**
*@}
*/
/**
* @defgroup WMBus_Link_Exported_Types Exported Structure Types
* @{
*/
/**
* @brief  SPIRIT1 WMBus Link data field definition.
*/
typedef struct DataHeder
{
  uint8_t accessNum;
  uint8_t status_field;
  uint16_t config_word;
} DataHeder;

typedef struct DataField
{
  DataHeder header;
  uint8_t size;       /*!< Payload Size; */
  uint8_t capacity;   /*!< Payload buffer capacity; */
  uint8_t *payload;/*!< pointer to Payload buffer; */
} DataField;

/**
* @brief  SPIRIT1 WMBus Link layer Frame primitive definition.
*/
typedef struct
{
  uint8_t    c_field;/*!< C-field attribute;For details refer 10.5.4 of EN 13757-4:2011 */
  MFR_ATTR   m_field;/*!< C-field attribute;For details refer 10.5.5 of EN 13757-4:2011 */
  ADDR_ATTR  a_field;/*!< C-field attribute;For details refer 10.5.6 of EN 13757-4:2011 */
  uint8_t    ci_field;/*!< CI-field attribute;For details refer 10.5.8 of EN 13757-4:2011 */
  DataField  data_field;/*!< Data field; */
  float      RSSI_field;/*!< RSSI of the packet; */
}Frame;
/* Link Attributes */ 
typedef struct 
{
  WMBusDeviceType_T DevType;
  WMBusMode_T  WMBusMode;
  WMBusChannel_T WMBusChannel;
  int32_t RFPower;
  int32_t RSSIThreshold;
  uint16_t S1PreambleLen;
  uint16_t S1mPreambleLen;
  uint16_t S2PreambleLen;
  uint16_t TPreambleLen;
  uint16_t R2PreambleLen;  
  uint16_t NPreambleLen;
  uint16_t CPreambleLen;
  
  uint8_t LinkAttrTxFcb;
  uint8_t LinkAttrRxFcb;
  
  uint8_t S1PostambleLen;
  uint8_t S1mPostambleLen;
  uint8_t S2PostambleLen;
  uint8_t TPostambleLen;
  uint8_t R2PostambleLen;  
  uint8_t NPostambleLen;
  uint8_t CPostambleLen;
  
  uint16_t TRespTimeMax;
  uint16_t TRespTimeMin;
  uint16_t SRespTimeMax;
  uint16_t SRespTimeMin;
  uint16_t RMeterRespTimeMax;
  uint16_t RMeterRespTimeMin;
  uint16_t ROtherRespTimeMax;
  uint16_t ROtherRespTimeMin;  
  uint16_t NRespTimeMax;
  uint16_t NRespTimeMin;
  uint16_t CRespTimeMax;
  uint16_t CRespTimeMin;
    
  uint16_t NRespTimeTyp;  
  uint16_t TRespTimeTyp;
  uint16_t SRespTimeTyp;
  uint16_t RMeterRespTimeTyp;
  uint16_t ROtherRespTimeTyp;
  uint16_t CRespTimeTyp;
    
  uint8_t CIField;
  uint32_t Identification;
  uint16_t MFR_ID;
  uint8_t METERVersion;
  uint8_t MeterType;
  uint8_t AccessNumber;
  uint8_t Status;
  uint16_t CONFIG_WORD;
  
  uint32_t S1TXInterval;
  uint32_t S1mTXInterval;
  uint32_t S2TXInterval;
  uint32_t T1TXInterval;
  uint32_t T2TXInterval;
  uint32_t NTXInterval;  
  uint32_t R2TXInterval;
  uint32_t CTXInterval;
  
  uint32_t RxTimeout;
  
  uint8_t HardwareRev;
  uint8_t FirmwareRev;     
  volatile uint8_t Direction;
  volatile FrameFormatType_t LinkFrameFormat;
  volatile FrameFormatType_t LinkRecvFrameFormat;
  int32_t Offset;
}WMBusLinkAttributes;


/**
* @}
*/

/** @defgroup WMBus_Link_Exported_Functions        WMBus Link Exported Functions
* @{
*/
/**
* @brief  This function will init the link layer link layer attributes 
*         if valid link attributes available , otherwise the default configuration.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
extern LINK_STATUS WMBus_LinkInit(void);

/**
* @brief  This function will configure the WMBus link layer with the current Attributes
* @param  None.
* @retval LINK_STATUS:
*                       
*/
extern LINK_STATUS WMBus_LinkConfig(void);

/**
* @brief  This function will reset the WMBus link layer to Reset configuration.
* @param  param: None.
* @retval LINK_STATUS:
*                       
*/
extern LINK_STATUS WMBus_LinkReset(void);

/**
* @brief  This function will set the link attribute value.
* @param  Input Param: 1. LINK_ATTR_ID attr  - The Link attribute.
*                      2. uint32_t value   - The value of the attribute.
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*       LINK_STATUS_INVALID_ATTR   - The attribute ID is invalid.
*       LINK_STATUS_INVALID_ATTR_VAL   - The attribute value is invalid.
*       LINK_STATUS_PHY_ERROR_READ_ONLY_ADDRESS   - The attribute is read only.
*                       
*/
extern LINK_STATUS WMBus_LinkSetAttribute(LINK_ATTR_ID attr, int32_t value);

/**
* @brief  This function returns the Link layer attributes to application layer
* @param  Input Param: 1. LINK_ATTR_ID attr  - The Link attribute.
*                      2. uint32_t* value   - The value of the attribute.
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*       LINK_STATUS_INVALID_ATTR   - The attribute ID is invalid.
*                       
*/
extern LINK_STATUS WMBus_LinkGetAttribute(LINK_ATTR_ID attr, int32_t* value);

/**
* @brief  This function will turn on the radio and waits till it enters the Ready state.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
extern LINK_STATUS WMBus_LinkRadioPowerOn(void);

/**
* @brief  This function will Shut Down the radio.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
extern LINK_STATUS WMBus_LinkRadioPowerOff(void);

/**
* @brief  This function will put the radio in standby state.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
extern LINK_STATUS WMBus_LinkRadioStandby(void);

/**
* @brief  This function will receive data within the timeout period specified.If reception is
*         successful, then the function parses the received frame buffer by 
*         checking for valid data lengths, CRC, and copies the data to a frame 
*         structure.
* @param  Input Param:1. uint32_t timeout in ms.
*                     2. void (*LinkRxDataCallback)(Frame*): The callback function.
* @retval LINK_STATUS. After successfully initiating the receive request, it will 
* return LINK_STATUS_SUCCESS and after the reception the callback function is called.
*                       
*/
extern LINK_STATUS WMBus_LinkRxFrameHandlerMode(uint32_t RxTimeout, \
  void (*LinkRxDataCallback)(Frame*));

/**
* @brief  This function takes a frame input and generates a frame buffer consisting
*        of L, C, M, A, CI, Data, and CRC fields and transmits frame buffer via the PHY.
* @param  Input Param: 1. Frame* frame - A pointer to a frame structure containing
*                       C, M, A, CI, and Data fields
*                      2. void (*LSTxDataSentHandler)(LINK_STATUS): This function 
*                         is called on successful transmission.
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*                       
*/
extern LINK_STATUS WMBus_LinkTransmitFrameHandlerMode(Frame *frame, \
  void (*LinkTxDataSentHandler)(LINK_STATUS));

/**
* @brief  This is used from the physical layer in case of C mode to set the frame format.
* @param  Input FrameFormat
* @retval LINK_STATUS : LINK_STATUS_SUCCESS
*                       
*/
extern void WMBus_SetRecvFrameFormat(FrameFormatType_t FrameFormat);

/**
* @}
*/
/**
* @}
*/
/**
* @}
*/
#endif/* __WMBUS_LINK_H*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/