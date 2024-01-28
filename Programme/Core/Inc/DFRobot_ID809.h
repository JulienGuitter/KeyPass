/*
 * DFRobot_ID809.h
 *
 *  Created on: Jun 11, 2022
 *      Author: julie
 */

#ifndef INC_DFROBOT_ID809_H_
#define INC_DFROBOT_ID809_H_

#include <stdbool.h>
#include "main.h"


#endif /* INC_DFROBOT_ID809_H_ */


  /*
   Frame struct of command packet
  */

typedef struct{
  uint16_t  PREFIX;
  uint8_t   SID;
  uint8_t   DID;
  uint16_t  CMD;
  uint16_t  LEN;
  uint8_t payload[0];
}__attribute__ ((packed)) sCmdPacketHeader_t, *pCmdPacketHeader_t;


/*
 Frame struct of response packet
*/
typedef struct{
uint16_t  PREFIX;
uint8_t   SID;
uint8_t   DID;
uint16_t  RCM;
uint16_t  LEN;
uint16_t  RET;
uint8_t   payload[0];
}__attribute__ ((packed)) sRcmPacketHeader_t, *pRcmPacketHeader_t;



#define FINGERPRINT_CAPACITY     80      //Fingerprint module capacity
#define MODULE_SN_SIZE           16      //Module SN length


#define DELALL                   0xFF    //Delete all fingerprints

#define CMD_PREFIX_CODE          0xAA55  //Command packet prefix code
#define RCM_PREFIX_CODE          0x55AA  //Response packet prefix code
#define CMD_DATA_PREFIX_CODE     0xA55A  //Command data packet prefix code
#define RCM_DATA_PREFIX_CODE     0x5AA5  //Response data packet prefix code

#define CMD_TYPE                 0xF0    //Command packet type
#define RCM_TYPE                 0xF0    //Response packet type
#define DATA_TYPE                0x0F    //Data packet type

#define CMD_TEST_CONNECTION      0X0001  //Test connection
#define CMD_SET_PARAM            0X0002  //Set parameter
#define CMD_GET_PARAM            0X0003  //Read parameter
#define CMD_DEVICE_INFO          0X0004  //Read device information
#define CMD_SET_MODULE_SN        0X0008  //Set module serial number
#define CMD_GET_MODULE_SN        0X0009  //Read module serial number
#define CMD_ENTER_STANDBY_STATE  0X000C  //Enter sleep mode
#define CMD_GET_IMAGE            0X0020  //Capture fingerprint image
#define CMD_FINGER_DETECT        0X0021  //Detect fingerprint
    #define CMD_UP_IMAGE_CODE        0X0022  //Upload fingerprint image to host
    #define CMD_DOWN_IMAGE           0X0023  //Download fingerprint image to module
#define CMD_SLED_CTRL            0X0024  //Control collector backlight
#define CMD_STORE_CHAR           0X0040  //Save fingerprint template data into fingerprint library
#define CMD_LOAD_CHAR            0X0041  //Read fingerprint in module and save it in RAMBUFFER temporarily
    #define CMD_UP_CHAR              0X0042  //Upload the fingerprint template saved in RAMBUFFER to host
    #define CMD_DOWN_CHAR            0X0043  //Download fingerprint template to module designated RAMBUFFER
#define CMD_DEL_CHAR             0X0044  //Delete fingerprint in specific ID range
#define CMD_GET_EMPTY_ID         0X0045  //Get the first registerable ID in specific ID range
#define CMD_GET_STATUS           0X0046  //Check if the designated ID has been registered
#define CMD_GET_BROKEN_ID        0X0047  //Check whether there is damaged data in fingerprint library of specific range
#define CMD_GET_ENROLL_COUNT     0X0048  //Get the number of registered fingerprints in specific ID range
#define CMD_GET_ENROLLED_ID_LIST 0X0049  //Get registered ID list
#define CMD_GENERATE             0X0060  //Generate template from the fingerprint images saved in IMAGEBUFFER temporarily
#define CMD_MERGE                0X0061  //Synthesize fingerprint template data
#define CMD_MATCH                0X0062  //Compare templates in 2 designated RAMBUFFER
#define CMD_SEARCH               0X0063  //1:N Recognition in specific ID range
#define CMD_VERIFY               0X0064  //Compare specific RAMBUFFER template with specific ID template in fingerprint library

#define ERR_SUCCESS              0x00    //Command processed successfully
#define ERR_ID809                0xFF    //error


typedef enum{
  eBreathing = 1,  //Breathing
  eFastBlink,      //Quick blink
  eKeepsOn,        //On
  eNormalClose,    //Off
  eFadeIn,         //Fade in
  eFadeOut,        //Fade out
  eSlowBlink       //Slow blink
}eLEDMode_t;

typedef enum{
  eLEDGreen = 1,   //green
  eLEDRed,         //red
  eLEDYellow,      //yellow
  eLEDBlue,        //blue
  eLEDCyan,        //cyan
  eLEDMagenta,     //magenta
  eLEDWhite        //white
}eLEDColor_t;

typedef enum{
  e9600bps = 1,
  e19200bps,
  e38400bps,
  e57600bps,
  e115200bps
}eDeviceBaudrate_t;

typedef enum{
  eErrorSuccess            = 0x00,    //Command processed successfully
  eErrorFail               = 0x01,    //Command processing failed
  eErrorVerify             = 0x10,    //1:1 Templates comparison in specific ID failed
  eErrorIdentify           = 0x11,    //1:N comparison has been made, no same templates here
  eErrorTmplEmpty          = 0x12,    //No registered template in the designated ID
  eErrorTmplNotEmpty       = 0x13,    //Template already exists in the specified ID
  eErrorAllTmplEmpty       = 0x14,    //No registered Template
  eErrorEmptyIDNoexist     = 0x15,    //No registerable Template ID
  eErrorBrokenIDNoexist    = 0x16,    //No damaged Template
  eErrorInvalidTmplData    = 0x17,    //The designated Template Data is invalid
  eErrorDuplicationID      = 0x18,    //The fingerprint has been registered
  eErrorBadQuality         = 0x19,    //Poor quality fingerprint image
  eErrorMergeFail          = 0x1A,    //Template synthesis failed
  eErrorNotAuthorized      = 0x1B,    //Communication password not authorized
  eErrorMemory             = 0x1C,    //Error in exernal Flash burning
  eErrorInvalidTmplNo      = 0x1D,    //The designated template ID is invalid
  eErrorInvalidParam       = 0x22,    //Incorrect parameter has been used
  eErrorTimeOut            = 0x23,    //Acquisition timeout
  eErrorGenCount           = 0x25,    //Invalid number of fingerprint synthesis
  eErrorInvalidBufferID    = 0x26,    //Wrong Buffer ID value
  eErrorFPNotDetected      = 0x28,    //No fingerprint input into fingerprint reader
  eErrorFPCancel           = 0x41,    //Command cancelled
  eErrorRecvLength         = 0x42,    //Wrong length of recieved data
  eErrorRecvCks            = 0x43,    //Wrong check code
  eErrorGatherOut          = 0x45,    //Exceed upper limit of acquisition times
  eErrorRecvTimeout        = 0x46     //Communication timeout
}eError_t;

typedef struct{
  /**< Gesture enumeration variable X */
  eError_t error;
  /**< Description about the gesture enumeration variable X */
  const char * description;
}sErrorDescription_t;


bool UART_Is_Available();

uint8_t UART_Read();

void DFRobot_ID809_Init();

/**
 * @brief Test whether the module connection is ok
 * @return true or false
 */
bool DFRobot_ID809_isConnected();

uint8_t DFRobot_ID809_getDeviceID();

uint8_t DFRobot_ID809_getSecurityLevel();

uint8_t DFRobot_ID809_getDuplicationCheck();

uint8_t DFRobot_ID809_getBaudrate();

uint8_t DFRobot_ID809_getSelfLearn();

uint8_t DFRobot_ID809_getEmptyID();

uint8_t DFRobot_ID809_getParam(uint8_t* data);




uint8_t DFRobot_ID809_ctrlLED(eLEDMode_t mode,eLEDColor_t color,uint8_t blinkCount);

uint8_t DFRobot_ID809_getImage();

uint8_t DFRobot_ID809_collectionFingerprint(uint16_t timeout,int ramNumber);

uint8_t DFRobot_ID809_detectFingers();

uint8_t DFRobot_ID809_generate(uint8_t RamBufferID);

uint8_t DFRobot_ID809_search();

uint8_t DFRobot_ID809_storeFingerprint(uint8_t ID);

uint8_t DFRobot_ID809_merge();






/**
  * @brief Packing data frame
  * @param Data type：CMD_TYPE or DATA_TYPE
  * @param Command
  * @param Data
  * @param Length
  * @return Data frame
  */
 pCmdPacketHeader_t DFRobot_ID809_pack(uint8_t type, uint16_t cmd, const char *payload, uint16_t len);

/**
  * @brief Send data
  * @param Data frame
  */
 void DFRobot_ID809_sendPacket(pCmdPacketHeader_t header);

 /**
   * @brief Read byte
   * @param Pointer for saving data
   * @param Length of data to be received
   * @return Actual received data length
   */
  size_t DFRobot_ID809_readN(void* buf_, size_t len);

 /**
   * @brief Read frame header
   * @param Frame header struct of response packet
   * @return Response packet type：RCM_TYPE,DATA_TYPE or 1(reading timeout)
   */
  uint16_t DFRobot_ID809_readPrefix( pRcmPacketHeader_t header );

 /**
   * @brief Read data
   * @param Pointer for saving data
   * @return 0(success) or ERR_ID809
   */
  uint8_t DFRobot_ID809_responsePayload(void* buf);

 /**
   * @brief Get command packet CKS
   * @param Command packet frame
   * @return CKS
   */
  uint16_t DFRobot_ID809_getCmdCKS(pCmdPacketHeader_t packet);

 /**
   * @brief Get response packet CKS
   * @param Response packet frame
   * @return CKS
   */
  uint16_t DFRobot_ID809_getRcmCKS(pRcmPacketHeader_t packet);




  bool testFingerPrint();
  void addFingerPrint();
  void delFingerPrint();
  void removeAllFinger();
