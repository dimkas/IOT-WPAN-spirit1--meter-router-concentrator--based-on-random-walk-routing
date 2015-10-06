/**
* @file    wmbus_appli.c
* @author  MSH RF/IMS Systems Lab
* @version V1.0.0
* @date    September 20, 2011
* @brief   This file includes routines for WMBus application Layer.
* @details
*
* THE PRESENT FIRMWARE WHICH IS FOR GUIDANCE ONLY AIMS AT PROVIDING CUSTOMERS
* WITH CODING INFORMATION REGARDING THEIR PRODUCTS IN ORDER FOR THEM TO SAVE
* TIME. AS A RESULT, STMICROELECTRONICS SHALL NOT BE HELD LIABLE FOR ANY
* DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES WITH RESPECT TO ANY CLAIMS ARISING
* FROM THE CONTENT OF SUCH FIRMWARE AND/OR THE USE MADE BY CUSTOMERS OF THE
* CODING INFORMATION CONTAINED HEREIN IN CONNECTION WITH THEIR PRODUCTS.
*
* THIS SOURCE CODE IS PROTECTED BY A LICENSE.
* FOR MORE INFORMATION PLEASE CAREFULLY READ THE LICENSE AGREEMENT FILE LOCATED
* IN THE ROOT DIRECTORY OF THIS FIRMWARE PACKAGE.
*
* <h2><center>&copy; COPYRIGHT 2011 STMicroelectronics</center></h2>
*/
/* Includes ------------------------------------------------------------------*/
#include "wmbus_appli.h"


/** @defgroup WMBUS_APPLI
* @brief This file is the interface between WMBus user interface and Link layer
* @{
*/
//#pragma pack(1)
/** @defgroup WMBUS_APPLI_Private_Define
* @{
*/
/**
* @}
*/
/** @defgroup WMBUS_APPLI_Private_Variables
* @{
*/
WMBusOperationMode_t WMBusOperationMode = INSTALL_MODE;

volatile SpiritBool bNewdatareceived = S_FALSE;
volatile SpiritBool AckReceived = S_FALSE;
//DisplayFrame_t TFTDisplayFrame;
volatile SpiritBool InstallationReqSent = S_FALSE;
volatile SpiritBool InstallationConfirmSent = S_FALSE;
static volatile SpiritBool AccessDemandReqTransmitted = S_FALSE;
static volatile SpiritBool ValidDataReqReceived = S_FALSE;
static volatile SpiritBool TimeoutOcurred =S_FALSE;
SpiritBool LinkAttrChanged = S_FALSE;
volatile SpiritBool NewUserDataRequestRxd = S_FALSE;
volatile SpiritBool ACKTransmitted = S_FALSE;
volatile SpiritBool NRDataTransmitted = S_FALSE;
Indication gindicFrame;
uint8_t indicationPayload[128]; //modified to 128 from 255
volatile LastEventLog_t AppliLastEvent;
extern SpiritBool DataSendFlag;
/*Some state variables used for encryption*/
/*As of now all these fields are 0, and will be implemented 
as and when required*/

extern uint8_t accNum;
extern uint8_t CipherBlocklen;

/*Initialization vctr in the Meter Side*/
uint8_t AES_InitnalizationVector_Tx[16]={0};
//
///*Initialization vctr in the Concentrator Side*/
uint8_t AES_InitnalizationVector_Rx[16]={0};


#ifndef CLI
RTC_InitTypeDef RTC_InitStructVal=
{
  /*Calender Configuartion*/
  .RTC_HourFormat = RTC_HourFormat_24,
  .RTC_AsynchPrediv = 0x7F,
  .RTC_SynchPrediv	=  0xFF  
};

RTC_TimeTypeDef RTC_TimeStructVal = 
{
  /* Set the Default Time */
  .RTC_Hours   = 0x08,
  .RTC_Minutes = 0x00,
  .RTC_Seconds = 0x57
}; 

RTC_DateTypeDef RTC_DateStructVal =
{
  /* Set the Date */
  .RTC_Month = RTC_Month_January,
  .RTC_Date = 0x01,  
  .RTC_Year = 0x11, 
  .RTC_WeekDay = RTC_Weekday_Saturday
};
#endif
struct DataBaseList *rootDataBase;
/**
* @}
*/
/** @defgroup WMBUS_APPLI_Private_FunctionPrototypes
* @{
*/
void WMBus_AppliInit(void);
LINK_STATUS WMBus_AppliPowerOn(void);
void WMBus_AppliRxDataHandler(Frame *Frame);
static void WMBus_AppliIndicaCallback(Indication *indic, LastEventLog_t LastEvent);
static void WMBus_AppliTxDataSentHandler(LINK_STATUS status,LastEventLog_t LastEvent);
void RTC_Config(void);
void LSE_StabTime(void);
void SpiritManagementRangeExtInit(void)
{}
/**
* @}
*/
/** @defgroup WMBUS_APPLI_Private_Functions
* @{
*/
/**
* @brief  This function will send Meter Data to concentrator.
* @param  Param:None. 
* @retval LINK_STATUS.
*                       
*/
LINK_STATUS WMBus_AppliSendMeterData(uint8_t *buff, uint8_t length)
{
  LINK_STATUS status;
  int32_t value;
  
  Request request;
  uint8_t requestPayload[128];/*modified to 128 from 5*/
  request.frame.ci_field                = 0x72;
  /*M A CC SN FN BC*/;
  
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_MODE, &value))
  {
    switch((WMBusMode_T)value)
    {
    case S1_MODE:
    case S1m_MODE:  
    case T1_MODE:
    case N1_MODE: 
    case C1_MODE:
      {
        request.frame.data_field.header.accessNum = accNum;
        request.frame.data_field.header.status_field = 0x00;
        request.frame.data_field.header.config_word = 0x0150;
        request.frame.data_field.payload       = buff;
        request.frame.data_field.capacity      = 128;//255;
        request.frame.data_field.size          = length;
      
        /* Prepare the frame for transmission  */
        request.frame.c_field                 = LINK_SEND_NOREPLY_USER_DATA;
        request.frame.ci_field                = 0x7A;
        
        request.retries = 1;
        /**************** Non-Block Mode Example Starts here    *****************/
        NRDataTransmitted = S_FALSE;
        status = WMBus_LinkServicesRequestsCallbackMode(&request, WMBus_AppliTxDataSentHandler);
        /**************** Non-Block Mode Example Ends here    *****************/
        while(S_FALSE == NRDataTransmitted);
        break;
      }
    case S2_MODE:
    case R2_MODE:
    case T2_MODE:
    case N2_MODE:
    case C2_MODE:
      {
        request.frame.data_field.payload       = requestPayload;
        request.frame.data_field.capacity      = sizeof(requestPayload);
        request.frame.data_field.size          = 0;
        
        Indication indication;
        indication.RxTimeout = 2000;/* Milliseconds*/
        indication.frame.data_field.payload    = indicationPayload;
        indication.frame.data_field.capacity   = sizeof(indicationPayload);
        indication.frame.data_field.size       = 0;
        
        /* Set number of request retries  */
        request.retries = 1;
        /* Prepare the frame for transmission  */
        request.frame.c_field                 = LINK_REQUEST_RESPOND_ACCESS_DEMAND;
        request.frame.ci_field                = 0x8A;
        /**************** Non-Block Mode Example Starts here    *****************/
        AckReceived = S_FALSE;
        AccessDemandReqTransmitted = S_FALSE;
        if(LINK_STATUS_SUCCESS == WMBus_LinkServicesRequestsCallbackMode(&request, WMBus_AppliTxDataSentHandler))
        {
          while(S_FALSE ==AccessDemandReqTransmitted);
          
          
          status = WMBus_LinkServicesIndicationCallbackMode(&indication, WMBus_AppliIndicaCallback);
          while((S_FALSE == AckReceived)&&(S_FALSE == TimeoutOcurred));
          if(S_TRUE == TimeoutOcurred)//retry
          {
            TimeoutOcurred = S_FALSE;
          }
          if(S_TRUE == AckReceived)
          {
            Response response;
            TimeoutOcurred = S_FALSE;
            AckReceived = S_FALSE;
            NewUserDataRequestRxd = S_FALSE;
            status = WMBus_LinkServicesIndicationCallbackMode(&indication, WMBus_AppliIndicaCallback);
            while((S_FALSE == NewUserDataRequestRxd)&&(S_FALSE == TimeoutOcurred));
            if(S_TRUE == NewUserDataRequestRxd)
            {
              gindicFrame.status = LINK_STATUS_SUCCESS;
              gindicFrame.serviceType = LINK_SERVICE_TYPE_REQUEST_RESPOND;
              
              response.frame.data_field.header.accessNum = accNum;
              response.frame.data_field.header.status_field = 0;
              response.frame.data_field.header.config_word = 0;
              response.frame.data_field.payload   = buff; 
              response.frame.data_field.capacity  = sizeof(buff);
              response.frame.data_field.size      = length;
              response.positive = S_TRUE;
              
              response.frame.c_field                 = LINK_RESPOND_USER_DATA;
              response.frame.ci_field                = 0x7A;
              
              SdkDelayMs(4);
              
              /*  Transmit the response with the meter usage  */
              if ((status = WMBus_LinkServicesResponseCallbackMode(&gindicFrame, &response,WMBus_AppliTxDataSentHandler)) == LINK_STATUS_SUCCESS)
              {
                /* Transmitted response  */
                
              }
            }
          }
        }
        /**************** Non-Block Mode Example Ends here    *****************/
        break; 
      }
    case ALL_MODE:
      break;
    default:
      break;
    }
  }
  else
  {
    return LINK_STATUS_INVALID_ATTR;
  }
return status;
}

/**
* @brief  This function will init the link layer after power on
* @param  Param:None. 
* @retval LINK_STATUS.
*                       
*/
LINK_STATUS WMBus_AppliPowerOn(void)
{
  /* Start the radio with default Link and Phy settings */
  LINK_STATUS status = WMBus_LinkServicesInit();
  
  /* IRQ GPIO */
  SGpioInit xGpioIRQ=
  {
    SPIRIT_GPIO_3,
    SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
    SPIRIT_GPIO_DIG_OUT_IRQ
  };
  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ);
  
  /* IRQ GPIO */
  SGpioInit xGpioIRQ2=
  {
    SPIRIT_GPIO_0,
    SPIRIT_GPIO_MODE_DIGITAL_OUTPUT_LP,
    SPIRIT_GPIO_DIG_OUT_TX_RX_MODE
  };
  /* Spirit IRQ config */
  SpiritGpioInit(&xGpioIRQ2);
  
  return status;
}

/**
* @brief  This function will init the WMBUS application.
* @param  Param:None. 
* @retval None:
*                       
*/
void WMBus_AppliInit(void)
{
  /*  Init Meter database */
  WMBus_AppliPowerOn();  
    
}
/**
* @brief  This function will handle the received data from the link layer.
* @param  Frame frame: The pointer to the link frame. 
* @retval None.
*                       
*/
void WMBus_AppliRxDataHandler(Frame *frame)
{
  
  int32_t value;

  if(DATA_COMM_MODE == WMBusOperationMode)
  {
    
    if(NULL != frame)
    {
     
      WMBus_LinkGetAttribute(LINK_ATTR_ID_DEV_TYPE, &value);
      
      bNewdatareceived =S_TRUE;
      /* Check the type of frame received and trigger the necessary actions here*/
    }

  }
}

/**
* @brief  This function will handle the install Meter request.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
LINK_STATUS WMBus_appliSendMeterInstallationRequest(void)
{
  LINK_STATUS status= LINK_STATUS_UNKNOWN_ERROR;
  Request request;
  uint8_t requestPayload[5];
  Indication indication;
  
  request.frame.data_field.payload       = requestPayload;
  request.frame.data_field.capacity      = sizeof(requestPayload);
  request.frame.data_field.size          = 0;
  /* Set number of request retries*/
  request.retries = 1;
  /* Prepare the frame for transmission , For S1, S1m and T1 mode */
  request.frame.c_field                 = LINK_PRIMARY_INSTALLATION_REQ;
  request.frame.ci_field                = 0x7A;
  
  /* Set receive timeout  */
  WMBus_LinkGetAttribute(LINK_ATTR_ID_RX_TIMEOUT,&indication.RxTimeout);
  
  indication.frame.data_field.payload    = indicationPayload;
  indication.frame.data_field.capacity   = sizeof(indicationPayload);
  indication.frame.data_field.size       = 0;
  
  /***********  Non-Block Mode Installation Request Starts Here    ************/
  InstallationReqSent  = S_FALSE;
  if(LINK_STATUS_SUCCESS == WMBus_LinkServicesRequestsCallbackMode(&request, WMBus_AppliTxDataSentHandler))
  {
    while(S_FALSE ==InstallationReqSent);
    status = WMBus_LinkServicesIndicationCallbackMode(&indication, WMBus_AppliIndicaCallback);
    while((CONFIRM_INSTALLATION_RXD != AppliLastEvent.event)&&(S_FALSE == TimeoutOcurred));
    if(S_TRUE == TimeoutOcurred)
    {
      TimeoutOcurred = S_FALSE;
    }
    status = WMBus_LinkServicesIndicationCallbackMode(&indication, WMBus_AppliIndicaCallback);
    while((SEND_CONFIRM_RESET_REMOTE_LINK_RXD != AppliLastEvent.event)&&(S_FALSE == TimeoutOcurred));
    if(S_TRUE == TimeoutOcurred)
    {
      TimeoutOcurred = S_FALSE;
    }
    if(AppliLastEvent.event == SEND_CONFIRM_RESET_REMOTE_LINK_RXD)
    {
      /*meter can go into low power mode*/
    }
  }
  
  return status;
}


/**
* @brief  This function will set the MFR ID(MFR+Identification field).
* @param  uint8_t *buff: The MFR ID in array format. 
* @retval SpiritBool:Returns S_TRUE if the operation succesful S_FALSE otherwise.
*                       
*/
SpiritBool WMBus_SetBoardMnfID(uint8_t *buff)
{
  uint32_t value;  
  value =(uint32_t)*buff++;
  value = value<<8;
  value |=(uint8_t)*buff++;
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_MFR_ID, value))
    return S_FALSE;  
  value =(uint32_t)*buff++;
  value = value<<8;
  value |=(uint8_t)*buff++;
  value = value<<8;
  value |=(uint8_t)*buff++;
  value = value<<8;
  value |=(uint8_t)*buff++;  
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_IDENTIFICATION, value))
    return S_FALSE;
  value =(uint32_t)*buff++;
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_METER_VERSION, value))
    return S_FALSE;
  value =(uint32_t)*buff++;
  if(LINK_STATUS_SUCCESS != WMBus_LinkSetAttribute(LINK_ATTR_ID_METER_TYPE, value))
    return S_FALSE;
  LinkAttrChanged = S_TRUE;
  return S_TRUE;
}

/**
* @brief  This function will return the WMBus device type.
* @param  Param:None. 
* @retval WMBusDeviceType_T: As shown below
*                     0x01 -> METER
*                     0x02 -> OTHER,
*                     0x03 -> ROUTER,
*                     0x04 -> SNIFFER, 
*                     0xFF -> NOT_CONFIGURED
*                       
*/
WMBusDeviceType_T WMBus_GetWMBusDeviceType(void)
{
  int32_t value=0x00;
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_DEV_TYPE, &value))
  {
    return (WMBusDeviceType_T)value;
  }
  return NOT_CONFIGURED;
}

void WMBus_AppliIndicaCallback(Indication *indic, LastEventLog_t LastEvent)
{
  AppliLastEvent.event =  LastEvent.event;
  AppliLastEvent.globalTic =  LastEvent.globalTic;
  gindicFrame.frame.data_field.payload = indicationPayload;
  gindicFrame.frame.data_field.capacity = sizeof(indicationPayload);
  if(LINK_STATUS_PHY_ERROR_TIMEOUT == indic->status)
  {
#ifdef CLI
    WMBus_AppliRxDataHandler(NULL);
#endif
    TimeoutOcurred =S_TRUE;
  }
  else
  {   
    ValidDataReqReceived = S_TRUE;
    switch(indic->frame.c_field &0x4F)
    {
    case LINK_SEND_NOREPLY_USER_DATA:
      memcpy(&gindicFrame.frame,&indic->frame,10);
      gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
      gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
      gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
      WMBus_AppliRxDataHandler(&indic->frame);
      break;
    case LINK_SEND_USER_DATA2_CMD:/*(SND-UD): conc -> mtr: after conc receives SND-NR*/
      if(indic->frame.c_field==0x73)
      {
        memcpy(&gindicFrame.frame,&indic->frame,10);
        gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
        gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
        gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
        gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
        gindicFrame.frame.data_field.size = indic->frame.data_field.size;
        memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//        SendUserDataRequestRxd = S_TRUE;
      }
      else
      {
        memcpy(&gindicFrame.frame,&indic->frame,10);
        gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
        gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
        gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
        gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
        gindicFrame.frame.data_field.size = indic->frame.data_field.size;
        memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
        NewUserDataRequestRxd = S_TRUE;
        
      }
      break;
    case LINK_PRIMARY_INSTALLATION_REQ:
      memcpy(&gindicFrame.frame,&indic->frame,10);
      gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
      gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
      gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
      break;
    case LINK_REQUEST_RESPOND_ACCESS_DEMAND:
      memcpy(&gindicFrame.frame,&indic->frame,10);
      gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
      gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
      gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
      break;
    case LINK_REQUEST_RESPOND_CLASS2_DATA:
      memcpy(&gindicFrame.frame,&indic->frame,10);
      gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
      gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
      gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
      NewUserDataRequestRxd = S_TRUE;
      break;
    case LINK_REQUEST_RESPOND_CLASS1_DATA:/*(REQ_UD1): cocn->mtr; Alarm request*/
      memcpy(&gindicFrame.frame,&indic->frame,10);
      gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
      gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
      gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//      NewAlarmDataRequestRxd = S_TRUE;
      break;
    case LINK_RESPOND_USER_DATA:
      memcpy(&gindicFrame.frame,&indic->frame,10);
      gindicFrame.frame.data_field.header.accessNum = indic->frame.data_field.header.accessNum;
      gindicFrame.frame.data_field.header.status_field = indic->frame.data_field.header.status_field;
      gindicFrame.frame.data_field.header.config_word = indic->frame.data_field.header.config_word;
      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
      WMBus_AppliRxDataHandler(&indic->frame);
      break;
    case LINK_CONFIRM_INSTALLATION:
//      SdkEvalLedOn(LED5);
      break;
    case LINK_CONFIRM_ACK:
      AckReceived = S_TRUE;
      break;
    case LINK_SEND_CONFIRM_RESET_REMOTE_LINK:/*(SND_NKE): conc->mtr*/
//      LinkResetReceived = S_TRUE;
//      SdkEvalLedToggle(LED4);
      break;
    default:
      ;
    }
  }
}
//
///**
//* @brief  This function is called after invoking the indication service.
//* @param  Indication: This indicates type of request and associated data.
//* @retval None
//*/
//void WMBus_AppliIndicaCallback(Indication *indic, LastEventLog_t LastEvent)
//{
//  AppliLastEvent.event =  LastEvent.event;
//  AppliLastEvent.globalTic =  LastEvent.globalTic;
//  gindicFrame.frame.data_field.payload = indicationPayload;
//  gindicFrame.frame.data_field.capacity = sizeof(indicationPayload);
//  if(LINK_STATUS_PHY_ERROR_TIMEOUT == indic->status)
//  {
//#ifdef CLI
//    WMBus_AppliRxDataHandler(NULL);
//#endif
//    TimeoutOcurred =S_TRUE;
//  }
//  else
//  {   
//    ValidDataReqReceived = S_TRUE;
//    switch(indic->frame.c_field &0x4F)
//    {
//    case LINK_SEND_NOREPLY_USER_DATA:
//#if 1
//      memcpy(&gindicFrame.frame,&indic->frame,10);
//#endif
//      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
//      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
//      #if 1
//      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//      #endif
//      WMBus_AppliRxDataHandler(&indic->frame);
//      break;
//    case LINK_PRIMARY_INSTALLATION_REQ:
//#if 1
//      memcpy(&gindicFrame.frame,&indic->frame,10);
//#endif
//      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
//      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
//#if 1
//      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//#endif
//      break;
//    case LINK_REQUEST_RESPOND_ACCESS_DEMAND:
//#if 1
//      memcpy(&gindicFrame.frame,&indic->frame,10);
//#endif
//      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
//      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
//#if 1
//      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//#endif
//      break;
//    case LINK_REQUEST_RESPOND_CLASS2_DATA:
//#if 1
//      memcpy(&gindicFrame.frame,&indic->frame,10);
//#endif
//      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
//      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
//#if 1
//      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//#endif
//      NewUserDataRequestRxd = S_TRUE;
//      break;
//    case LINK_RESPOND_USER_DATA:
//#if 1
//      memcpy(&gindicFrame.frame,&indic->frame,10);
//#endif
//      gindicFrame.frame.RSSI_field = indic->frame.RSSI_field;
//      gindicFrame.frame.data_field.size = indic->frame.data_field.size;
//#if 1
//      memcpy(gindicFrame.frame.data_field.payload,indic->frame.data_field.payload,indic->frame.data_field.size);
//#endif
//      WMBus_AppliRxDataHandler(&indic->frame);
//      break;
//    case LINK_CONFIRM_INSTALLATION:
//      SdkEvalLedOn(LED2);
//      break;
//    case LINK_CONFIRM_ACK:
//      AckReceived = S_TRUE;
//      break;
//    default:
//      ;
//    }
//  }
//}
/**
* @brief  This function is called after invoking the indication service.
* @param  Indication: This indicates type of request and associated data.
* @retval None
*/
void WMBus_AppliTxDataSentHandler(LINK_STATUS status, LastEventLog_t LastEvent)
{
  AppliLastEvent.event =  LastEvent.event;
  AppliLastEvent.globalTic =  LastEvent.globalTic;
  
  if(LINK_STATUS_SUCCESS == status)
  {
    /* The data transmitted successfully*/
    if(AppliLastEvent.event == CONFIRM_INSTALLATION_SENT)
    {
      SdkEvalLedOn(LED2);
      InstallationConfirmSent = S_TRUE;
    }
    else if(AppliLastEvent.event == PRIMARY_INSTALLATION_REQ_TXD)
    {
      InstallationReqSent = S_TRUE;
    }
    else if(AppliLastEvent.event == REQUEST_RESPOND_ACCESS_DEMAND_TXD)
    {
      AccessDemandReqTransmitted = S_TRUE;
    }  
    else if(CONFIRM_ACK_SENT == AppliLastEvent.event)
    {
      ACKTransmitted = S_TRUE;
    }
    else if(REQUEST_RESPOND_CLASS2_DATA_TXD == AppliLastEvent.event)
    {
      //ACKTransmitted = S_FALSE;
    }
    else if(SEND_NOREPLY_USER_DATA_TXD == AppliLastEvent.event)
    {
      NRDataTransmitted = S_TRUE;
    }
  } 
}

/**
* @brief  This function will turn on the radio and waits till it enters the Ready state.
* @param  Param:None. 
* @retval None
*                       
*/
void Appli_RadioPowerON(void)
{
  WMBus_LinkRadioPowerOn();
}
/**
* @brief  This function will Shut Down the radio.
* @param  Param:None. 
* @retval None
*                       
*/
void Appli_RadioPowerOFF(void)
{
  WMBus_LinkRadioPowerOff();
}
/**
* @brief  This function will put the radio in standby state.
* @param  Param:None. 
* @retval None
*                       
*/
void Appli_RadioStandBy(void)
{
  WMBus_LinkRadioStandby();
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
* @brief  This function will set the wmbus channel.
* @param  Param:WMBusChannel_T channel. 
* @retval SpiritBool:
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
* @param  Param:uint8_t MeterType. 
* @retval SpiritBool:
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
* @}
*/
/**
* @}
*/
/******************* (C) COPYRIGHT 2012 STMicroelectronics *****END OF FILE****/
