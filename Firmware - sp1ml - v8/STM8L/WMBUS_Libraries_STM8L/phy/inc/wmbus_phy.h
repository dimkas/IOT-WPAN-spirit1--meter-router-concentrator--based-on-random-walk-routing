/**
******************************************************************************
* @file    wmbus_phy.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file contains header for wmbus_phy.c
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
#ifndef __WMBUS_PHY_H
#define __WMBUS_PHY_H
/* Includes ------------------------------------------------------------------*/
#include "SPIRIT_Config.h"
#include <string.h>  

/**
* @defgroup WMBUS_Libraries
* @{
*/
/**
* @defgroup SPIRIT_WMBus_Phy WMBus PHY
* @brief Configuration and management of WMBus Phy layer.
* @details See the file <i>@ref wmbus_phy.h</i> for more details.
* @{
*/
/**
* @defgroup SPIRIT_WMBus_Phy WMBus PHY Exported defines
* @{
*/
#define  MIN(a,b)                       ((a>b)?b:a)
#define  MAX(a,b)                       ((a>b)?a:b)
#define  LSB_U16(NUM)        ((uint8_t)(NUM &0xFF)) 
#define  MSB_U16(NUM)   ((uint8_t)((NUM>>8) &0xFF)) 
#define CEILING(X)      ((X-(uint16_t)(X)) > 0 ? (uint16_t)(X+1) : (uint16_t)(X))

#define TX_STATUS               uint8_t
#define RX_STATUS               uint8_t
#define PHY_STATUS              uint8_t
#define PHY_CONFIG_DELAY              2
#define PHY_BUFF_MAX_LENGTH         128//320//255 /*including crc*/

/*  Radio configuration parameters  */
#define	SUBMODE0_BASE_FREQUENCY                 868.3e6         /* Submode S1, S2(long header, mbus_submode=0): */
#define	SUBMODE1_BASE_FREQUENCY                 868.3e6         /*Submode S1-m, S2, T2 (other to meter) (mbus_submode=1) */
#define	SUBMODE3_BASE_FREQUENCY                 868.95e6        /*Submode T1, T2 (meter to other)  (mbus_submode=3) */
#define	SUBMODE5_BASE_FREQUENCY                 868.03e6        /*Submode R2, short header (mbus_submode=5) */
#define N_MODE_BASE_FREQUENCY                   169406250       /*Submode N1 and N2 (done by sw) */
#define C1C2m2o_MODE_BASE_FREQUENCY             868950000       /*Submode C1 or C2 m2o (done by sw) */
#define C2o2m_MODE_BASE_FREQUENCY               869525000       /*Submode C2 o2m (done by sw) */

#define SMODE_TMODE_CHANNEL_SPACE            100e3
#define R2MODE_CHANNEL_SPACE                  10e3
#define N_MODE_CHANNEL_SPACE                 12500

#define SMODE_TMODE_CHANNEL_NUMBER              0
#define S_T_R_MODE_MODULATION_SCHEME         FSK
#define N_MODE_MODULATION_SCHEME            GFSK_BT05
#define C1C2m2o_MODE_MODULATION_SCHEME      FSK      
#define C2o2m_MODE_MODULATION_SCHEME         GFSK_BT05   

#define SUBMODE0_FREQ_DEV                     50e3
#define SUBMODE1_FREQ_DEV                     50e3
#define SUBMODE3_FREQ_DEV                     50e3
#define SUBMODE5_FREQ_DEV                    6.0e3
#define N_AB_EF_MODE_FREQ_DEV                1200
#define N_CD_MODE_FREQ_DEV                   2400
#define C1C2m2o_MODE_FREQ_DEV                 45000
#define C2o2m_MODE_FREQ_DEV                   25000

#define SUBMODE0_BANDWIDTH                   150e3
#define SUBMODE1_BANDWIDTH                   150e3
#define SUBMODE3_BANDWIDTH                   250e3
#define SUBMODE5_BANDWIDTH                   20.0e3
#define N_AB_EF_MODE_BANDWIDTH               8000
#define N_CD_MODE_BANDWIDTH                  8000
#define C1C2m2o_MODE_BANDWIDTH               200000
#define C2o2m_MODE_BANDWIDTH                 120000 

#define N_MODE_SYNC_LENGTH                 PKT_SYNC_LENGTH_2BYTES
#define N_MODE_SYNC_WORD                   0xF68D0000
#define C_MODE_SYNC_LENGTH                 PKT_SYNC_LENGTH_3BYTES
#define C_MODE_SYNC_WORD                   0x543d5400


#define S1_PREAMB_CHIPCOMB_LEN_MIN      (uint16_t)288
#define S1m_PREAMB_CHIPCOMB_LEN_MIN      (uint16_t)24
#define S2_PREAMB_CHIPCOMB_LEN_MIN       (uint16_t)24
#define T_PREAMB_CHIPCOMB_LEN_MIN        (uint16_t)24
#define R2_PREAMB_CHIPCOMB_LEN_MIN       (uint16_t)48
#define POSTABMLE_MAX_LEN               (uint8_t)8
#define N_PREAMB_CHIPCOMB_LEN_MIN       (uint16_t)8
#define N_PREAMB_CHIPCOMB_LEN_MAX       (uint16_t)8

#define SUBMODE0_DATARATE                    32768
#define SUBMODE1_DATARATE                    32768
#define SUBMODE3_DATARATE                   100000
#define SUBMODE5_DATARATE                     4800
#define N_AB_EF_MODE_DATARATE                 4800
#define N_CD_MODE_DATARATE                    2400
#define C1C2m2o_MODE_DATARATE               100000
#define C2o2m_MODE_DATARATE                  50000

#define PREAMBLE_LENGTH_DEFAULT                 100
#define POSTAMBLE_LENGTH_DEFAULT                4

#define DEFAULT_S1_PREAMBLE_LEN        (uint16_t)344
#define DEFAULT_S1_POSTAMBLE_LEN                4
#define DEFAULT_S1m_PREAMBLE_LEN                45
#define DEFAULT_S1m_POSTAMBLE_LEN               8
#define DEFAULT_S2_PREAMBLE_LEN                 45
#define DEFAULT_S2_PREAMBLE_LEN_L               344
#define DEFAULT_S2_POSTAMBLE_LEN                8
#define DEFAULT_T_PREAMBLE_LEN                  45
#define DEFAULT_T_POSTAMBLE_LEN                 4
#define DEFAULT_R2_PREAMBLE_LEN                 96          
#define DEFAULT_R2_POSTAMBLE_LEN                8
#define DEFAULT_N_MODE_PREAMBLE_LENGTH          PKT_PREAMBLE_LENGTH_02BYTES  
#define DEFAULT_N_MODE_POSTAMBLE_LENGTH         0
#define DEFAULT_C_MODE_PREAMBLE_LENGTH          PKT_PREAMBLE_LENGTH_04BYTES
#define DEFAULT_C_MODE_POSTAMBLE_LENGTH         0

#define T_RESPONSE_TIME_MAX_MS                  50 /* ms*/
#define T_RESPONSE_TIME_MIN_MS                  2 /* ms*/
#define S_RESPONSE_TIME_MAX_MS                  50 /* ms*/
#define S_RESPONSE_TIME_MIN_MS                  3 /* ms*/
#define R_METER_RESPONSE_TIME_MAX_MS            10000 /* ms*/
#define R_METER_RESPONSE_TIME_MIN_MS            10 /* ms*/
#define R_OTHER_RESPONSE_TIME_MAX_MS            50 /* ms*/
#define R_OTHER_RESPONSE_TIME_MIN_MS            3  /* ms*/
#define N_RESPONSE_TIME_MAX_MS                  50 /* ms*/
#define N_RESPONSE_TIME_MIN_MS                   2 /* ms*/
#define C_SLOW_RESPONSE_TIME_MAX_MS             1000.5 /* ms*/
#define C_SLOW_RESPONSE_TIME_MIN_MS             999.5  /* ms*/
#define C_SLOW_RESPONSE_TIME_TYP_MS             1000.0 /* ms*/
#define C_FAST_RESPONSE_TIME_MAX_MS             100.5 /* ms*/
#define C_FAST_RESPONSE_TIME_MIN_MS             99.5  /* ms*/
#define C_FAST_RESPONSE_TIME_TYP_MS             100.0 /* ms*/

#ifdef DEVICE_TYPE_METER
#define DEFAULT_DEVICE_TYPE          0x01 /* METER  */ 
#endif

#ifdef DEVICE_TYPE_CONCENTRATOR
#define DEFAULT_DEVICE_TYPE          0x02 /* OTHER */
#endif

#ifdef DEVICE_TYPE_ROUTER
#define DEFAULT_DEVICE_TYPE          0x03 /* ROUTER  */
#endif

#ifdef DEVICE_TYPE_SNIFFER
#define DEFAULT_DEVICE_TYPE          0x04 /* SNIFFER  */
#endif

#ifndef DEFAULT_DEVICE_TYPE
#define DEFAULT_DEVICE_TYPE          0x01 /* METER  */ 
#endif
/**
*@}
*/

/**
* @defgroup WMBus_Phy_Exported_Types Exported Types
* @{
*/

/**
*@}
*/

typedef struct
{
  uint8_t DataBuff[PHY_BUFF_MAX_LENGTH];
  uint16_t length;
  uint16_t lenRemain;
}PHY_Frame;

/**
* @brief  PHY_STATUS for WMBus Phy enumeration
* @{
*/
typedef enum {
  S1_MODE = 0x00,/*!<Transmit only meter for stationary receiving readout */
  S1m_MODE,/*!<Transmit only meter for mobile or stationary readout*/
  S2_MODE,/*!<All meter types. Stationary reading*/
  T1_MODE,/*!<Frequent transmission (short frame meters)*/
  T2_MODE,/*!<Frequent transmission (short frame meter with two way capability)*/
  R2_MODE,/*!<Frequent reception (long range)*/
  ALL_MODE,/*!<Multi-mode option */
  N1_MODE,/*!<Long range transmit for stationary readout*/
  N2_MODE,/*!<Long range two-way communication for stationary readout*/ 
  F2_MODE,/*!<Long range two-way communication for stationary readout*/
  F2m_MODE,/*!<Long range two-way communication */
  C1_MODE,/*!<Frequent transmit only meter for mobile or stationary readout*/
  C2_MODE,/*!<Frequent transmit meter for mobile or stationary readout*/
  INVALID_MODE/*!<Invalid Mode*/
}WMBusMode_T;

typedef enum{
  CHANNEL_1 = (uint8_t)0x0,/*!<Support for Channel 1*/
  CHANNEL_2,/*!<Support for Channel 2*/
  CHANNEL_3,/*!<Support for Channel 3*/
  CHANNEL_4,/*!<Support for Channel 4*/
  CHANNEL_5,/*!<Support for Channel 5*/
  CHANNEL_6,/*!<Support for Channel 6*/
  CHANNEL_7,/*!<Support for Channel 7*/
  CHANNEL_8,/*!<Support for Channel 8*/
  CHANNEL_9,/*!<Support for Channel 9*/             
  CHANNEL_10,/*!<Support for Channel 10*/
  CHANNEL_1a=0,/*!<Support for Channel 1a*/
  CHANNEL_1b,/*!<Support for Channel 1b*/
  CHANNEL_2a,/*!<Support for Channel 2a*/
  CHANNEL_2b,/*!<Support for Channel 2b*/
  CHANNEL_3a,/*!<Support for Channel 3a*/
  CHANNEL_3b,/*!<Support for Channel 3b*/
  Invalid_Channel/*!<Invalid channel*/
}WMBusChannel_T;

typedef enum {
  METER=(uint8_t)0x01,/*!<Device Type is Meter*/
  OTHER,/*!<Device Type is Other/Concentrator*/
  ROUTER,/*!<Device Type is Router*/
  SNIFFER,/*!<Device Type is Sniffer*/ 
  NOT_CONFIGURED =0xFF/*!<Device Type not configured*/
}WMBusDeviceType_T;

#define IS_WMBUS_MODE(MODE) (((MODE) == S1_MODE) || ((MODE) == S1m_MODE)|| \
((MODE) == S2_MODE)|| ((MODE) == T1_MODE) ||((MODE) == T2_MODE)|| \
  ((MODE) == R2_MODE)|| ((MODE) == N1_MODE)|| ((MODE) == N2_MODE)||\
    ((MODE) == C1_MODE)||((MODE) == C2_MODE)||((MODE) == F2m_MODE)||((MODE) == F2_MODE)||\
      ((MODE) == ALL_MODE)|| ((MODE) == INVALID_MODE))

#define IS_WMBUS_DEVICE_TYPE(DEV_TYPE) (((DEV_TYPE) == METER) || \
((DEV_TYPE) == OTHER)|| ((DEV_TYPE) == ROUTER)|| ((DEV_TYPE)\
  == SNIFFER)|| ((DEV_TYPE) == NOT_CONFIGURED))

#define IS_WMBUS_CHANNEL_NUM(CHAN_NUM) (((CHAN_NUM) == CHANNEL_1) ||\
((CHAN_NUM) == CHANNEL_2) || ((CHAN_NUM) == CHANNEL_3) ||\
  ((CHAN_NUM) == CHANNEL_4) || ((CHAN_NUM) == CHANNEL_5) ||\
    ((CHAN_NUM) == CHANNEL_6) || ((CHAN_NUM) == CHANNEL_7) ||\
      ((CHAN_NUM) == CHANNEL_8) || ((CHAN_NUM) == CHANNEL_9) ||\
        ((CHAN_NUM) == CHANNEL_10) || ((CHAN_NUM) == CHANNEL_1a) ||\
          ((CHAN_NUM) == CHANNEL_1b) || ((CHAN_NUM) == CHANNEL_2a) ||\
            ((CHAN_NUM) == CHANNEL_2b) || ((CHAN_NUM) == CHANNEL_3a) ||\
              ((CHAN_NUM) == CHANNEL_3b) || ((CHAN_NUM) == Invalid_Channel))


typedef enum
{
  FRAME_FORMAT_A  =(uint8_t)0x00,/*!<Indicates Frame Format A*/ 
  FRAME_FORMAT_B/*!<Indicates Frame Format B*/ 
}FrameFormatType_t;

enum
{
  PHY_STATUS_SUCCESS = 0x00,/*!<Indicates PHY Radio success */  
  PHY_STATUS_ERROR,/*!<Indicates PHY Radio error */                   
  PHY_STATUS_ERROR_TIMEOUT,/*!<Indicates PHY  timeout error*/           
  PHY_STATUS_ERROR_INVALID_DEVICE_TYPE,/*!<PHY error for Unknown Device type*/
  PHY_STATUS_ERROR_INVALID_ADDRESS, /*!<PHY error for Invalid Address*/
  PHY_STATUS_ERROR_INVALID_VALUE,/*!<PHY error for invalid value*/
  PHY_STATUS_ERROR_INVALID_STATE,/*!<PHY error for invalid state*/
  PHY_STATUS_ERROR_INVALID_LENGTH,/*!<PHY error for invalid length*/
  PHY_STATUS_ERROR_NOTHING_RECEIVED,/*!<PHY error nothing received*/
  PHY_STATUS_ERROR_DECODING,/*!<PHY error in decoding*/
  PHY_STATUS_ERROR_XTAL_TIMEOUT,/*!<PHY error crystal timeout occured*/
  PHY_STATUS_ERROR_TX_TIMEOUT,/*!<PHY error for transmission timeout*/
  PHY_STATUS_ERROR_RX_INVALID_LENGTH,/*!<PHY error for received invalid length*/
  PHY_STATUS_ERROR_RX_PACKET_INCOMPLETE,/*!<PHY error for received incomplete packet*/
  PHY_STATUS_ERROR_RX_INVALID_WMBUS_MODE,/*!<PHY error for invalid WMBus mode*/
  PHY_STATUS_INVALID_ATTR/*!<Invalid attribute value*/     
};
typedef enum {
  PHY_STATE_READY=0x00,/*!<PHY radio ready state*/ 
  PHY_STATE_TX,/*!<PHY radio tx state*/ 
  PHY_STATE_RX,/*!<PHY radio rx state*/ 
  PHY_STATE_SHUTDOWN,/*!<PHY radio shutdown state*/
  PHY_STATE_STANDBY,/*!<PHY radio standby state*/
  PHY_STATE_INVALID/*!<PHY radio state invalid*/
}WMBusPhyState; 

typedef enum {
  WMBUS_PHY_MODE =0x00,/*!<PHY Device Operation Mode*/
  WMBUS_PHY_DEVICE_TYPE,/*!<Device Type*/
  WMBUS_PHY_CHANNEL,/*!< For R2 only, otherwise it is 0*/
  WMBUS_PHY_RESPONSE_TIME_MIN,/*!<PHY minimum response time*/
  WMBUS_PHY_RESPONSE_TIME_MAX,/*!<PHY maximum response time*/
  WMBUS_PHY_RESPONSE_TIME_TYP,/*!<PHY typical response time*/
  WMBUS_PHY_PREAMBLE_LEN,/*!<Preamble length*/ 
  WMBUS_PHY_POSTAMBLE_LEN,/*!<Postable length*/
  WMBUS_PHY_OP_POWER,/*!<Operating Power*/
  WMBUS_PHY_RSSI_THRESHOLD,/*!<RSSI threshold*/
  WMBUS_PHY_TX_INTERVAL,/*!<Transmission interval*/
  WMBUS_PHY_RX_TIMEOUT, /*<RX period to seek the SYNC*/
  WMBUS_PHY_DIR,/*!<Direction of Communication*/  
  WMBUS_PHY_ATTR_CHG_INDIC,/*!<PHY Attribute Change Indication*/
  FRAME_FORMAT_TYPE_INDIC,/*!<Frame format type indication*/
  WMBUS_PHY_OFFSET/*!<Carrier Frequency offset*/
}WMBUS_PHY_ATTR;
/**
*@}
*/
/**
* @brief  WMBUS  Phy exported structures
* @{
*/
typedef struct
{
  WMBusMode_T   DevMode;
  WMBusDeviceType_T  DevType;/*!<WMbus Device type*/
  uint8_t  channel;/* For R2 only, otherwise it is 0*/
  uint16_t ResponseTimeMin;/*!<Min response time*/
  uint16_t ResponseTimeMax;/*!<Max response time*/
  uint16_t ResponseTimeTyp;/*!<Typical response time*/
  uint16_t  PreambleLength;/*!<Preamble length*/
  uint8_t  PostambleLength;/*!<Postamble length*/ 
  int32_t  RFPower;/*!<RF Power*/
  int32_t RSSIThreshold;/*RSSI Threshold*/
  uint32_t TxInterval;/*!<Tx Interval*/
  float RxTimeout;  /*<RX period to seek the SYNC*/
  uint8_t  Direction;/*!<0--> Meter to other, 1--> other to meter */
  SpiritBool AttrChangeIndic;/*!<Indicates change in attribute*/
  volatile FrameFormatType_t FrameFormat;/*!<Frame format*/
  int32_t FreqOffset;
}WMBus_PhyAttr_t;

/**
* @}
*/

/** @defgroup WMBus_Phy_Exported_Functions          WMBus Phy Exported Functions
* @{
*/
/**
* @brief  This function will initialize the Transceiver Radio registers.
* @param  Param: WMBus_PhyAttr_t *WMBusCurrPar: the pointer to WMBus_PhyAttr_t structure.
* @retval PHY_STATUS:
*                    -PHY_STATUS_SUCCESS
*                    -PHY_STATUS_ERROR_INVALID_DEVICE_TYPE.
*/
extern PHY_STATUS WMBus_PhyInit(WMBus_PhyAttr_t *WMBusCurrPar);

/**
* @brief  This function handles Transmit and Receive interrupt request from the Transceiver.
* @param  None
* @retval None
*/
extern void WMBus_PhyInterruptHandler(void);

/**
* @brief  This Routine will return the WMBus PHY attributes.
* @param  Input: 1. WMBUS_PHY_ATTR attr: The WMBus attribute.
*                2. int32 *ParamVal: The pointer to the variable to set the attribute value.
* @retval PHY_STATUS: The status of execution
*/
extern PHY_STATUS WMBus_PhyGetAttr(WMBUS_PHY_ATTR attr, int32_t *ParamVal);

/**
* @brief  This Routine will set the WMBus PHY attributes.
* @param  Input: 1. WMBUS_PHY_ATTR attr - The WMBus attribute.
*                2. int32 ParamVal: The value of the attribute.
* @retval PHY_STATUS: The status of execution.
*/
extern PHY_STATUS WMBus_PhySetAttr(WMBUS_PHY_ATTR attr, int32_t ParamVal);

/**
* @brief  This function will transmit the buffer.
* @param  1. uint8_t *buff: The pointer to the buffer to be transmitted.
*         2. uint8_t length: The length of the data buffer.
*         3. void(*TxDataSentStateHandler)(uint8_t): This callback function is 
*            invoked after successful transmission.
* @retval PHY_STATUS.
*                       
*/
extern PHY_STATUS  WMBus_PhyTxDataHandleMode (uint8_t *buff, uint16_t length, \
                                        void(*TxDataSentStateHandler)(uint8_t));
/**
* @brief  This function will receive the data in handler mode
* @param  1. uint32_t timeout: The timeout in ms.
*         2. void (*WMBus_LinkLayerParser)(uint8_t *LinkRxFrameBuffer, uint16_t LinkRxFrameBufferSize, uint8_t* status): Link Layer Parser calback function.
*         3. void (*WMBus_LinkRxFrameHandler)(uint8_t*, uint8_t): This callback function, called once new data available.
* @retval PHY_STATUS.
*                       
*/
extern PHY_STATUS  WMBus_PhyRxDataHandlerMode(uint32_t RxTimeoutDes, \
                      void (*WMBus_LinkLayerParser)(uint8_t *LinkRxFrameBuffer,\
                               uint16_t LinkRxFrameBufferSize, uint8_t* status),\
                           void (*WMBus_LinkRxFrameHandler)(uint8_t*, uint16_t));

/**
* @brief  This function will return the current PHY state.
* @param  WMBusPhyState *phyState: The pointer to WMBusPhyState type primitive.
* @retval PHY_STATUS: PHY_STATUS_SUCCESS.
*/
extern PHY_STATUS  WMBus_PhyGetState (WMBusPhyState *phyState);

/**
* @brief  This function will set the current PHY state.
* @param  WMBusPhyState phyState: The phy state.
* @retval PHY_STATUS: PHY_STATUS_ERROR_INVALID_STATE/PHY_STATUS_SUCCESS
*/
extern PHY_STATUS  WMBus_PhySetState (WMBusPhyState phyState);

/**
* @brief  This function will return the current PHY configuration.
* @param  WMBus_PhyAttr_t *WMBusCurrPar: The pointer to WMBus_PhyAttr_t structure.
* @retval PHY_STATUS.
*                       
*/
extern PHY_STATUS  WMBus_PhyReadConfig (WMBus_PhyAttr_t *WMBusCurrPar);



/**
* @}
*/

/**
*@}
*/

/**
* @}
*/
#endif /* __WMBUS_PHY_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
