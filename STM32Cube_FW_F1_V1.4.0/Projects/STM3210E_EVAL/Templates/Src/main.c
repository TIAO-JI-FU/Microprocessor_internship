	/**
  ******************************************************************************
  * File Name          : main.c
  * Description        : Main program body
  ******************************************************************************
  *
  * COPYRIGHT(c) 2016 STMicroelectronics
  *
  * Redistribution and use in source and binary forms, with or without modification,
  * are permitted provided that the following conditions are met:
  *   1. Redistributions of source code must retain the above copyright notice,
  *      this list of conditions and the following disclaimer.
  *   2. Redistributions in binary form must reproduce the above copyright notice,
  *      this list of conditions and the following disclaimer in the documentation
  *      and/or other materials provided with the distribution.
  *   3. Neither the name of STMicroelectronics nor the names of its contributors
  *      may be used to endorse or promote products derived from this software
  *      without specific prior written permission.
  *
  * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
  * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
  * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
  * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
  * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
  * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
  * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
  * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
  *
  ******************************************************************************
  */
/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"
#include <stdlib.h>
#include "lcd.h"
#include <main.h>
#include <math.h>
#ifdef __GNUC__
/* With GCC/RAISONANCE, small printf (option LD Linker->Libraries->Small printf
   set to 'Yes') calls __io_putchar() */
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif /* __GNUC__ */

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private variables ---------------------------------------------------------*/
TIM_HandleTypeDef htim1;
UART_HandleTypeDef huart1;
/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
static GPIO_InitTypeDef  GPIO_InitStruct;
void btn(void);
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM1_Init(void);
static void MX_USART1_UART_Init(void);
void itoa (int n,char s[]);								//輸出Player1的次數到LCD上面 
void itoa2 (int n,char s[]);							//輸出Player2的次數到LCD上面
unsigned char BeginWord1[8]={DW,De,Dl,Dl,Dc,Do,Dm,De};
unsigned char BeginWord2[2]={DT,Do};
unsigned char BeginWord3[4]={DO,Du,Dr,Ds};
unsigned char BeginWord4[4]={DG,Da,Dm,De};
unsigned char BeginWord5[6]={DP,Dl,De,Da,Ds,De};
unsigned char BeginWord6[4]={DL,Do,Do,Dk};
unsigned char BeginWord7[2]={DA,Dt};
unsigned char BeginWord8[3]={DT,Dh,De};
unsigned char BeginWord9[8]={DR,De,Da,Dl,Dt,De,Dr,Dm};
unsigned char game1[6]={DW,Di,Dn,Dn,De,Dr};
unsigned char game2[2]={DI,Ds};
unsigned char game3[7]={DP,Dl,Da,Dy,De,Dr,D1};
unsigned char game4[7]={DP,Dl,Da,Dy,De,Dr,D2};
unsigned char game5[4]={DD,Dr,Da,Dw};
unsigned char clr[]={123};
unsigned char again[5]={DP,Dr,De,Ds,Ds};
unsigned char again1[6]={DW,Da,Dk,De,Du,Dp};
char ch1[10];
char ch2[10];
int i=100;
int a=0;
int b=0;
int z=1;
int run=1;
int Ply1=0;
int Ply2=0;
unsigned char word;
int count=0;

int main(void)
{
  	HAL_Init();
  	SystemClock_Config();
	MX_GPIO_Init();
	MX_TIM1_Init();
	HAL_TIM_Base_Start_IT(&htim1);
	btn();
	LCD_Init();
	HAL_Delay(10);
 	MX_USART1_UART_Init();
	int run=1;
	int stop=0;
	int final=1;
	LCD_Clear();
	
	LCD_DrawString(0,32,BeginWord1,8);
	LCD_DrawString(2,56,BeginWord2,2);
	LCD_DrawString(4,48,BeginWord3,4);
	LCD_DrawString(6,48,BeginWord4,4);
	HAL_Delay(3000);
	while(1)
	{
		LCD_Clear();
		LCD_DrawString(0,40,BeginWord5,6);
		LCD_DrawString(2,36,BeginWord6,4);
		LCD_DrawString(2,76,BeginWord7,2);
		LCD_DrawString(4,52,BeginWord8,3);
		LCD_DrawString(6,32,BeginWord9,8);
		int lock=1;
		printf("You need to enter the time for the game to countdown.\r\n");
		printf("Input time limit(second):");
		HAL_UART_Receive(&huart1, (uint8_t *)&word, 1, 0xffff);
		printf("%d\r\n",word);
		printf("You enter %ds\r\n",word);
		printf("First player is Tamper button\r\n");
		printf("Second player is Key button\r\n");
		printf("Press Wakeup button to start game\r\n");
		while(lock){
			if(HAL_GPIO_ReadPin(WAKEUP_BUTTON_GPIO_PORT,WAKEUP_BUTTON_PIN)==1)lock = 0;//當WakeUp被按下的時候跳出迴圈 
		}
		LCD_Clear();
		LCD_Draw_ST_Logo(3);//顯示3 
		HAL_Delay(1000);
		LCD_Clear();
		LCD_Draw_ST_Logo(2);//顯示2 
		HAL_Delay(1000);
		LCD_Clear();
		LCD_Draw_ST_Logo(1);//顯示1		
		HAL_Delay(1000);
		LCD_Clear();
		LCD_Draw_ST_Logo(4);//顯示ST 
		HAL_Delay(500);
		count=0;
		run=1;
		Ply1=0;
		Ply2=0;
		LCD_Clear();
		while (run)
		{
			LCD_DrawString(0,0,game3,7);//顯示Player1 
			LCD_DrawString(0,64,game4,7);//顯示Player2
			if(count==word)//當數到輸入秒數時，跳出while(run) 
			{
				run=0;
				i=100;
				break;
			}
			if(a==1)
			{
				if(HAL_GPIO_ReadPin(TAMPER_BUTTON_GPIO_PORT,TAMPER_BUTTON_PIN)==1)//當按鈕放開時 
				{
					a=0;
				}			
			}
			if(b==1)
			{
				if(HAL_GPIO_ReadPin(KEY_BUTTON_GPIO_PORT,KEY_BUTTON_PIN)==1)//當按鈕放開時
				{
					b=0;
				}
			}
			while((a+b)==0) 
			{
				if(count==word)
				{
					run=0;
					i=100;
					break;
				}
				if(HAL_GPIO_ReadPin(TAMPER_BUTTON_GPIO_PORT,TAMPER_BUTTON_PIN)==0)//當按鈕按下時 
				{
					a=1;
					Ply1++;
					break;
				}
				if(HAL_GPIO_ReadPin(KEY_BUTTON_GPIO_PORT,KEY_BUTTON_PIN)==0)//當按鈕按下時
				{
					b=1;
					Ply2++;
					break;
				}
			}
			itoa (Ply1,ch1);//輸出Player1的次數到LCD上面 
			itoa2(Ply2,ch2);//輸出Player2的次數到LCD上面
		}
		printf("%d %d \r\n",Ply1,Ply2);
		if(Ply1>Ply2)
		{
			printf("Winner is Player1\r\n");
		}
		else if(Ply1<Ply2)
		{
			printf("Winner is Player2\r\n");
		}
		else
		{
			printf("Draw\r\n");
		}
		z=1;
		LCD_DrawString(0,0,game3,7);
		LCD_DrawString(0,64,game4,7);
		itoa (Ply1,ch1);
		itoa2(Ply2,ch2);
		LCD_DrawString(4,16,again,5);
		LCD_DrawString(4,64,again1,6);
		while(z)//跑馬燈 
		{
			if(HAL_GPIO_ReadPin(WAKEUP_BUTTON_GPIO_PORT,WAKEUP_BUTTON_PIN)==1)z = 0;
			i--;//目的左移 
			LCD_DrawString(6,0,clr,sizeof(clr));
			LCD_DrawString(6,8,clr,sizeof(clr));
			LCD_DrawString(6,16,clr,sizeof(clr));
			LCD_DrawString(6,24,clr,sizeof(clr));
			LCD_DrawString(6,32,clr,sizeof(clr));
			LCD_DrawString(6,40,clr,sizeof(clr));
			LCD_DrawString(6,48,clr,sizeof(clr));
			LCD_DrawString(6,56,clr,sizeof(clr));
			LCD_DrawString(6,64,clr,sizeof(clr));
			LCD_DrawString(6,72,clr,sizeof(clr));
			LCD_DrawString(6,80,clr,sizeof(clr));
			LCD_DrawString(6,88,clr,sizeof(clr));
			LCD_DrawString(6,96,clr,sizeof(clr));
			LCD_DrawString(6,104,clr,sizeof(clr));
			LCD_DrawString(6,112,clr,sizeof(clr));
			LCD_DrawString(6,120,clr,sizeof(clr));
			LCD_DrawString(6,128,clr,sizeof(clr));
			if(Ply1>Ply2)
			{
				LCD_DrawString(6,i,game1,6);
				LCD_DrawString(6,i+56,game2,2);
				LCD_DrawString(6,i+80,game3,7);
				HAL_Delay(70);
			}
			else if(Ply1<Ply2)
			{
				LCD_DrawString(6,i,game1,6);
				LCD_DrawString(6,i+56,game2,2);
				LCD_DrawString(6,i+80,game4,7);
				HAL_Delay(70);
			}
			else
			{
				LCD_DrawString(6,i,game5,4);
				HAL_Delay(70);
			}
			if(i==(-80))
			{
				i=100;
			}
		}
	}
}
void itoa (int n,char s[])
{
int i,j,sign;
if((sign=n)<0)
n=-n;
i=0;
do{
      s[i++]=n%10+'0';
}
while ((n/=10)>0);
if(sign<0)
s[i++]='-';
s[i]='\0';
for(j=i;j>=0;j--)
      LCD_DrawChar(2,24-8*j,s[j]-48);
} 
void itoa2 (int n,char s[])
{
int i,j,sign;
if((sign=n)<0)
n=-n;
i=0;
do{
      s[i++]=n%10+'0';
}
while ((n/=10)>0);
if(sign<0)
s[i++]='-';
s[i]='\0';
for(j=i;j>=0;j--)
      LCD_DrawChar(2,88-8*j,s[j]-48);
} 
void TIM1_UP_IRQHandler(void)//計時 
{
  HAL_TIM_IRQHandler(&htim1);
	if(run==1)
	{
		count++;
	}
}
/** System Clock Configuration
*/

void btn(void)
{
	__HAL_RCC_GPIOC_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_13;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOC , &GPIO_InitStruct);
	
	__HAL_RCC_GPIOB_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_10;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOB , &GPIO_InitStruct);
	
	__HAL_RCC_GPIOA_CLK_ENABLE();
	GPIO_InitStruct.Pin = GPIO_PIN_0;
	GPIO_InitStruct.Mode  = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull  = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
	HAL_GPIO_Init(GPIOA , &GPIO_InitStruct);
}
void MX_USART1_UART_Init(void)
{
  huart1.Instance = USART1;
  huart1.Init.BaudRate = 9600;
  huart1.Init.WordLength = UART_WORDLENGTH_8B;
  huart1.Init.StopBits = UART_STOPBITS_1;
  huart1.Init.Parity = UART_PARITY_NONE;
  huart1.Init.Mode = UART_MODE_TX_RX;
  huart1.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart1.Init.OverSampling = UART_OVERSAMPLING_16;
  HAL_UART_Init(&huart1);
}
PUTCHAR_PROTOTYPE
{
  /* Place your implementation of fputc here */
  /* e.g. write a character to the USART1 and Loop until the end of transmission */
	
  HAL_UART_Transmit(&huart1, (uint8_t *)&ch, 1, 0xffff);

  return ch;
}
void SystemClock_Config(void)
{

  RCC_OscInitTypeDef RCC_OscInitStruct;
  RCC_ClkInitTypeDef RCC_ClkInitStruct;

  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = 16;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  HAL_RCC_OscConfig(&RCC_OscInitStruct);

  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_SYSCLK;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;
  HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0);

  HAL_SYSTICK_Config(HAL_RCC_GetHCLKFreq()/1000);

  HAL_SYSTICK_CLKSourceConfig(SYSTICK_CLKSOURCE_HCLK);
}

/* TIM1 init function */
void MX_TIM1_Init(void)
{

  TIM_ClockConfigTypeDef sClockSourceConfig;
  TIM_SlaveConfigTypeDef sSlaveConfig;
  TIM_MasterConfigTypeDef sMasterConfig;

  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 7200;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period =1120;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  HAL_TIM_Base_Init(&htim1);

  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig);

  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_TRIGGER;
  sSlaveConfig.InputTrigger = TIM_TS_ITR0;
  HAL_TIM_SlaveConfigSynchronization(&htim1, &sSlaveConfig);

  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig);

}

/** Configure pins as 
        * Analog 
        * Input 
        * Output
        * EVENT_OUT
        * EXTI
*/
void MX_GPIO_Init(void)
{

  GPIO_InitTypeDef GPIO_InitStruct;

  /* GPIO Ports Clock Enable */
  __GPIOF_CLK_ENABLE();
	__GPIOA_CLK_ENABLE();

  /*Configure GPIO pins : PF6 PF7 PF8 PF9 */
  GPIO_InitStruct.Pin = GPIO_PIN_6|GPIO_PIN_7|GPIO_PIN_8|GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Speed = GPIO_SPEED_HIGH;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

#ifdef USE_FULL_ASSERT

/**
   * @brief Reports the name of the source file and the source line number
   * where the assert_param error has occurred.
   * @param file: pointer to the source file name
   * @param line: assert_param error line source number
   * @retval None
   */
void assert_failed(uint8_t* file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
    ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */

}

#endif

/**
  * @}
  */ 

/**
  * @}
*/ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
