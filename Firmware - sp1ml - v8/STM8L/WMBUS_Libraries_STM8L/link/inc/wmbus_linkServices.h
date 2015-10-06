/**
******************************************************************************
* @file    wmbus_linkServices.h
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  Header for Wireles M-bus link services.
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
#ifndef __WMBUS_LINKSERVICES_H
#define __WMBUS_LINKSERVICES_H
/* Includes ------------------------------------------------------------------*/
#include "wmbus_link.h"
#include <stdint.h>
/**
* @defgroup WMBUS_Libraries
* @{
*/
/**
 * @defgroup SPIRIT_WMBus_LinkServices    WMBus Link Services
* @brief Configuration and management of WMBus Link Services layer.
* @details See the file <i>@ref wmbus_linkservices.h</i> for more details.
 * @{
 */
/**
* @defgroup SPIRIT_WMBus_LinkServices WMBus Link Services enum Types
* @{
*/
/**
* @brief  SPIRIT1 WMBus Link Primary Services enum definition.The PRM bit in C field is 1.
*/
typedef enum 
{
  LINK_SEND_CONFIRM_RESET_REMOTE_LINK = 0x40, /*!< Reset Remote Link 
       Command From Concentrator; The direction is from Concentrator to meter */
  LINK_SEND_USER_DATA2_CMD            = 0x43, /*!< Send data Command; 
                                            The meter should send RSP_UD for this command*/
  LINK_SEND_USER_DATA_CMD             = 0x73, /*!< Send data Command; 
                                            The meter should ACK this  command*/
  LINK_SEND_NOREPLY_USER_DATA         = 0x44,  /*!< Send Application Data; 
          The Application data from Meter without request and no Ack is needed*/
  LINK_PRIMARY_INSTALLATION_REQ       = 0x46,/*!< Installation Request from Meter; 
                 The concentrator shoul ACK and register meter in its database*/
  LINK_REQUEST_NOREPLY_ACCESS_DEMAND  = 0x47,/*!< ACCESS Request from Meter; 
                      The meter needs access. No Ack needed from Concentrator */
  LINK_REQUEST_RESPOND_ACCESS_DEMAND  = 0x48,/*!< Access Demand request from Meter; 
          The concentrator should reply with LINK_CONFIRM_ACK for this request*/
  LINK_REQUEST_RESPOND_CLASS1_DATA    = 0x4A, /*!< Class 1 Data(Status/Alarm) 
  Request from Concentrator;The meter should reply with LINK_RESPOND_USER_DATA/LINK_CONFIRM_ACK */
  LINK_REQUEST_RESPOND_CLASS2_DATA    = 0x4B  /*!< Class 2 Data(Application) 
  Request from Concentrator; The meter should reply with LINK_RESPOND_USER_DATA*/ 
} LINK_PRIMARY_SERVICE_FUNCTION;

/**
* @brief  SPIRIT1 WMBus Link Secondary Services enum definition.The PRM bit in C field is 0.
*/
typedef enum 
{
  LINK_CONFIRM_ACK                          = 0x00,/*!< Acknowledgement from Meter/Concentrator; */
  LINK_CONFIRM_INSTALLATION                 = 0x06,/*!< Confimation of Installation request from Concentrator;
                    The meter should Stop the installation request after this. */
  LINK_RESPOND_USER_DATA                    = 0x08,/*!< Alarm or application data from Meter; */
} LINK_SECONDARY_SERVICE_FUNCTION;
/**
* @brief  SPIRIT1 WMBus Link Services Last event enum definition.
*/
typedef enum
{
  NO_EVENT                           = 0x00,/*!< There is no event after start-up; */
  SEND_CONFIRM_RESET_REMOTE_LINK_RXD = 0x41,/*!< The reset Remote Link Command received; */
  SEND_CONFIRM_RESET_REMOTE_LINK_TXD = 0x81,/*!< The reset Remote Link Command transmitted; */
  SEND_USER_DATA_CMD_RXD               = 0x42,/*!< The Command Received from Concentrator; */
  SEND_USER_DATA_CMD_TXD               = 0x82,/*!< The Command transmitted to Meter; */
  SEND_NOREPLY_USER_DATA_TXD         = 0x43,/*!< The User data(SND-NR) Received; */
  SEND_NOREPLY_USER_DATA_RXD         = 0x83,/*!< The User data(SND-NR) Transmitted; */
  PRIMARY_INSTALLATION_REQ_TXD       = 0x44,/*!< The Primary installation request(SND_IR) transmitted to concentrator; */
  PRIMARY_INSTALLATION_REQ_RXD       = 0x84,/*!< The Primary installation request(SND_IR) Received; */
  REQUEST_NOREPLY_ACCESS_DEMAND_TXD  = 0x85,/*!< The Primary Access request(ACC_NR) Transmitted; */
  REQUEST_NOREPLY_ACCESS_DEMAND_RXD  = 0x45,/*!< The Primary Access request(ACC_NR) Received; */
  REQUEST_RESPOND_ACCESS_DEMAND_TXD  = 0x0B,/*!< The Primary Access demand request(ACC_DMD) Transmitted; */
  REQUEST_RESPOND_ACCESS_DEMAND_RXD  = 0x0C,/*!< The Primary Access demand request(ACC_DMD) Received; */
  REQUEST_RESPOND_CLASS1_DATA_TXD    = 0x0D,/*!< The Primary Class 1 Data request(REQ_UD1) Transmitted; */
  REQUEST_RESPOND_CLASS1_DATA_RXD    = 0x0E,/*!< The Primary Class 1 Data request(REQ_UD1) Received; */
  REQUEST_RESPOND_CLASS2_DATA_TXD    = 0x0F,/*!< The Primary Class 2 Data request(REQ_UD1) Transmitted; */
  REQUEST_RESPOND_CLASS2_DATA_RXD    = 0x10,/*!< The Primary Class 2 Data request(REQ_UD1) Received; */
  
  CONFIRM_ACK_SENT                   = 0x11,/*!< The Ack(ACK) Sent; */
  CONFIRM_ACK_RXD                    = 0x12,/*!< The Ack(ACK) Received; */
  CONFIRM_INSTALLATION_SENT          = 0x13,/*!< The confirm Installation request(CNF-IR) Sent; */
  CONFIRM_INSTALLATION_RXD           = 0x14,/*!< The confirm Installation request(CNF-IR) received; */
  RESPOND_USER_DATA_SENT             = 0x15,/*!< The User Datagram (RSP-UD) Sent; */
  RESPOND_USER_DATA_RXD              = 0x16,/*!< The User Datagram (RSP-UD) Received; */
  
  LAST_FRAME_REPEATED                = 0x20,/*!< The repeated frame received; */
  TIME_OUT_EVENT                     = 0x30,/*!< The timeout ocurred; */
  
  INVALID_EVENT                      = 0xFF /*!< The invalid event; */
}EVENT_TYPE_T;
/**
* @brief  SPIRIT1 WMBus Link Services Current Link State enum definition.
*/
typedef enum
{
  LINK_STATE_IDLE =0x00,/*!< The Stack is in Idle state; */
  LINK_STATE_BUSY,/*!< The Stack is in busy state;It is preparing for Tx or Rx operation */
  LINK_STATE_TX_SERV,/*!< The Stack is in Tx(Service Request) state; */
  LINK_STATE_TX_RESPONSE,/*!< The Stack is in Tx( Sending Response) state; */
  LINK_STATE_RX_INDIC,/*!< The Stack is in Rx(Waiting for Indication) state; */
  LINK_STATE_RX, /*!< The Stack is in Rx state; */
  LINK_STATE_ERROR/*!< The Stack is in Error state; This can be cleared by reconfiguring or reinitilizing the stack */
}WMBUS_LINK_STATE_T;
/**
* @brief  SPIRIT1 WMBus Link Service type enum definition.
*/
typedef enum 
{
  LINK_SERVICE_TYPE_SEND_NOREPLY,/*!< The frame transmitted/received is SND_NR user data;No ack needed */
  LINK_SERVICE_TYPE_SEND_CONFIRM,/*!< The frame transmitted/received needs confirmation/ack  from Secondary station */
  LINK_SERVICE_TYPE_REQUEST_RESPOND /*!< The frame transmitted/received needs response(Data) from Secondary station */
} LINK_SERVICE_TYPE;
/**
*@}
*/

/**
* @brief  WMBUS Link services structures
* @{
*/
/**
* @brief  SPIRIT1 WMBus Link Services Request type structure definition.
*/
typedef struct 
{
  uint8_t retries;/*!< The number of retries; The stack send repeated requests if there is no Ack/Response from Secondary Station */
  Frame   frame;/*!< The frame contains attributes and payload; */
} Request;
/**
* @brief  SPIRIT1 WMBus Link Services Response type structure definition.
*/
typedef struct 
{
  SpiritBool  positive;/*!< The Ackpositive-> S_TRUE/Nack(positive-> S_FALSE); */
  Frame frame;/*!< The frame contains attributes and payload; */
} Response;
/**
* @brief  SPIRIT1 WMBus Link Services Confirmation type structure definition.
*/
typedef struct 
{
  LINK_STATUS  status;/*!< The Link layer stack statue; */
  SpiritBool   positive;/*!< The Ackpositive-> S_TRUE/Nack(positive-> S_FALSE); */
  LINK_SERVICE_TYPE  serviceType;/*!< The service type attribute; */
  Frame                frame;/*!< The frame contains attributes and payload; */
} Confirmation;
/**
* @brief  SPIRIT1 WMBus Link Services Indication structure definition.
*/
typedef struct 
{
  int32_t           RxTimeout;/*!< The timeout in milli seconds; */
  LINK_STATUS        status;/*!< The Link layer stack statue; */
  LINK_SERVICE_TYPE  serviceType;/*!< The service type attribute; */
  uint8_t  timestamp[6];/*!< The timestamp DD:MM:YY:SS:MM:HH; */
  Frame  frame;/*!< The frame contains attributes and payload; */
} Indication;
/**
* @brief  SPIRIT1 WMBus Link Services LastEventLog_t structure definition.
*/
typedef struct
{
  EVENT_TYPE_T event;/*!< The last Link Event; */
  uint32_t globalTic;/*!< The Global Tic value at last Link Event; */
}LastEventLog_t;

/**
*@}
*/
/** @defgroup WMBus_Link_Services_Functions WMBus link services Exported Functions
* @{
*/

/**
 * @brief  This function initializes the Mbus Link Services module.
 * @param  None.
 * @retval LINK_STATUS.
 */
extern LINK_STATUS WMBus_LinkServicesInit(void);
/**
* @brief This function reset the Mbus Link Services module.
* @param  None.
* @retval Return Value :LINK_STATUS
*                       
*/
extern LINK_STATUS WMBus_LinkServicesReset(void);
/**
* @brief  This function initiates a Service request, generates a valid
*     C-field by setting the FCV, FCB, and PRM bits, transmits the request frame,
*     checks for confirm/response is applicable.
* @param  1) Request *request - A pointer to a Request primitive contains retry counter, attributes and payload.
*         2) void (*ApplliTxDataHandler)(LINK_STATUS,LastEventLog_t LastEvent)
               - This callback is called when the service request is complete.
* @retval LINK_STATUS :This function returns LINK_STATUS_SUCCESS once the service 
*                      request is initiated successfully. Once the service request
*                      is complete, the callback is invoked by link layer.
*                       
*/
extern LINK_STATUS WMBus_LinkServicesRequestsCallbackMode(Request *request, void (*ApplliTxDataHandler)(LINK_STATUS,LastEventLog_t LastEvent));
/**
* @brief  This function attempts to receive a frame within the timeout duration specified.
*       After successful configuration of stack parameters it returns LINK_STATUS_SUCCESS 
*       and after succesful receptio of the frame the WMBus_AppliIndicaCallback 
*       function is called from the link layer.
* @param  1) Indication *indication - A pointer to a Indication primitive contains attributes and payload.
*         2) void (*WMBus_AppliIndicaCallback)(Indication*, LastEventLog_t)
*               - This callback is called when frame is received or timeout ocurred. 
*                This can be checked by indication->LINK_STATUS.
* @retval LINK_STATUS :This function returns LINK_STATUS_SUCCESS once the service 
*                      request is initiated successfully. Once the service request
*                      is complete, the callback is invoked by link layer.
*                       
*/
extern LINK_STATUS WMBus_LinkServicesIndicationCallbackMode(Indication *indication, void (*WMBus_AppliIndicaCallback)(Indication*, LastEventLog_t));
/**
* @brief  TThis function attempts to transmit the response to the indication frame received.
* @param  1) Indication* indication - A pointer to the received link layer indication service primitive.
*         2) Response* response     - A pointer to a link layer response service primitive.
*         3) void (*WMBus_AppliResponseCallback)(LINK_STATUS,LastEventLog_t)
               - This callback is called when the response is sent.
* @retval LINK_STATUS :This function returns LINK_STATUS_SUCCESS once the service 
*                      request is initiated successfully. Once the service request
*                      is complete, the callback is invoked by link layer.
*                       
*/
extern LINK_STATUS WMBus_LinkServicesResponseCallbackMode(Indication *indication, Response *response, void (*WMBus_AppliResponseCallback)(LINK_STATUS,LastEventLog_t));

/**
* @brief This function will return the current stat of the stack.
* @param  None.
* @retval WMBUS_LINK_STATE_T.
*                       
*/
extern WMBUS_LINK_STATE_T WMBus_LinkServicesCurrState(void);
/**
* @}
*/
/**
*@}
*/
/**
* @}
*/
#endif /* __WMBUS_LINKSERVICES_H */
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/