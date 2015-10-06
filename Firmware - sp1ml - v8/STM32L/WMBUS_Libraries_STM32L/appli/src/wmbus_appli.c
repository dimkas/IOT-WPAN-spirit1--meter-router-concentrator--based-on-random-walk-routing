/**
******************************************************************************
* @file    wmbus_appli.c
* @author  System Lab - NOIDA
* @version V1.0.8
* @date    24-Jul-2014
* @brief  This file includes routines for WMBus application Layer.
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
#include "wmbus_appli.h"
#include "wmbus_phydatamem.h"
#include "aes128.h"
#include "main.h"
#include "SDK_EVAL_Timers.h"
/** @defgroup WMBUS_APPLI
* @brief This file is the interface between WMBus user interface and Link layer
* @{
*/
#pragma pack(1)
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

SpiritBool bNewdatareceived = S_FALSE;
SpiritBool DatabaseTobeUpdated =S_FALSE;
SpiritBool bNewSnifferdataRxd = S_FALSE;
SpiritBool AckReceived = S_FALSE;
DisplayFrame_t TFTDisplayFrame;
SpiritBool InstallationReqSent = S_FALSE;
SpiritBool InstallationConfirmSent = S_FALSE;
static volatile SpiritBool AccessDemandReqTransmitted = S_FALSE;
static volatile SpiritBool ValidDataReqReceived = S_FALSE;
static volatile SpiritBool TimeoutOcurred =S_FALSE;
SpiritBool LinkAttrChanged = S_FALSE;
SpiritBool NewUserDataRequestRxd = S_FALSE;
SpiritBool SendUserDataRequestRxd = S_FALSE;
SpiritBool NewAlarmDataRequestRxd = S_FALSE;
SpiritBool ReqRespondClass2DataTxd = S_FALSE;
SpiritBool ReqRespondClass1DataTxd = S_FALSE;
SpiritBool ResetRemoteLinkTxd = S_FALSE;
SpiritBool RespondUserDataTxd = S_FALSE;
SpiritBool SendUserDataTxd = S_FALSE;
SpiritBool AccessDemandNoReplyTxd = S_FALSE;
SpiritBool LinkResetReceived = S_FALSE;
SpiritBool ACKTransmitted = S_FALSE;
SpiritBool NRDataTransmitted = S_FALSE;

extern SpiritBool bNewUserDataAvailable;
extern SpiritBool bNewAlertDataAvailable;

Indication gindicFrame;
uint8_t indicationPayload[255];
LastEventLog_t AppliLastEvent;

/*Some state variables used for encryption*/
/*As of now all these fields are 0, and will be implemented 
as and when required*/
uint8_t CommCtrl_Field=0;
uint32_t Session_Field=0;
uint16_t FrameNumber_Field =0;
uint64_t Rev_METER_ID =0x00;
extern uint64_t EEPROM_MTR_ID;
extern uint8_t EEpromMtrData[25];
extern uint8_t CipherBlocklen;
extern uint8_t ConcDataBaseBuff[10][26];
extern uint8_t MTR_EnKey_TO_EEPROM[16];
extern uint8_t METER_Enkey_DB[26];
extern uint8_t manufIdBuff[8];
extern uint64_t accNum;

uint8_t DataToUSB[260] = {0x00};
/*Initialization vctr in the Meter Side*/
uint8_t AES_InitnalizationVector_Tx[64]={0};    // was 16

/*Initialization vctr in the Concentrator Side*/
uint8_t AES_InitnalizationVector_Rx[64]={0};    //was 16


EncryptionKey_t EncryptionKey=
{
  /*Encription key Configuartion*/
  .EnKey ={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00},/*concentartor*/
  .KeyType = 0x00,
  .keySeqNum = 0x00  
};
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
SpiritBool WMBus_AppliUpdateMeterDatabase(Frame *frame);
static void WMBus_AppliIndicaCallback(Indication *indic, LastEventLog_t LastEvent);
static void WMBus_AppliTxDataSentHandler(LINK_STATUS status,LastEventLog_t LastEvent);
void RTC_Config(void);
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
  uint8_t requestPayload[5];
  /*M A CC SN FN BC*/
  
#ifdef AES_CBC_ENABLE 
  int32_t tempAttr=0;
  WMBus_LinkGetAttribute(LINK_ATTR_ID_MFR_ID, (int32_t*)&(tempAttr));
  
  AES_InitnalizationVector_Tx[0]   = (uint8_t)(tempAttr>>8);
  AES_InitnalizationVector_Tx[1] = (uint8_t)(tempAttr);
  WMBus_LinkGetAttribute(LINK_ATTR_ID_IDENTIFICATION, &tempAttr);
  AES_InitnalizationVector_Tx[5] = (uint8_t)tempAttr;
  AES_InitnalizationVector_Tx[4] = (uint8_t)(tempAttr>>8);
  AES_InitnalizationVector_Tx[3] = (uint8_t)(tempAttr>>16);
  AES_InitnalizationVector_Tx[2] = (uint8_t)(tempAttr>>24);
  WMBus_LinkGetAttribute(LINK_ATTR_ID_METER_VERSION, &tempAttr);
  AES_InitnalizationVector_Tx[6] = (uint8_t)tempAttr;
  WMBus_LinkGetAttribute(LINK_ATTR_ID_METER_TYPE, &tempAttr);  
  AES_InitnalizationVector_Tx[7] = (uint8_t)tempAttr;
  AES_InitnalizationVector_Tx[8] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[9] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[10] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[11] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[12] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[13] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[14] = (uint8_t)accNum;
  AES_InitnalizationVector_Tx[15] = (uint8_t)accNum;
  
  WMBus_ReadMeterEnkeyFromEEPROM(EEPROM_MTR_EnKey_SADDRESS, 16, EEPROM_MTR_EnKey_EADDRESS);
  memcpy(EncryptionKey.EnKey,&METER_Enkey_DB[0],16);
  WMBus_AppliEncrypt(EncryptionKey.EnKey, buff, length);
#endif
  
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
        request.frame.data_field.capacity      = 255;
#ifdef AES_CBC_ENABLE        
        request.frame.data_field.size          = CipherBlocklen;
        request.frame.data_field.header.config_word = 0x0150;
#else       
        
        request.frame.data_field.size          = length;
#endif
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
              response.frame.data_field.header.status_field = 0x00;
              response.frame.data_field.header.config_word = 0x0150;
              response.frame.data_field.payload   = buff; 
              response.frame.data_field.capacity  = sizeof(buff);
#ifdef AES_CBC_ENABLE       
              response.frame.data_field.size          = CipherBlocklen;
              response.frame.data_field.header.config_word = 0x0150;
#else
              response.frame.data_field.size      = length;
#endif
              response.positive = S_TRUE;
              
              response.frame.c_field                 = LINK_RESPOND_USER_DATA;
              response.frame.ci_field                = 0x7A;
              
              SdkDelayMs(10);
              
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
  
  
  return status;
}

/**
* @brief  This function will poll for the requests coming from the meters.
* @param  Param:None. 
* @retval None:
*                       
*/
LINK_STATUS WMBus_AppliPollRequests(void)
{
  LINK_STATUS status;
  Indication indication;
  /* Set receive timeout  */
  WMBus_LinkGetAttribute(LINK_ATTR_ID_RX_TIMEOUT,&indication.RxTimeout);
  indication.RxTimeout = 10; 
  indication.frame.data_field.payload    = indicationPayload;
  indication.frame.data_field.capacity   = sizeof(indicationPayload);
  indication.frame.data_field.size       = 0;
  /***************   Block mode Application Example Starts here    **************/
  ValidDataReqReceived = S_FALSE;
  TimeoutOcurred =S_FALSE;
  AppliLastEvent.event = NO_EVENT;
  if(LINK_STATE_IDLE == WMBus_LinkServicesCurrState())
  {
    status = WMBus_LinkServicesIndicationCallbackMode(&indication, WMBus_AppliIndicaCallback);
    while ((S_FALSE == ValidDataReqReceived )&&(S_FALSE == TimeoutOcurred));
    if((REQUEST_RESPOND_ACCESS_DEMAND_RXD == AppliLastEvent.event)&&(DATA_COMM_MODE == WMBusOperationMode))
    {
      Response response;
      Request request;
      uint8_t requestPayload[50];
      response.positive =S_TRUE;
      response.frame.data_field.payload       = requestPayload;
      response.frame.data_field.capacity      = sizeof(requestPayload);
      response.frame.data_field.size          = 8;
      InstallationConfirmSent = S_FALSE;
      /* Prepare the frame for transmission , For S1, S1m and T1 mode */
      response.frame.c_field                 = LINK_CONFIRM_ACK;
      response.frame.ci_field   = 0x00;
      response.frame.data_field.payload[0] = gindicFrame.frame.m_field[0];
      response.frame.data_field.payload[1] = gindicFrame.frame.m_field[1];
      response.frame.data_field.payload[2] = gindicFrame.frame.a_field[0];
      response.frame.data_field.payload[3] = gindicFrame.frame.a_field[1];
      response.frame.data_field.payload[4] = gindicFrame.frame.a_field[2];
      response.frame.data_field.payload[5] = gindicFrame.frame.a_field[3];
      response.frame.data_field.payload[6] = gindicFrame.frame.a_field[4];
      response.frame.data_field.payload[7] = gindicFrame.frame.a_field[5];
      /***********  Non-Block Mode Installation Request Starts Here    ************/
      
      SdkDelayMs(4);
            
      ACKTransmitted = S_FALSE;
      WMBus_LinkServicesResponseCallbackMode(&gindicFrame, &response, WMBus_AppliTxDataSentHandler);
      while(S_FALSE == ACKTransmitted);
      
      request.frame.data_field.payload       = requestPayload;
      request.frame.data_field.capacity      = sizeof(requestPayload);
      request.frame.data_field.size          = 0;
      /* Prepare the frame for transmission  */
      request.frame.c_field                 = LINK_REQUEST_RESPOND_CLASS2_DATA;
      request.retries = 1;
      request.frame.ci_field                = 0x7A;
      
      SdkDelayMs(4);
      
      /**************** Non-Block Mode Example Starts here    *****************/
      do {
        status = WMBus_LinkServicesRequestsCallbackMode(&request, WMBus_AppliTxDataSentHandler);
      } while ((status != LINK_STATUS_SUCCESS));
      while(REQUEST_RESPOND_CLASS2_DATA_TXD != AppliLastEvent.event);
      ValidDataReqReceived = S_FALSE;
      TimeoutOcurred = S_FALSE;
      indication.RxTimeout = 10;
      status = WMBus_LinkServicesIndicationCallbackMode(&indication, WMBus_AppliIndicaCallback);
      while ((S_FALSE == ValidDataReqReceived )&&(S_FALSE == TimeoutOcurred));
      if(S_TRUE == ValidDataReqReceived)
      {
        ValidDataReqReceived = S_FALSE;
      }
      //      SdkDelayMs(50);
    }
    if((PRIMARY_INSTALLATION_REQ_RXD == AppliLastEvent.event)&&(INSTALL_MODE == WMBusOperationMode))
    {
      Response response;
      uint8_t requestPayload[50];
      response.positive =S_TRUE;
      response.frame.data_field.payload       = requestPayload;
      response.frame.data_field.capacity      = sizeof(requestPayload);
      response.frame.data_field.size          = 8;
      InstallationConfirmSent = S_FALSE;
      /* Prepare the frame for transmission , For S1, S1m and T1 mode */
      response.frame.c_field                 = LINK_CONFIRM_INSTALLATION;
      response.frame.ci_field   = 0x8A;
      response.frame.data_field.payload[0] = gindicFrame.frame.m_field[0];
      response.frame.data_field.payload[1] = gindicFrame.frame.m_field[1];
      response.frame.data_field.payload[2] = gindicFrame.frame.a_field[0];
      response.frame.data_field.payload[3] = gindicFrame.frame.a_field[1];
      response.frame.data_field.payload[4] = gindicFrame.frame.a_field[2];
      response.frame.data_field.payload[5] = gindicFrame.frame.a_field[3];
      response.frame.data_field.payload[6] = gindicFrame.frame.a_field[4];
      response.frame.data_field.payload[7] = gindicFrame.frame.a_field[5];
      /***********  Non-Block Mode Installation Request Starts Here    ************/
      
      SdkDelayMs(4);
      
      WMBus_LinkServicesResponseCallbackMode(&gindicFrame, &response, WMBus_AppliTxDataSentHandler);
      while (S_TRUE != InstallationConfirmSent);
      
      WMBusDatabaseFrame_t WMBusDatabaseFrame;
      WMBusDatabaseFrame.MeterID = gindicFrame.frame.m_field[1];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.m_field[0];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.a_field[3];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.a_field[2];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.a_field[1];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.a_field[0];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.a_field[4];
      WMBusDatabaseFrame.MeterID = (WMBusDatabaseFrame.MeterID<<8) | gindicFrame.frame.a_field[5];
      WMBusDatabaseFrame.SerialNum.GROUP_ID =0x00;
      WMBus_AppliAddMeter(WMBusDatabaseFrame);
    }
  }
  /**********  Non-Blocking  mode Application Example Ends here    **************/
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
  if(SdkEvalGetVersion()==SDK_EVAL_VERSION_2_1)
  {
    RTC_Config();
    
    if(SUCCESS == RTC_Init(&RTC_InitStructVal))
    { 
      RTC_SetDate(RTC_Format_BCD, &RTC_DateStructVal);
      RTC_SetTime(RTC_Format_BCD, &RTC_TimeStructVal);
    }
  }
  /*  Init Meter database */
  WMBus_AppliPowerOn();  
  if(OTHER == WMBus_GetWMBusDeviceType())
  { 
    
    rootDataBase = (struct DataBaseList*)malloc(sizeof(struct DataBaseList));
    rootDataBase->next = NULL;
    
    rootDataBase->MeterDataFrame.MeterID = manufIdBuff[0];/*m_field[0]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[1];/*m_field[1]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[2];/*a_field[3]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[3];/*a_field[2]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[4];/*a_field[1]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[5];/*a_field[0]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[6];/*a_field[4]*/
    rootDataBase->MeterDataFrame.MeterID = (rootDataBase->MeterDataFrame.MeterID<<8) | manufIdBuff[7];/*a_field[5]*/
    
    rootDataBase->MeterDataFrame.SerialNum.SERIAL_NUM = 0;
    rootDataBase->MeterDataFrame.SerialNum.VALIDITY = 1;
    rootDataBase->MeterDataFrame.SerialNum.GROUP_ID = 0;
    rootDataBase->MeterDataFrame.DeviceType = 0x00;
    memset(rootDataBase->MeterDataFrame.TimeStamp, 0x00,6);
    memset(rootDataBase->MeterDataFrame.Payload, 0x00,DATA_PAYLOAD_LENGTH);
    memset(rootDataBase->MeterDataFrame.EncrypKey, 0x00,16);
    rootDataBase->MeterDataFrame.KeyType = 0x00;
    rootDataBase->MeterDataFrame.Status = 0x00;
    
  }   
}
/**
* @brief  This function will handle the received data from the link layer.
* @param  Frame frame: The pointer to the link frame. 
* @retval None.
*                       
*/
void WMBus_AppliRxDataHandler(Frame *frame)
{
  
#ifdef CLI
  responsePrintf("{&N API callback...\r\n", "WMBus_AppliRxDataHandler");
#endif
#ifndef CLI
  int32_t value;
#endif
  if(DATA_COMM_MODE == WMBusOperationMode)
  {
    
    if(NULL == frame)
    {
#ifdef CLI
      /*  Timeout Error Ocurred */
      printf ("RX timeout\r\n");
      responsePrintf("    &tx\r\n", "timeout", 1);
#endif
    }
    else
    {
#ifdef AES_CBC_ENABLE 
      //Decryption to take place here 
      Rev_METER_ID = frame->m_field[1];//msb of manufacture id 
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->m_field[0];//lsb of manufacure id
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->a_field[3];
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->a_field[2];
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->a_field[1];
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->a_field[0];
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->a_field[4];
      Rev_METER_ID = (Rev_METER_ID<<8)|frame->a_field[5];
      
      
      AES_InitnalizationVector_Rx[0] = frame->m_field[1]; //msb of manufacture id 
      AES_InitnalizationVector_Rx[1] = frame->m_field[0]; //lsb of manufacure id
      AES_InitnalizationVector_Rx[2] = frame->a_field[3];
      AES_InitnalizationVector_Rx[3] = frame->a_field[2];
      AES_InitnalizationVector_Rx[4] = frame->a_field[1];
      AES_InitnalizationVector_Rx[5] = frame->a_field[0];
      AES_InitnalizationVector_Rx[6] = frame->a_field[4];
      AES_InitnalizationVector_Rx[7] = frame->a_field[5];
      AES_InitnalizationVector_Rx[8] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[9] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[10] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[11] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[12] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[13] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[14] = frame->data_field.header.accessNum;
      AES_InitnalizationVector_Rx[15] = frame->data_field.header.accessNum;
      
      Read_EEPROM_Mtr_DataBase(Rev_METER_ID);
      
      /*rest of the fileds of the init vector are zero as of now*/
      
      WMBus_AppliDecrypt(EncryptionKey.EnKey, frame->data_field.payload, frame->data_field.size);
      
      //end of decryption
#endif
#ifdef CLI
      responsePrintf("    &tx\r\n", "timeout", 0);
      responsePrintf("    &tx\r\n", "filtered", WMBus_AppliUpdateMeterDatabase(frame) ? 1 : 0);
      responsePrintf("    &tx\r\n", "c", frame->c_field);
      responsePrintf("    &t2x\r\n", "m", (frame->m_field[1] << 8) + frame->m_field[0] );
      
      responsePrintf("    {%t: ","a");
      { int i; for (i = 2; i < 6; i++) {
        responsePrintf("%x",frame->a_field[5-i]);
      }; 
      responsePrintf("%x",frame->a_field[4]);
      responsePrintf("%x",frame->a_field[5]);
      responsePrintf("}\r\n"); }
      
      responsePrintf("    &tx\r\n", "ci", frame->ci_field);
      
      responsePrintf("    {%t: ","data");
      int i; for (i = 0; i < frame->data_field.size; i++) {
        responsePrintf("%x",frame->data_field.payload[i]);
      }; 
      responsePrintf("}\r\n"); 
#endif
      
#ifndef CLI
      WMBus_AppliUpdateMeterDatabase(frame);
      uint8_t installed = WMBus_AppliUpdateMeterDatabase(frame);
      WMBus_LinkGetAttribute(LINK_ATTR_ID_DEV_TYPE, &value);
      if(OTHER == (WMBusDeviceType_T)value)
      {
        TFTDisplayFrame.installed = installed;
        
        uint16_t tempVar;
        tempVar = frame->m_field[1];
        tempVar <<= 8;
        tempVar |= frame->m_field[0];
        TFTDisplayFrame.MeterMFR[2] = tempVar%32+0x40;
        tempVar -= (tempVar%32);
        tempVar /=32;
        TFTDisplayFrame.MeterMFR[1] = (tempVar%32)+0x40;   
        TFTDisplayFrame.MeterMFR[0] = (uint8_t)(tempVar/32)+0x40;
        TFTDisplayFrame.MeterMFR[3]=0x00;
        
        TFTDisplayFrame.A_Filed[1] = (frame->a_field[0]&0x0F)+0x30;
        TFTDisplayFrame.A_Filed[0] = ((frame->a_field[0]>>4)&0x0F)+0x30;
        
        TFTDisplayFrame.A_Filed[3] = (frame->a_field[1]&0x0F)+0x30;
        TFTDisplayFrame.A_Filed[2] = ((frame->a_field[1]>>4)&0x0F)+0x30;
        
        TFTDisplayFrame.A_Filed[5] = (frame->a_field[2]&0x0F)+0x30;
        TFTDisplayFrame.A_Filed[4] = ((frame->a_field[2]>>4)&0x0F)+0x30;
        
        TFTDisplayFrame.A_Filed[7] = (frame->a_field[3]&0x0F)+0x30;
        TFTDisplayFrame.A_Filed[6] = ((frame->a_field[3]>>4)&0x0F)+0x30;
        
        TFTDisplayFrame.A_Filed[9] = (frame->a_field[4]&0x0F)+0x30;
        TFTDisplayFrame.A_Filed[8] = ((frame->a_field[4]>>4)&0x0F)+0x30;  
        
        TFTDisplayFrame.A_Filed[11] = (frame->a_field[5]&0x0F)+0x30;
        TFTDisplayFrame.A_Filed[10] = ((frame->a_field[5]>>4)&0x0F)+0x30;  
        TFTDisplayFrame.A_Filed[12] = 0x00;  
        
        TFTDisplayFrame.DeviceType[0] = TFTDisplayFrame.A_Filed[10];
        TFTDisplayFrame.DeviceType[1] = TFTDisplayFrame.A_Filed[11];
        TFTDisplayFrame.DeviceType[2] =0;  
        TFTDisplayFrame.Payload[0] = ((frame->data_field.payload[2]>>4)\
          &0x0F)>9?((frame->data_field.payload[2]>>4)&0x0F)+0x37:\
            ((frame->data_field.payload[2]>>4)&0x0F)+0x30;
            TFTDisplayFrame.Payload[1] = ((frame->data_field.payload[2])&0x0F)\
              >9?((frame->data_field.payload[2])&0x0F)+0x37:\
                ((frame->data_field.payload[2])&0x0F)+0x30;
                
                TFTDisplayFrame.Payload[2] = ((frame->data_field.payload[3]>>4)\
                  &0x0F)>9?((frame->data_field.payload[3]>>4)&0x0F)+0x37:\
                    ((frame->data_field.payload[3]>>4)&0x0F)+0x30;
                    TFTDisplayFrame.Payload[3] = ((frame->data_field.payload[3])\
                      &0x0F)>9?((frame->data_field.payload[3])&0x0F)+0x37:\
                        ((frame->data_field.payload[3])&0x0F)+0x30;
                        
                        TFTDisplayFrame.Payload[4] = ((frame->data_field.payload[4]>>4)\
                          &0x0F)>9?((frame->data_field.payload[4]>>4)&0x0F)+0x37:\
                            ((frame->data_field.payload[4]>>4)&0x0F)+0x30;
                            TFTDisplayFrame.Payload[5] = ((frame->data_field.payload[4])\
                              &0x0F)>9?((frame->data_field.payload[4])&0x0F)+0x37:\
                                ((frame->data_field.payload[4])&0x0F)+0x30;
                                
                                TFTDisplayFrame.Payload[6] = ((frame->data_field.payload[5]>>4)\
                                  &0x0F)>9?((frame->data_field.payload[5]>>4)&0x0F)+0x37:\
                                    ((frame->data_field.payload[5]>>4)&0x0F)+0x30;
                                    TFTDisplayFrame.Payload[7] = ((frame->data_field.payload[5])\
                                      &0x0F)>9?((frame->data_field.payload[5])&0x0F)+0x37:\
                                        ((frame->data_field.payload[5])&0x0F)+0x30;
                                        
                                        TFTDisplayFrame.Payload[8] = ' ';
                                        TFTDisplayFrame.Payload[9] = 'u';
                                        TFTDisplayFrame.Payload[10] = 'n';
                                        TFTDisplayFrame.Payload[11] = 'i';
                                        TFTDisplayFrame.Payload[12] = 't';
                                        TFTDisplayFrame.Payload[13] = 's';
      }
#endif
      bNewdatareceived =S_TRUE;
      /* Check the type of frame received and trigger the necessary actions here*/
    }
#ifdef CLI
    responsePrintf("}\r\n");
#endif
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
  request.retries = 3;
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
* @brief  This function will set the Encryption key.
* @param  EncryptionKey_t keydata: The Encryption key. 
*         void *mfrID: The pointer to MFR IF+A-Field String.
* @retval None.
*                       
*/
void WMBus_AppliSetEncryptKey(void *mfrID,EncryptionKey_t keydata)
{  
  /* int32_t value;*/
  WMBus_WriteMeterDatabaseToEEPROM(keydata.EnKey, 16, EEPROM_MTR_EnKey_SADDRESS);
  
  memcpy(&EncryptionKey.EnKey[0],keydata.EnKey,16);
  
  EncryptionKey.KeyType = keydata.KeyType;
  EncryptionKey.keySeqNum = keydata.keySeqNum;
}
/**
* @brief  This function will handle meter read request from the PC-GUI.
* @param  WMBusDatabaseFrame_t *dataframe: The pointer to meter database frame. 
* @retval uint8_t: length of the payload
*                       
*/
uint8_t WMBus_AppliReadMeter(uint64_t meterID, uint8_t *buff)
{
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    struct DataBaseList *travPtr;
    travPtr = rootDataBase;
    uint8_t* pu8_RSSITempptr;
    for( ;((NULL != travPtr->next)&&(travPtr->MeterDataFrame.MeterID != meterID));travPtr = travPtr->next);
    if(travPtr->MeterDataFrame.MeterID == meterID)
    {
      *buff = 0x00;/*S.No*/
      *(buff+1) = 0x00;/*S.No*/
      *(buff+3) = (uint8_t)meterID;
      *(buff+4) = (uint8_t)(meterID>>8);
      *(buff+5) = (uint8_t)(meterID>>16);
      *(buff+6) = (uint8_t)(meterID>>24);
      *(buff+7) = (uint8_t)(meterID>>32);
      *(buff+8) = (uint8_t)(meterID>>40);
      *(buff+9) = (uint8_t)(meterID>>48);
      *(buff+10) = (uint8_t)(meterID>>56);
      *(buff+2) = (uint8_t)travPtr->MeterDataFrame.SerialNum.GROUP_ID;
      *(buff+11) = (uint8_t)(meterID);
      *(buff+12) = (uint8_t)travPtr->MeterDataFrame.TimeStamp[0];
      *(buff+13) = (uint8_t)travPtr->MeterDataFrame.TimeStamp[1];
      *(buff+14) = (uint8_t)travPtr->MeterDataFrame.TimeStamp[2];
      *(buff+15) = (uint8_t)travPtr->MeterDataFrame.TimeStamp[3];
      *(buff+16) = (uint8_t)travPtr->MeterDataFrame.TimeStamp[4];
      *(buff+17) = (uint8_t)travPtr->MeterDataFrame.TimeStamp[5];
      
      float f_RSSI_temp = (travPtr->MeterDataFrame.RSSI_field);
      int temo_int = (int)(f_RSSI_temp*10);
      pu8_RSSITempptr = (uint8_t*)&temo_int;
      /*RSSI field 57th to 60th bytes in payload*/
      *(buff+57)= *pu8_RSSITempptr;
      *(buff+58)= *(pu8_RSSITempptr+1);
      *(buff+59)= *(pu8_RSSITempptr+2);
      *(buff+60)= *(pu8_RSSITempptr+3);
      
      *(buff+61) = (uint8_t)travPtr->MeterDataFrame.Status;
      
      /*-------------------data to GUI----------------------------------------------*/  
      *(buff+19) = (uint8_t)(travPtr->MeterDataFrame.Payload[2]&0x0F);
      *(buff+19) += ((*(buff+19))>9)?55:0x30;
      
      *(buff+18) = (uint8_t)(((uint8_t)(travPtr->MeterDataFrame.Payload[2]>>4))&0x0F);
      *(buff+18) += ((*(buff+18))>9)?55:0x30;
      
      *(buff+21) = (uint8_t)(travPtr->MeterDataFrame.Payload[3]&0x0F);
      *(buff+21) += ((*(buff+21))>9)?55:0x30;
      
      *(buff+20) = (uint8_t)(((uint8_t)(travPtr->MeterDataFrame.Payload[3]>>4))&0x0F);
      *(buff+20) += ((*(buff+20))>9)?55:0x30;
      
      
      *(buff+23) = (uint8_t)(travPtr->MeterDataFrame.Payload[4]&0x0F);
      *(buff+23) += ((*(buff+23))>9)?55:0x30;
      
      *(buff+22) = (uint8_t)(((uint8_t)(travPtr->MeterDataFrame.Payload[4]>>4))&0x0F);
      *(buff+22) += ((*(buff+22))>9)?55:0x30;
      
      *(buff+25) = (uint8_t)(travPtr->MeterDataFrame.Payload[5]&0x0F);
      *(buff+25) += ((*(buff+25))>9)?55:0x30;
      
      *(buff+24) = (uint8_t)(((uint8_t)(travPtr->MeterDataFrame.Payload[5]>>4))&0x0F);
      *(buff+24) += ((*(buff+24))>9)?55:0x30;      
      
      *(buff+26) = 0x3B;
      
      return S_TRUE;
    }
  } 
  return 0x00;
}
/**
* @brief  This function will return the board Manufacturer ID(MFR+IDENTIFICATION).
* @param  uint8_t *buff: The uint8_t array that contains the Board MFR ID. 
* @retval None.
*                       
*/
void WMBus_AppliReadMeterAttr(uint64_t MeterIDTemp, uint8_t *tempBuff)
{
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    struct DataBaseList *travPtr;
    travPtr = rootDataBase->next;
    for( ;((NULL != travPtr->next)&&(travPtr->MeterDataFrame.MeterID != MeterIDTemp));travPtr = travPtr->next);
    if(travPtr->MeterDataFrame.MeterID == MeterIDTemp)
    {
      *tempBuff     = 0x03;
      *(tempBuff+2) = (uint8_t)MeterIDTemp;
      *(tempBuff+3) = (uint8_t)(MeterIDTemp>>8);
      *(tempBuff+4) = (uint8_t)(MeterIDTemp>>16);
      *(tempBuff+5) = (uint8_t)(MeterIDTemp>>24);
      *(tempBuff+6) = (uint8_t)(MeterIDTemp>>32);
      *(tempBuff+7) = (uint8_t)(MeterIDTemp>>40);
      *(tempBuff+8) = (uint8_t)(MeterIDTemp>>48);
      *(tempBuff+9) = (uint8_t)(MeterIDTemp>>56);
      
      *(tempBuff+1) = (uint8_t)travPtr->MeterDataFrame.SerialNum.GROUP_ID;
      *(tempBuff+10) = (uint8_t)(travPtr->MeterDataFrame.DeviceType);
      memcpy(tempBuff+11, travPtr->MeterDataFrame.EncrypKey,16);
      *(tempBuff+27) = (uint8_t)(travPtr->MeterDataFrame.KeyType);
      *(tempBuff+28) = 0x00;/* Key Sequence Number*/
    }
  }
}
/**
* @brief  This function will return the encryption key.
* @param  EncryptionKey_t *keydata: The pointer to Encryption key string. 
* @retval None:
*                       
*/
void WMBus_AppliGetEncryptKey(EncryptionKey_t *keydata)
{
  WMBus_ReadMeterEnkeyFromEEPROM(EEPROM_MTR_EnKey_SADDRESS, 16, EEPROM_MTR_EnKey_EADDRESS);
  memcpy(keydata->EnKey, &METER_Enkey_DB[0], 16);
  keydata->KeyType = METER_Enkey_DB[16];
  keydata->keySeqNum = METER_Enkey_DB[17];
}

/**
* @brief  This function will add meter to the database.
* @param  WMBusDatabaseFrame_t databaseFrame :The meter parameters. 
* @retval SpiritBool.
*                       
*/
SpiritBool WMBus_AppliAddMeter(WMBusDatabaseFrame_t databaseFrame)
{  
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    struct DataBaseList *travPtr, *newPtr;
    travPtr = rootDataBase;
    for( ;((NULL != travPtr->next)&&(travPtr->MeterDataFrame.MeterID != databaseFrame.MeterID));travPtr = travPtr->next);
    if(NULL == travPtr->next)
    {
      newPtr = (struct DataBaseList*)malloc(sizeof(struct DataBaseList));
      newPtr->next = NULL;
      newPtr->MeterDataFrame.MeterID= databaseFrame.MeterID;
      newPtr->MeterDataFrame.SerialNum.SERIAL_NUM = 0;
      newPtr->MeterDataFrame.SerialNum.VALIDITY = 1;
      newPtr->MeterDataFrame.SerialNum.GROUP_ID = databaseFrame.SerialNum.GROUP_ID;
      newPtr->MeterDataFrame.DeviceType = (uint8_t)databaseFrame.MeterID;
      newPtr->MeterDataFrame.KeyType = databaseFrame.KeyType;
      newPtr->MeterDataFrame.Status = databaseFrame.Status;
      memcpy(newPtr->MeterDataFrame.EncrypKey, &databaseFrame.EncrypKey, 16);
      travPtr->next = newPtr;
      
      memset(newPtr->MeterDataFrame.Payload, 0x00, DATA_PAYLOAD_LENGTH);
      memset(newPtr->MeterDataFrame.TimeStamp, 0x00, 6);
      
      return S_TRUE;
    }
    else if(travPtr->MeterDataFrame.MeterID == databaseFrame.MeterID)
    {
      return S_TRUE;
    }
    else
    {
      return S_FALSE;
    }
  }
  else
    return S_FALSE;
}
/**
* @brief  This function will delete the meter from the database.
* @param  uint64_t meterid: The meter ID. 
* @retval SpiritBool.
*                       
*/
SpiritBool WMBus_AppliDeleteMeter(uint64_t meterid)
{
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    struct DataBaseList *travPtr, *prePtr;
    travPtr = rootDataBase;
    for(;((NULL != travPtr->next)&&(travPtr->MeterDataFrame.MeterID != meterid));travPtr = travPtr->next)
      prePtr = travPtr;
    if(travPtr->MeterDataFrame.MeterID == meterid)
    {
      prePtr->next = travPtr->next;
      free(travPtr);
      return S_TRUE;
    }
    else if(NULL == travPtr->next)
    {
      return S_TRUE;
    }
  }
  return S_FALSE;
}

/**
* @brief  This function will update the meter attributes(This will not add meter).
* @param  WMBusDatabaseFrame_t databaseFrame: The database frame contains meter attriute. 
* @retval SpiritBool.
*                       
*/
SpiritBool WMBus_AppliUpdateMeterAttr(WMBusDatabaseFrame_t databaseFrame)
{
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    struct DataBaseList *travPtr;
    travPtr = rootDataBase->next;
    for( ;((NULL != travPtr->next)&&(travPtr->MeterDataFrame.MeterID != databaseFrame.MeterID));travPtr = travPtr->next);
    if(travPtr->MeterDataFrame.MeterID == databaseFrame.MeterID)
    {
      travPtr->MeterDataFrame.MeterID= databaseFrame.MeterID;
      travPtr->MeterDataFrame.SerialNum.SERIAL_NUM = 0;
      travPtr->MeterDataFrame.SerialNum.VALIDITY = 1;
      travPtr->MeterDataFrame.SerialNum.GROUP_ID = databaseFrame.SerialNum.GROUP_ID;
      travPtr->MeterDataFrame.DeviceType = databaseFrame.DeviceType;
      travPtr->MeterDataFrame.KeyType = databaseFrame.KeyType;
      travPtr->MeterDataFrame.Status = databaseFrame.Status;
      memcpy(travPtr->MeterDataFrame.EncrypKey, &databaseFrame.EncrypKey, 16);
      return S_TRUE;
    }
    else
    {
      return S_FALSE;
    }
  }
  else
    return S_FALSE;
}
/**
* @brief  This function will return the number of meters in the group.
* @param  uint8_t groupid: The group ID. 
* @retval uint16_t: The number of meters in the group.
*                       
*/
uint16_t WMBus_AppliGetGroupAttr(uint8_t groupid)
{
  uint16_t numOfMeters=0x00;
  if(OTHER == WMBus_GetWMBusDeviceType())
  {    
    struct DataBaseList *travPtr;
    travPtr = rootDataBase->next;
    if(0x00 == groupid)
    {
      while(NULL != travPtr)
      { 
        numOfMeters++;
        travPtr = travPtr->next;      
      }
    }
    else
    {
      while(NULL != travPtr)
      {
        if(groupid == (uint8_t)travPtr->MeterDataFrame.SerialNum.GROUP_ID)
        {          
          numOfMeters++;
        }      
        travPtr = travPtr->next;      
      }
    }
  }
  return numOfMeters;
}

/**
* @brief  This function will fill update the meter ID and Group ID in the 
*         parameter passed and also return the number of meters in the database.
* @param  MeterDatabase_t *pdatabase:The pointer to the array of MeterDatabase_t type. 
* @retval uint16_t: The number of the meters in the database.
*                       
*/
uint16_t WMBus_AppliGetGroupMembers(MeterDatabase_t *pDatabase, uint8_t groupid)
{
  uint16_t numOfMeters=0x00;
  if(OTHER == WMBus_GetWMBusDeviceType())
  {    
    struct DataBaseList *travPtr;
    travPtr = rootDataBase->next;
    while(NULL != travPtr)
    {
      if(groupid == (uint8_t)travPtr->MeterDataFrame.SerialNum.GROUP_ID)
      {
        pDatabase->GroupID = (uint8_t)travPtr->MeterDataFrame.SerialNum.GROUP_ID;
        pDatabase->MeterID = travPtr->MeterDataFrame.MeterID;
        numOfMeters++;
        pDatabase++;
      }      
      travPtr = travPtr->next;      
    }
  }
  return numOfMeters;
}

/**
* @brief  This function will fill update the meter ID and Group ID in the 
*         parameter passed and also return the number of meters in the database.
* @param  MeterDatabase_t *pdatabase:The pointer to the array of MeterDatabase_t type. 
* @retval uint16_t: The number of the meters in the database.
*                       
*/
uint16_t WMBus_AppliGetMeterList(MeterDatabase_t *pDatabase)
{
  uint16_t numOfMeters=0x00;
  if(OTHER == WMBus_GetWMBusDeviceType())
  {    
    struct DataBaseList *travPtr;
    travPtr = rootDataBase->next;
    while(NULL != travPtr)
    {
      pDatabase->GroupID = (uint8_t)travPtr->MeterDataFrame.SerialNum.GROUP_ID;
      pDatabase->MeterID = travPtr->MeterDataFrame.MeterID;
      pDatabase++;
      travPtr = travPtr->next;
      numOfMeters++;
    }
  }
  else
  {
    return S_FALSE;/*  Ignore the data*/
  }
  return numOfMeters;
}
/**
* @brief  This function will read and send all meter entry in eeprom of conc database to GUI.
* @param  none 
* @retval uint8_t*: ptr to the buffer returned.
*                       
*/
uint8_t* WMBus_AppliGetEEpromEntries(/*MeterDatabase_t *pDatabase*/)
{
  for(uint8_t Eindex=0; Eindex<10; Eindex++)
  {
    switch(Eindex)
    {
    case 0:
      WMBus_PhyReadMeterDatabase(EEPROM_METER1_DATABASE_START,26, EEPROM_METER1_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr] = EEpromMtrData[ctr];
      }
      break;
    case 1:
      WMBus_PhyReadMeterDatabase(EEPROM_METER2_DATABASE_START, 26, EEPROM_METER2_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+26] = EEpromMtrData[ctr];
      }            
      break;
    case 2:
      WMBus_PhyReadMeterDatabase(EEPROM_METER3_DATABASE_START, 26, EEPROM_METER3_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+52] = EEpromMtrData[ctr];
      }            
      break;
    case 3:
      WMBus_PhyReadMeterDatabase(EEPROM_METER4_DATABASE_START, 26, EEPROM_METER4_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+78] = EEpromMtrData[ctr];
      }            
      break;
    case 4:
      WMBus_PhyReadMeterDatabase(EEPROM_METER5_DATABASE_START, 26, EEPROM_METER5_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+104] = EEpromMtrData[ctr];
      }            
      break;
    case 5:
      WMBus_PhyReadMeterDatabase(EEPROM_METER6_DATABASE_START, 26, EEPROM_METER6_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+130] = EEpromMtrData[ctr];
      }            
      break;
    case 6:
      WMBus_PhyReadMeterDatabase(EEPROM_METER7_DATABASE_START, 26, EEPROM_METER7_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+156] = EEpromMtrData[ctr];
      }            
      break;
    case 7:
      WMBus_PhyReadMeterDatabase(EEPROM_METER8_DATABASE_START, 26, EEPROM_METER8_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+182] = EEpromMtrData[ctr];
      }            
      break;
    case 8:
      WMBus_PhyReadMeterDatabase(EEPROM_METER9_DATABASE_START, 26, EEPROM_METER9_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+208] = EEpromMtrData[ctr];
      }            
      break;
    case 9:
      WMBus_PhyReadMeterDatabase(EEPROM_METER_10_DATABASE_START, 26, EEPROM_METER_10_DATABASE_END);
      for(uint8_t ctr=0; ctr<=25; ctr++)
      {
        DataToUSB[ctr+234] = EEpromMtrData[ctr];
      }            
      break;    
    default:
      break;
    }
  }
  return (unsigned char*)DataToUSB;
}

/**
* @brief  This function will return the board Manufacturer ID(MFR+IDENTIFICATION).
* @param  uint8_t *buff: The uint8_t array that contains the Board MFR ID. 
* @retval None.
*                       
*/
void WMBus_GetBoardMnfID(uint8_t *buff)
{
  WMBus_ReadMeterEnkeyFromEEPROM(EEPROM_METER_ID_SADDRESS, 8, EEPROM_METER_ID_EADDRESS);
  memcpy(&buff[0], &METER_Enkey_DB[0], 8);  
}
/**
* @brief  This function will set the MFR ID(MFR+Identification field).
* @param  uint8_t *buff: The MFR ID in array format. 
* @retval SpiritBool:Returns S_TRUE if the operation succesful S_FALSE otherwise.
*                       
*/
SpiritBool WMBus_SetBoardMnfID(uint8_t *buff)
{
  WMBus_WriteMeterDatabaseToEEPROM(&buff[0], 8, EEPROM_METER_ID_SADDRESS);
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
* @brief  This function will return the current operation mode.
* @param  Param: none 
* @retval WMBusOperationMode_t
*                       
*/
WMBusOperationMode_t WMBus_AppliGetOperationMode(void)
{
  return WMBusOperationMode;
}
/**
* @brief  This function will set the current operation mode.
* @param  WMBusOperationMode_t currmode: The current operation mode. 
* @retval None.
*                       
*/
void WMBus_AppliSetOperationMode(WMBusOperationMode_t currmode)
{
  WMBusOperationMode = currmode;
  if(DATA_COMM_MODE == WMBusOperationMode)
  {
    SdkEvalLedOn(LED3);
  }
  else
  {
    SdkEvalLedOff(LED3);
  }
}
/**
* @brief  This function will update the data received from the Meters over WMBus
*  interface.This will ignore if the meter is not registered.
* @param  Frame *frame: The database frame contains meter data. 
* @retval SpiritBool.
*                       
*/
SpiritBool WMBus_AppliUpdateMeterDatabase(Frame *frame)
{
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    uint64_t MeterID;
    struct DataBaseList *travPtr;
    MeterID = frame->m_field[1];
    MeterID = (MeterID<<8) | frame->m_field[0];
    MeterID = (MeterID<<8) | frame->a_field[3];
    MeterID = (MeterID<<8) | frame->a_field[2];
    MeterID = (MeterID<<8) | frame->a_field[1];
    MeterID = (MeterID<<8) | frame->a_field[0];
    MeterID = (MeterID<<8) | frame->a_field[4];
    MeterID = (MeterID<<8) | frame->a_field[5];
    
    travPtr = rootDataBase->next;
    for( ;((NULL != travPtr)&&(travPtr->MeterDataFrame.MeterID != MeterID));travPtr = travPtr->next);
    if(travPtr->MeterDataFrame.MeterID == MeterID)
    {     
      travPtr->MeterDataFrame.DeviceType = frame->a_field[5];
      travPtr->MeterDataFrame.Status =0x00;
      travPtr->MeterDataFrame.RSSI_field = frame->RSSI_field;
#ifndef CLI
      uint32_t var1, var2;
      WMBus_GetTime(&var1, &var2);
      travPtr->MeterDataFrame.TimeStamp[0] = (uint8_t)var1;
      travPtr->MeterDataFrame.TimeStamp[1] = (uint8_t)(var1>>8);
      travPtr->MeterDataFrame.TimeStamp[2] = (uint8_t)(var1>>16);
      travPtr->MeterDataFrame.TimeStamp[3] = (uint8_t)var2;
      travPtr->MeterDataFrame.TimeStamp[4] = (uint8_t)(var2>>8);
      travPtr->MeterDataFrame.TimeStamp[5] = (uint8_t)(var2>>16); 
#endif      
      memcpy(travPtr->MeterDataFrame.Payload, frame->data_field.payload, \
        (frame->data_field.size));
      memset(&travPtr->MeterDataFrame.Payload[frame->data_field.size],0,\
        DATA_PAYLOAD_LENGTH-(frame->data_field.size));
      return S_TRUE;
    }
    else
    {
      return S_FALSE;/*  Ignore the data*/
    }
  }
  else
    return S_FALSE;
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

/**
* @brief  This function writes Meter database to internal EEPROM.
* @param  None.
* @retval None.
*/
void WMBus_AppliWriteMeterDatabase(void)
{
  if(OTHER == WMBus_GetWMBusDeviceType())
  {
    struct DataBaseList *travPtr;
    travPtr = rootDataBase->next;
    for(uint32_t offset=0x00 ;(NULL != travPtr->next);travPtr = travPtr->next)
    {
      WMBus_PhyWriteMeterDatabase((uint8_t*)&travPtr->MeterDataFrame, sizeof(WMBusDatabaseFrame_t), (offset * sizeof(WMBusDatabaseFrame_t)));
      offset++;
    }
  }
}

/**
* @brief  This function is called after invoking the indication service.
* @param  Indication: This indicates type of request and associated data.
* @retval None
*/
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
        SendUserDataRequestRxd = S_TRUE;
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
      NewAlarmDataRequestRxd = S_TRUE;
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
      SdkEvalLedOn(LED5);
      break;
    case LINK_CONFIRM_ACK:
      AckReceived = S_TRUE;
      break;
    case LINK_SEND_CONFIRM_RESET_REMOTE_LINK:/*(SND_NKE): conc->mtr*/
      LinkResetReceived = S_TRUE;
      SdkEvalLedToggle(LED4);
      break;
    default:
      ;
    }
  }
}
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
    switch(AppliLastEvent.event)
    {
    case PRIMARY_INSTALLATION_REQ_TXD:/*(SND-IR): mtr->conc*/
      InstallationReqSent = S_TRUE;
      break;
    case CONFIRM_INSTALLATION_SENT:/*(CNF-IR): conc->mtr*/
      SdkEvalLedOn(LED5);
      InstallationConfirmSent = S_TRUE;
      break;
    case REQUEST_RESPOND_ACCESS_DEMAND_TXD:/*(ACC-DMD): mtr->conc*/
      AccessDemandReqTransmitted = S_TRUE;
      break;
    case CONFIRM_ACK_SENT:/*(ACC-DMD): mtr->conc*/
      ACKTransmitted = S_TRUE;
      break;
    case REQUEST_RESPOND_CLASS2_DATA_TXD:/*(REQ-UD2): conc->mtr*/
      ReqRespondClass2DataTxd = S_TRUE;
      break;
    case REQUEST_RESPOND_CLASS1_DATA_TXD:/*(REQ-UD1): conc->mtr*/
      ReqRespondClass1DataTxd = S_TRUE;
      break;
    case SEND_NOREPLY_USER_DATA_TXD:/*(SND-NR): mtr->conc*/
      NRDataTransmitted = S_TRUE;
      break; 
    case REQUEST_NOREPLY_ACCESS_DEMAND_TXD:/*(ACC-NR): mtr->conc*/
      AccessDemandNoReplyTxd = S_TRUE;
      break;
    case SEND_USER_DATA_CMD_TXD:/*(SND-UD): conc->mtr*/
      SendUserDataTxd = S_TRUE;
      break;
    case SEND_CONFIRM_RESET_REMOTE_LINK_TXD:/*(SND-NKE): conc->mtr*/
      ResetRemoteLinkTxd = S_TRUE;
      break;
    case RESPOND_USER_DATA_SENT:/*(RSP-UD): mtr->conc*/
      RespondUserDataTxd = S_TRUE;
      break;     
    default:
      ;
    } 
  } 
}
#ifndef CLI
/**
* @brief  This function will Configure the RTC clock source.
* @param  None.
* @retval None.
*/
void RTC_Config(void)
{
  /* Enable The PWR Register */
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_PWR, ENABLE);
  
  /* Allow access to RTC */
  PWR_RTCAccessCmd(ENABLE);
  
  /* Reset RTC Domain */
  RCC_RTCResetCmd(ENABLE);
  RCC_RTCResetCmd(DISABLE);
  
  /*!< LSE Enable */
  RCC_LSEConfig(RCC_LSE_ON);
  
  /*!< Wait till LSE is ready */
  while (RCC_GetFlagStatus(RCC_FLAG_LSERDY) == RESET)
  {}
  
  /*!< LCD Clock Source Selection */
  RCC_RTCCLKConfig(RCC_RTCCLKSource_LSE);
  
  /* Enable the RTC Clock */
  RCC_RTCCLKCmd(ENABLE);
  
  /* Wait for RTC APB registers synchronisation */
  RTC_WaitForSynchro();
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_GetTime(uint32_t *date, uint32_t *time)
{
  RTC_GetDate(RTC_Format_BIN, &RTC_DateStructVal);      
  *date = (uint32_t)RTC_DateStructVal.RTC_Year;    
  *date <<= 8;
  *date |=(uint32_t)RTC_DateStructVal.RTC_Month;    
  *date <<= 8;    
  *date |=(uint32_t)RTC_DateStructVal.RTC_Date;
  RTC_GetTime(RTC_Format_BIN, &RTC_TimeStructVal);
  *time = (uint32_t)RTC_TimeStructVal.RTC_Hours;
  *time <<= 8;
  *time |= (uint32_t)RTC_TimeStructVal.RTC_Minutes;
  *time <<= 8;
  *time |= (uint32_t)RTC_TimeStructVal.RTC_Seconds;
  return S_TRUE;
}
/**
* @brief  This function will the reset the board.
* @param  Param:None. 
* @retval LINK_STATUS:
*                       
*/
SpiritBool WMBus_SetTime(uint32_t date, uint32_t time)
{
  RTC_DateStructVal.RTC_Date = (uint8_t)date;
  RTC_DateStructVal.RTC_Month = (uint8_t)((date &0x007F00)>>8);
  RTC_DateStructVal.RTC_Year = (uint8_t)((date &0x007F0000)>>16); 
  RTC_SetDate(RTC_Format_BIN, &RTC_DateStructVal); 
  
  RTC_TimeStructVal.RTC_Hours   = (uint8_t)((time &0x007F0000)>>16);
  RTC_TimeStructVal.RTC_Minutes = (uint8_t)((time &0x00007F00)>>8);
  RTC_TimeStructVal.RTC_Seconds = (uint8_t)time;
  RTC_SetTime(RTC_Format_BIN, &RTC_TimeStructVal);  
  return S_TRUE;
}
#endif
/**
* @brief  This function will return the WMBus Frame format.
* @param  Param:None. 
* @retval FrameFormatType_t: As shown below
*              
*                       
*/
FrameFormatType_t WMBus_AppliGetFrameFormat(void)
{
  int32_t value=0x00;
  if(LINK_STATUS_SUCCESS == WMBus_LinkGetAttribute(LINK_ATTR_ID_FRAME_FORMAT, &value))
  {
    return (FrameFormatType_t)value;
  }
  return (FrameFormatType_t)0x00;
}
/**
* @brief  This function will return the WMBus device type.
* @param  Param:None. 
* @retval WMBusDeviceType_T: As shown below
*              
*                       
*/
LINK_STATUS WMBus_AppliSetFrameFormat(FrameFormatType_t frameFormat)
{
  LINK_STATUS status = LINK_STATUS_UNKNOWN_ERROR;
  uint32_t value = (uint32_t)frameFormat;
  status = WMBus_LinkSetAttribute(LINK_ATTR_ID_FRAME_FORMAT, value);
  LinkAttrChanged = S_TRUE;
  return status;
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
* @brief  This function will write meter database to eeprom.
* @param  Param:None. 
* @retval None
*                       
*/
void Write_Meter_DB_EEPROM(void)
{
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[0][0], 26, EEPROM_METER1_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[1][0], 26, EEPROM_METER2_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[2][0], 26, EEPROM_METER3_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[3][0], 26, EEPROM_METER4_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[4][0], 26, EEPROM_METER5_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[5][0], 26, EEPROM_METER6_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[6][0], 26, EEPROM_METER7_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[7][0], 26, EEPROM_METER8_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[8][0], 26, EEPROM_METER9_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&ConcDataBaseBuff[9][0], 26, EEPROM_METER_10_DATABASE_START);
  WMBus_WriteMeterDatabaseToEEPROM(&MTR_EnKey_TO_EEPROM[0], 16, EEPROM_MTR_EnKey_SADDRESS);
  
}
/**
* @}
*/
/**
* @}
*/
/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/