/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "i2c.h"
#include "spi.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "w25qxx.h"
#include "DFRobot_ID809.h"
#include "lcd_i2c.h"
#include "password.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern UART_HandleTypeDef huart1;

uint8_t buffer1[20];
uint8_t buffer2[100] = {0};
uint8_t codeState = e_INIT;
bool toSingleLoad = true;
char passwordStr[100] = {0};
uint8_t indexFlash[1] = {0};
uint8_t idButton = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  HAL_Delay(100);
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_USB_PCD_Init();
  MX_I2C1_Init();
  MX_USART1_UART_Init();
  MX_SPI2_Init();
  MX_ADC1_Init();
  /* USER CODE BEGIN 2 */

  DFRobot_ID809_Init();
  MX_USB_DEVICE_Init();
  // Calibrate The ADC On Power-Up For Better Accuracy
  HAL_ADCEx_Calibration_Start(&hadc1);

  HAL_Delay(100);
  lcd_init ();
  lcd_clear ();
  HAL_Delay(100);

  writeOnLCD("      KeyPass", "Loading ...");

  uint8_t *data = "Hello World from USB CDC\n";
  CDC_Transmit(0, data, strlen(data));


  /* --------- differente commande for flash memeorie */
  W25qxx_Init();
  //block 1 for nb of password and fingerprint nb
  HAL_Delay(500);
  DFRobot_ID809_ctrlLED(eKeepsOn, eLEDRed, 0);
  //printf("Start\n");

  while (DFRobot_ID809_isConnected() == false) {
  	DFRobot_ID809_ctrlLED(eBreathing, eLEDWhite, 0);
	HAL_Delay(1000);
  }

  HAL_Delay(50);
  DFRobot_ID809_ctrlLED(eKeepsOn, eLEDGreen, 0);

  HAL_Delay(500);

  codeState = e_MENU;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1){
	  /* USER CODE END WHILE */
	  static uint8_t elementInMenu = 0;
	  switch(codeState){
		  case e_MENU:{
			  static char* menuButtons[5] = {"   Load password", "    Add password", "     Add finger", "     Del finger", "   reset password"};
			  if(toSingleLoad){
				  DFRobot_ID809_ctrlLED(eNormalClose, eLEDRed, 0);
				  elementInMenu = 5;
				  toSingleLoad=false;
				  writeOnLCD(menuButtons[idButton],"          ->    ok");
			  }
			  if(readButtonFinger()){
				  toSingleLoad = true;
				  switch(idButton){
				  	  case 0:
				  		codeState = e_PASSWORD;
				  		  break;
				  	  case 1:
				  		codeState = e_ADDPASSWORD;
				  		  break;
				  	  case 2:
				  		codeState = e_ADDFINGER;
				  		  break;
				  	  case 3:
				  		codeState = e_DELFINGER;
				  		  break;
				  	  case 4:
				  		  codeState = e_PASSRESET;
				  		  break;
				  }
				  idButton = 0;
			  }
			  if(readButton2()){
				  toSingleLoad = true;
				  if(idButton == elementInMenu-1){
					  idButton = 0;
				  }else{
					  idButton++;
				  }
			  }
			  break;
		  }
		  case e_PASSWORD:{
			  if(toSingleLoad){
				  W25qxx_ReadBlock(indexFlash, 1, 0, 1);
				  toSingleLoad=false;
				  if(indexFlash[0] > 0){
					  DFRobot_ID809_ctrlLED(eNormalClose, eLEDRed, 0);
					  W25qxx_ReadBlock(buffer1, 2+(idButton*2)+1, 0, 100);
					  writeOnLCD(buffer1,"  menu    ->    ok");
				  }else{
					  writeOnLCD("     No password","    return");
				  }
				  codeState = e_PASSWORD;
			  }
			  if(readButton2()){
				  codeState = e_PASSWORD;
				  if(indexFlash[0] > 0){
					  toSingleLoad = true;
					  if(idButton == indexFlash[0]-1){
						  idButton = 0;
					  }else{
						  idButton++;
					  }
				  }
			  }
			  if(readButtonFinger()){
				  codeState = e_PASSWORD;
				  toSingleLoad = true;
				  if(indexFlash[0] > 0){
					  writeOnLCD("    Check finger","       put it");
					  while(DFRobot_ID809_detectFinger());
					  if(testFingerPrint()){
						  writeOnLCD("    Start writing","      password");
						  W25qxx_ReadBlock(buffer1, 2+(idButton*2)+2, 0, 100);
						  W25qxx_R
						  writePasswordHID(buffer1);
						  writeOnLCD("    Writing done","");
						  codeState = e_MENU;
						  idButton=0;
						  HAL_Delay(1000);
					  }else{
						  writeOnLCD("    Wrong finger","");
					  }
					  HAL_Delay(1000);
				  }
			  }
			  if(readButton1()){
				  codeState = e_MENU;
				  toSingleLoad = true;
				  idButton=0;
			  }
			  break;
		  }
		  case e_ADDPASSWORD:{
			  writeOnLCD("    Check finger","       put it");
			  while(DFRobot_ID809_detectFinger());
			  if(testFingerPrint()){
				  bool _Lettre_, _Number_, _SpeChar_, _Capitals_;

				  HAL_Delay(500);
				  DFRobot_ID809_ctrlLED(eNormalClose, eLEDRed, 0);

				  writeOnLCD("    Has lettre ?","    no       yes");
				  while(!readButton1() && !(_Lettre_ = readButton2()));
				  writeOnLCD("    Has number ?","    no       yes");
				  while(!readButton1() && !(_Number_ = readButton2()));
				  writeOnLCD(" Has special char ?","    no       yes");
				  while(!readButton1() && !(_SpeChar_ = readButton2()));
				  writeOnLCD("   Has capitals ?","    no       yes");
				  while(!readButton1() && !(_Capitals_ = readButton2()));
				  writeOnLCD(" Starting generation","");

				  if(!_Number_ && !_SpeChar_) _Lettre_ = true;

				  generatePassword(passwordStr, 10, _Lettre_, _Number_, _SpeChar_, _Capitals_);

				  uint8_t namePass[21] = "Password  ";

				  W25qxx_ReadBlock(indexFlash, 1, 0, 1);

				  namePass[9] = indexFlash[0]+'0';

				  W25qxx_EraseBlock(2+((indexFlash[0])*2)+1);
				  W25qxx_EraseBlock(2+((indexFlash[0])*2)+2);
				  W25qxx_WriteBlock(namePass, 2+((indexFlash[0])*2)+1, 0, 21);
				  W25qxx_WriteBlock(passwordStr, 2+((indexFlash[0])*2)+2, 0, 100);

				  indexFlash[0]++;
				  W25qxx_EraseBlock(1);
				  W25qxx_WriteBlock(indexFlash, 1, 0, 1);

				  writeOnLCD("  Password has been","      generated");
				  HAL_Delay(1000);

				  codeState = e_MENU;

			  }else{
				  writeOnLCD("    Wrong finger","");
				  HAL_Delay(1000);
				  codeState = e_MENU;
			  }
			  break;
		  }
		  case e_ADDFINGER:{
			  writeOnLCD("  Your going to add","   finger print");
			  HAL_Delay(500);
			  addFingerPrint();
			  HAL_Delay(500);
			  codeState = e_MENU;
			  break;
		  }
		  case e_DELFINGER:{
			  writeOnLCD("Your going to delete", "   finger print");
			  delFingerPrint();
			  resetAllPassword();
			  HAL_Delay(500);
			  codeState = e_MENU;
			  break;
		  }
		  case e_PASSRESET:{
			  writeOnLCD("    Check finger","       put it");
			  while(DFRobot_ID809_detectFinger());
			  if(testFingerPrint()){
				  writeOnLCD("  Password has been","        reset");
				  resetAllPassword();
			  }else{
				  writeOnLCD("    Wrong finger","");
			  }
			  HAL_Delay(1000);
			  codeState = e_MENU;
			  break;
		  }
	  }
    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

bool readButton2(void)
{
	static bool lastPosBP1 = false;
	bool actualPosBP1 = HAL_GPIO_ReadPin(BP1_GPIO_Port, BP1_Pin);
	if(actualPosBP1 != lastPosBP1){
		lastPosBP1 = actualPosBP1;
		return actualPosBP1;
	}
	return false;
}

bool readButton1(void)
{
	static bool lastPosBP2 = false;
	bool actualPosBP2 = HAL_GPIO_ReadPin(BP2_GPIO_Port, BP2_Pin);
	if(actualPosBP2 != lastPosBP2){
		lastPosBP2 = actualPosBP2;
		return actualPosBP2;
	}
	return false;
}
bool readButtonFinger(void){
	static bool lastPosBP2 = false;
	bool actualPosBP2 = DFRobot_ID809_detectFinger();
	if(actualPosBP2 != lastPosBP2){
		lastPosBP2 = actualPosBP2;
		return actualPosBP2;
	}
	return false;
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC|RCC_PERIPHCLK_USB;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV2;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
