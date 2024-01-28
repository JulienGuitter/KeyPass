/*
 * DFRobot_ID809.c
 *
 *  Created on: Jun 11, 2022
 *      Author: julie
 */

#include "DFRobot_ID809.h"
#include "HID_CDC.h"

#define RECIVE_BUFFER_SIZE	26
#define BUFFER_SIZE			26
#define COLLECT_NUMBER 		3

uint8_t buf[20];     //For saving response packet data
pCmdPacketHeader_t  sendHeader;
pRcmPacketHeader_t  recHeader;

uint8_t _number = 0;       //Fingerprint acquisistion times
uint8_t _state = 0;        //Collect fingerprint state
eError_t _error;           //Error code
uint16_t _PacketSize = 0;  //Data packet length to be sent


extern UART_HandleTypeDef huart1;


uint8_t posInBuffer = 0;
uint8_t Rx_data[BUFFER_SIZE];
uint8_t tempBuff[RECIVE_BUFFER_SIZE];
uint8_t posInBuffer;

uint8_t ID, samplingTimes, ret;

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart){
	HAL_UART_Receive_IT(&huart1, Rx_data, RECIVE_BUFFER_SIZE);
	posInBuffer = BUFFER_SIZE;
}

bool UART_Is_Available(){
	if(posInBuffer != 0){
		return true;
	}
	return false;
}

uint8_t UART_Read(){
	uint8_t tempReturnValue = Rx_data[0];

	for(int i=0; i<BUFFER_SIZE-1; i++){
		Rx_data[i] = Rx_data[i+1];
	}
	Rx_data[posInBuffer-1] = '\0';
	posInBuffer--;
	if(tempReturnValue == '\0'){
		return 0;
	}
	return tempReturnValue;
}

void DFRobot_ID809_Init(){
	HAL_UART_Receive_IT(&huart1, Rx_data, RECIVE_BUFFER_SIZE);
}


bool DFRobot_ID809_isConnected(){
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_TEST_CONNECTION, NULL, 0);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    if(ret == ERR_SUCCESS){
        return true;
    }else{
        return false;
    }
}

uint8_t DFRobot_ID809_getDeviceID(){
    uint8_t data[1];  //data:1byte Parameter Type
    data[0] = 0;
    uint8_t ret = DFRobot_ID809_getParam(data);
    return ret;
}

uint8_t DFRobot_ID809_getSecurityLevel(){
    uint8_t data[1];
    data[0] = 1;
    uint8_t ret = DFRobot_ID809_getParam(data);
    return ret;
}

uint8_t DFRobot_ID809_getDuplicationCheck(){
    uint8_t data[1];
    data[0] = 2;
    uint8_t ret = DFRobot_ID809_getParam(data);
    return ret;
}

uint8_t DFRobot_ID809_getBaudrate(){
    uint8_t data[1];
    data[0] = 3;
    uint8_t ret = DFRobot_ID809_getParam(data);
    return ret;
}

uint8_t DFRobot_ID809_getSelfLearn(){
    uint8_t data[1];
    data[0] = 4;
    uint8_t ret = DFRobot_ID809_getParam(data);
    return ret;
}

uint8_t DFRobot_ID809_getEmptyID(){
    char data[4] = {0};
    data[0] = 1;
    data[2] = FINGERPRINT_CAPACITY;     //80 fingerprints at most, default to full range
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_GET_EMPTY_ID, data, 4);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    if(ret == ERR_SUCCESS){
        ret = buf[0];
    }
    return ret;
}

uint8_t DFRobot_ID809_getParam(uint8_t* data){
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_GET_PARAM, (const char *)data, 1);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    if(ret == ERR_SUCCESS){
        ret = buf[0];
    }
    return ret;
}







uint8_t DFRobot_ID809_detectFinger(){
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_FINGER_DETECT, NULL, 0);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    if(ret == ERR_SUCCESS){
        ret = buf[0];
    }
    return ret;
}


uint8_t DFRobot_ID809_ctrlLED(eLEDMode_t mode,eLEDColor_t color,uint8_t blinkCount){
    char data[4] = {0};
  if(FINGERPRINT_CAPACITY == 80){
    data[0] = mode;
    data[2] = data[1] = color;
    data[3] = blinkCount;
  }else{
	if(mode == 1){
	  data[0] = 2;
	} else if(mode == 2){
		data[0] = 4;
	} else if(mode == 3){
	    data[0] = 1;
	} else if(mode == 4){
		data[0] = 0;
	} else if(mode == 5){
		data[0] = 3;
	}
	if(color == eLEDGreen){
          data[2] = data[1] =  0x84;
        }else if(color == eLEDRed){
	  data[2] = data[1] = 0x82;
	}else if(color == eLEDYellow){
	  data[2] = data[1] = 0x86;
	}else if(color == eLEDBlue){
	  data[2] = data[1] = 0x81;
	}else if(color == eLEDCyan){
	  data[2] = data[1] = 0x85;
	}else if(color == eLEDMagenta){
	  data[2] = data[1] = 0x83;
	}else {
	  data[2] = data[1] = 0x87;
	}
  }
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_SLED_CTRL, data, 4);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    return ret;
}



uint8_t DFRobot_ID809_getImage(){
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_GET_IMAGE, NULL, 0);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    return ret;
}



uint8_t DFRobot_ID809_collectionFingerprint(uint16_t timeout,int ramNumber){  //Collect fingerprint
    uint16_t i = 0;
    uint8_t ret;
    if(ramNumber == -1){
		if(_number > 2){
			_error = eErrorGatherOut;
			return ERR_ID809;
		}
    }
    while(!DFRobot_ID809_detectFinger()){
        if(timeout != 0){
            HAL_Delay(10);
            if(++i > timeout*10){
                _error = eErrorTimeOut;
                _state = 0;
                return ERR_ID809;
            }
       }
    }
    ret = DFRobot_ID809_getImage();
    if(ret != ERR_SUCCESS){
        _state = 0;
        return ERR_ID809;
    }
   if(ramNumber != -1){
     ret = DFRobot_ID809_generate(ramNumber);
   } else{
     ret = DFRobot_ID809_generate(_number);
   }
    if(ret != ERR_SUCCESS){
        _state = 0;
        return ERR_ID809;
    }
    _number++;
    _state = 1;
    return ret;
}

uint8_t DFRobot_ID809_detectFingers(){
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_FINGER_DETECT, NULL, 0);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    if(ret == ERR_SUCCESS){
        ret = buf[0];
    }
    return ret;
}

uint8_t DFRobot_ID809_generate(uint8_t RamBufferID){
    char data[2] = {0};
    data[0] = RamBufferID;
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_GENERATE, (const char *)data, 2);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    return ret;
}

uint8_t DFRobot_ID809_search(){
    if(_state == 1){
        char data[6] = {0};
        data[2] = 1;
        data[4] = FINGERPRINT_CAPACITY;
        _number = 0;
        pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_SEARCH, data, 6);
        DFRobot_ID809_sendPacket(header);
        free(header);
        uint8_t ret = DFRobot_ID809_responsePayload(buf);
        if(ret == ERR_SUCCESS){
            ret = buf[0];
        }else{
            ret = 0;
        }
        return ret;
    }
    return 0;
}

uint8_t DFRobot_ID809_delFingerprint(uint8_t ID)
{
  char data[4] = {0};
  if(ID == DELALL) {
    data[0] = 1;
    data[2] = FINGERPRINT_CAPACITY;
  } else {
    data[0] = data[2] = ID;
  }
  pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_DEL_CHAR, data, 4);
  DFRobot_ID809_sendPacket(header);
  free(header);
  uint8_t ret = DFRobot_ID809_responsePayload(buf);
  return ret;
}


uint8_t DFRobot_ID809_storeFingerprint(uint8_t ID){
    char data[4] = {0};
    uint8_t ret;
    ret = DFRobot_ID809_merge();
    if(ret != ERR_SUCCESS){
        return ERR_ID809;
    }
    _number = 0;
    data[0] = ID;
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_STORE_CHAR, data, 4);
    DFRobot_ID809_sendPacket(header);
    free(header);
    ret = DFRobot_ID809_responsePayload(buf);
    return ret;

}

uint8_t DFRobot_ID809_merge(){
    char data[3] = {0};
    data[2] = _number;
    pCmdPacketHeader_t header = DFRobot_ID809_pack(CMD_TYPE, CMD_MERGE, data, 3);
    DFRobot_ID809_sendPacket(header);
    free(header);
    uint8_t ret = DFRobot_ID809_responsePayload(buf);
    return ret;
}






pCmdPacketHeader_t DFRobot_ID809_pack(uint8_t type, uint16_t cmd, const char *payload, uint16_t len){
    pCmdPacketHeader_t header;
    uint16_t cks=0;
    uint16_t dataLen;
    if(type == CMD_TYPE){    //Structure of command packet, fixed 26 bytes：10(frame header)+14(data)+2(CKS)
        if((header = (pCmdPacketHeader_t)malloc(sizeof(sCmdPacketHeader_t)+16+2)) == NULL){
            return NULL;
        }
        header->PREFIX = CMD_PREFIX_CODE;
        for(int i=0;i<16;i++){
            header->payload[i] = 0;
        }
        dataLen = 16;   //Length of data to be replicated
    }else{                   //Structure of command data packet, unfixed length：10(frame header)+LEN(data)+2(CKS)
        if((header = (pCmdPacketHeader_t)malloc(sizeof(sCmdPacketHeader_t)+len+2)) == NULL){
            return NULL;
        }
        header->PREFIX = CMD_DATA_PREFIX_CODE;
        dataLen = len;   //Length of data to be replicated
    }
    header->SID = 0;
    header->DID = 0;
    header->CMD = cmd;
    header->LEN = len;
    if(len){
        memcpy(header->payload, payload, len);
    }
    cks = DFRobot_ID809_getCmdCKS(header);
    memcpy(&header->payload[dataLen],&cks,2);
    _PacketSize = sizeof(sCmdPacketHeader_t) + dataLen +2;
    return header;
}

void DFRobot_ID809_sendPacket(pCmdPacketHeader_t header){
	HAL_UART_Transmit(&huart1, header, _PacketSize, 0xFFFF);
}

uint8_t DFRobot_ID809_responsePayload(void* buf){
    sRcmPacketHeader_t header;
    uint16_t dataLen,dataCount,cks;
    uint8_t ch,ret;
    int16_t type;
    type = DFRobot_ID809_readPrefix(&header);
    if(type == 1){
        _error = eErrorRecvTimeout;
        return ERR_ID809;
    }
    pRcmPacketHeader_t packet;
    if(type == RCM_TYPE){    //Structure of response packet, fixed 26 bytes: 10(frame header)+14(data)+2(CKS)
        packet = (pRcmPacketHeader_t)malloc(sizeof(sRcmPacketHeader_t)+14+2);
        dataLen = 14+2;      //Length of data+CKS
        if(packet == NULL){
            printf("");
            while(1);
        }
    }else{                   //Structure of response data packet, unfixed length: 10(frame header)+(LEN-2)(data)+2(CKS)
        packet = (pRcmPacketHeader_t)malloc(sizeof(sRcmPacketHeader_t)+header.LEN);
        dataLen = header.LEN;  //Length of data+CKS
        if(packet == NULL){
            printf("");
            while(1);
        }
    }
    memcpy(packet, &header, 10);
    dataCount = DFRobot_ID809_readN(packet->payload, dataLen);
    cks = packet->payload[dataLen-2]+(packet->payload[dataLen-1]<<8);
    ret = (header.RET&0xFF);
    _error = (eError_t)ret;
    if(ret != ERR_SUCCESS){
        ret = ERR_ID809;
    }else if(dataLen != dataCount){
        _error = eErrorRecvLength;
        ret = ERR_ID809;
    }else if(DFRobot_ID809_getRcmCKS(packet) != cks){
        _error = eErrorRecvCks;
        ret = ERR_ID809;
    }else{
        memcpy(buf, packet->payload, dataLen);
    }
    free(packet);
    packet = NULL;
    return ret;
}

uint16_t DFRobot_ID809_readPrefix( pRcmPacketHeader_t header ){
    uint8_t ch,ret;
    typedef enum{
        RECV_HEADER_INIT,
        RECV_HEADER_AA,
        RECV_HEADER_A5,
        RECV_HEADER_OK
    }eRecvHeaderState;
    eRecvHeaderState state = RECV_HEADER_INIT;
    while(state != RECV_HEADER_OK){   //Can judge the received command packet and command data packet prefix at the same time
        if(DFRobot_ID809_readN(&ch, 1) != 1){
            ret = 1;
            return ret;
        }
        if((ch == 0xAA) && (state == RECV_HEADER_INIT)){
            state = RECV_HEADER_AA;
            continue;
        }else if((ch == 0xA5) && (state == RECV_HEADER_INIT)){
            state = RECV_HEADER_A5;
            continue;
        }else if((ch == 0x55) && (state == RECV_HEADER_AA)){
            state = RECV_HEADER_OK;
            ret = RCM_TYPE;
            continue;
        }else if((ch == 0x5A) && (state == RECV_HEADER_A5)){
            state = RECV_HEADER_OK;
            ret = DATA_TYPE;
            continue;
        }else{
            state = RECV_HEADER_INIT;
            if(ch == 0xAA){
                state = RECV_HEADER_AA;
            }else if(ch == 0xA5){
                state = RECV_HEADER_A5;
            }
        }
    }
    if(ret == RCM_TYPE){
        header->PREFIX = RCM_PREFIX_CODE;
    }else if(ret == DATA_TYPE){
        header->PREFIX = RCM_DATA_PREFIX_CODE;
    }
    DFRobot_ID809_readN(&header->SID, 1);
    DFRobot_ID809_readN(&header->DID, 1);
    DFRobot_ID809_readN(&header->RCM, 2);
    DFRobot_ID809_readN(&header->LEN, 2);
    DFRobot_ID809_readN(&header->RET, 2);
    return ret;
}

size_t DFRobot_ID809_readN(void* buffer, size_t len){
    size_t offset = 0,left = len;
    uint8_t *buf = (uint8_t*)buffer;
    long long curr = HAL_GetTick();
    while(left){
        if(UART_Is_Available()){
        	buf[offset++] = UART_Read();
            left--;
        }
        if(HAL_GetTick() - curr > 5000){
            break;
        }
    }
    return offset;
}

uint16_t DFRobot_ID809_getCmdCKS(pCmdPacketHeader_t packet){
    uint16_t cks = 0xFF;
    cks += packet->SID;
    cks += packet->DID;
    cks += packet->CMD&0xFF;
    cks += packet->CMD>>8;
    cks += packet->LEN&0xFF;
    cks += packet->LEN>>8;
    if(packet->LEN > 0){
        uint8_t *p = packet->payload;
        for(uint16_t i = 0; i < packet->LEN; i++){
            cks += p[i];
        }
    }
    return cks&0xFFFF;
}

uint16_t DFRobot_ID809_getRcmCKS(pRcmPacketHeader_t packet){
    uint16_t cks = 0xFF;
    cks += packet->SID;
    cks += packet->DID;
    cks += packet->RCM&0xFF;
    cks += packet->RCM>>8;
    cks += packet->LEN&0xFF;
    cks += packet->LEN>>8;
    cks += packet->RET&0xFF;
    cks += packet->RET>>8;
    if(packet->LEN > 0){
        uint8_t *p = packet->payload;
        for(uint16_t i = 0; i < packet->LEN-2; i++){
            cks += p[i];
        }
    }
    return cks&0xFFFF;
}



bool testFingerPrint() {
  uint8_t ret = 0;
  DFRobot_ID809_ctrlLED(/*LEDMode = */eBreathing, /*LEDColor = */eLEDBlue, /*blinkCount = */0);
  if ((DFRobot_ID809_collectionFingerprint(0, 0)) != ERR_ID809) {
	  DFRobot_ID809_ctrlLED(/*LEDMode = */eFastBlink, /*LEDColor = */eLEDYellow, /*blinkCount = */3);
    while (DFRobot_ID809_detectFingers());
    ret = DFRobot_ID809_search();
    if (ret != 0) {
    	DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDGreen, /*blinkCount = */0);
    	return true;
    } else {
    	DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDRed, /*blinkCount = */0);
    	return false;
    }
  } else {
  	DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDRed, /*blinkCount = */3);
  	return false;
  }
}

void addFingerPrint() {
  if ((ID = DFRobot_ID809_getEmptyID()) == ERR_ID809) {
    while (1) {
      HAL_Delay(1000);
    }
  }
  samplingTimes = 0;   //Clear sampling times
  while (samplingTimes < COLLECT_NUMBER) {
	  DFRobot_ID809_ctrlLED(/*LEDMode = */eBreathing, /*LEDColor = */eLEDBlue, /*blinkCount = */0);
	  writeOnLCD("   Press down your","       finger");
    if ((DFRobot_ID809_collectionFingerprint(10, 0)) != ERR_ID809) {
    	DFRobot_ID809_ctrlLED(/*LEDMode = */eFastBlink, /*LEDColor = */eLEDYellow, /*blinkCount = */3);
    	writeOnLCD("  Sampling succeeds","");
    	samplingTimes++;   //Sampling times +1
    } else {
    	writeOnLCD("   Sampling failed","");
    	DFRobot_ID809_ctrlLED(/*LEDMode = */eFastBlink, /*LEDColor = */eLEDWhite, /*blinkCount = */2);
    }
    writeOnLCD("    Release your","       finger");
    while (DFRobot_ID809_detectFingers());
  }

  if (DFRobot_ID809_storeFingerprint(/*Empty ID = */ID) != ERR_ID809) {
	  writeOnLCD("   Saving succeed","");
	  DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDGreen, /*blinkCount = */0);
	  HAL_Delay(1000);
    DFRobot_ID809_ctrlLED(/*LEDMode = */eNormalClose, /*LEDColor = */eLEDBlue, /*blinkCount = */0);
    HAL_Delay(1000);
  } else {
	  writeOnLCD("    Saving failed","");
	    DFRobot_ID809_ctrlLED(/*LEDMode = */eFastBlink, /*LEDColor = */eLEDWhite, /*blinkCount = */4);
	    HAL_Delay(1000);
  }
}

void delFingerPrint() {
  uint8_t ret = 0;
  DFRobot_ID809_ctrlLED(/*LEDMode = */eBreathing, /*LEDColor = */eLEDBlue, /*blinkCount = */0);
  writeOnLCD("   Press down your","       finger");
  if ((DFRobot_ID809_collectionFingerprint(/*timeout=*/10, 0)) != ERR_ID809) {
    ret = DFRobot_ID809_search();
    if (ret != 0) {
    	DFRobot_ID809_delFingerprint(/*Fingerprint ID = */ret);
  	  writeOnLCD("   Delete succeeds","");
      DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDGreen, /*blinkCount = */0);
    } else {
  	  writeOnLCD("   Fingerprint is","    unregistered");
    	DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDRed, /*blinkCount = */0);
    }
  } else {
	  writeOnLCD("   Capturing fails","");
	  DFRobot_ID809_ctrlLED(/*LEDMode = */eKeepsOn, /*LEDColor = */eLEDRed, /*blinkCount = */0);
  }
  writeOnLCD("    Release your","       finger");
  while (DFRobot_ID809_detectFinger());
}


void removeAllFinger(){
	for(int i=0; i<80; i++){
		DFRobot_ID809_delFingerprint(/*Fingerprint ID = */i);
	}
}
