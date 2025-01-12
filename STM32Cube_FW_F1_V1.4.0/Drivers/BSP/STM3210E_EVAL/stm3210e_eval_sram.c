/**
  ******************************************************************************
  * @file    stm3210e_eval_sram.c
  * @author  MCD Application Team
  * @version V6.0.2
  * @date    29-April-2016
  * @brief   This file includes the SRAM driver for the IS61WV51216BLL-10M memory 
  *          device mounted on STM3210E-EVAL evaluation board.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT(c) 2016 STMicroelectronics</center></h2>
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

/* File Info : -----------------------------------------------------------------
                                   User NOTES
1. How To use this driver:
--------------------------
   - This driver is used to drive the IS61WV51216BLL-10M SRAM external memory mounted
     on STM3210E-EVAL evaluation board.
   - This driver does not need a specific component driver for the SRAM device
     to be included with.

2. Driver description:
---------------------
  + Initialization steps:
     o Initialize the SRAM external memory using the BSP_SRAM_Init() function. This 
       function includes the MSP layer hardware resources initialization and the
       FSMC controller configuration to interface with the external SRAM memory.
  
  + SRAM read/write operations
     o SRAM external memory can be accessed with read/write operations once it is
       initialized.
       Read/write operation can be performed with AHB access using the functions
       BSP_SRAM_ReadData()/BSP_SRAM_WriteData(), or by DMA transfer using the functions
       BSP_SRAM_ReadData_DMA()/BSP_SRAM_WriteData_DMA().
     o The AHB access is performed with 16-bit width transaction, the DMA transfer
       configuration is fixed at single (no burst) halfword transfer 
       (see the SRAM_MspInit() static function).
     o User can implement his own functions for read/write access with his desired 
       configurations.
     o If interrupt mode is used for DMA transfer, the function BSP_SRAM_DMA_IRQHandler()
       is called in IRQ handler file, to serve the generated interrupt once the DMA 
       transfer is complete.
 
------------------------------------------------------------------------------*/

/* Includes ------------------------------------------------------------------*/
#include "stm3210e_eval_sram.h"

/** @addtogroup BSP
  * @{
  */

/** @addtogroup STM3210E_EVAL
  * @{
  */ 
  
/** @defgroup STM3210E_EVAL_SRAM STM3210E_EVAL SRAM
  * @{
  */ 
  
/* Private typedef -----------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_SRAM_Private_Types_Definitions Private_Types_Definitions
  * @{
  */ 
  
/**
  * @}
  */ 

/* Private define ------------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_SRAM_Private_Defines Private_Defines
  * @{
  */
  
/**
  * @}
  */ 

/* Private macro -------------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_SRAM_Private_Macros Private_Macros
  * @{
  */  
  
/**
  * @}
  */ 

/* Private variables ---------------------------------------------------------*/

/** @defgroup STM3210E_EVAL_SRAM_Private_Variables Private_Variables
  * @{
  */       
SRAM_HandleTypeDef sramHandle;
static FSMC_NORSRAM_TimingTypeDef Timing;

/**
  * @}
  */ 

/* Private function prototypes -----------------------------------------------*/

/** @defgroup STM3210E_EVAL_SRAM_Private_Function_Prototypes Private_Function_Prototypes
  * @{
  */ 
static void SRAM_MspInit(void); 

/**
  * @}
  */
   
/* Private functions ---------------------------------------------------------*/
    
/** @defgroup STM3210E_EVAL_SRAM_Exported_Functions Exported_Functions
  * @{
  */
    
/**
  * @brief  Initializes the SRAM device.
  * @retval SRAM status
  */
uint8_t BSP_SRAM_Init(void)
{ 
  sramHandle.Instance  = FSMC_NORSRAM_DEVICE;
  sramHandle.Extended  = FSMC_NORSRAM_EXTENDED_DEVICE;
  
  /* SRAM device configuration */  
  Timing.AddressSetupTime      = 2;
  Timing.AddressHoldTime       = 1;
  Timing.DataSetupTime         = 2;
  Timing.BusTurnAroundDuration = 1;
  Timing.CLKDivision           = 2;
  Timing.DataLatency           = 2;
  Timing.AccessMode            = FSMC_ACCESS_MODE_A;
  
  sramHandle.Init.NSBank             = FSMC_NORSRAM_BANK3;
  sramHandle.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
  sramHandle.Init.MemoryType         = FSMC_MEMORY_TYPE_SRAM;
  sramHandle.Init.MemoryDataWidth    = SRAM_MEMORY_WIDTH;
  sramHandle.Init.BurstAccessMode    = SRAM_BURSTACCESS;
  sramHandle.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
  sramHandle.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
  sramHandle.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
  sramHandle.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
  sramHandle.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
  sramHandle.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
  sramHandle.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
  sramHandle.Init.WriteBurst         = SRAM_WRITEBURST;
    
  /* SRAM controller initialization */
  SRAM_MspInit();
  
  if(HAL_SRAM_Init(&sramHandle, &Timing, &Timing) != HAL_OK)
  {
    return SRAM_ERROR;
  }
  else
  {
    return SRAM_OK;
  }
}

/**
  * @brief  Reads an amount of data from the SRAM device in polling mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory   
  * @retval SRAM status
  */
uint8_t BSP_SRAM_ReadData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{ 
  if(HAL_SRAM_Read_16b(&sramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return SRAM_ERROR;
  }
  else
  {
    return SRAM_OK;
  }
}

/**
  * @brief  Reads an amount of data from the SRAM device in DMA mode.
  * @param  uwStartAddress: Read start address
  * @param  pData: Pointer to data to be read
  * @param  uwDataSize: Size of read data from the memory   
  * @retval SRAM status
  */
uint8_t BSP_SRAM_ReadData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize)
{
  if(HAL_SRAM_Read_DMA(&sramHandle, (uint32_t *)uwStartAddress, (uint32_t *)pData, uwDataSize) != HAL_OK)
  {
    return SRAM_ERROR;
  }
  else
  {
    return SRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data from the SRAM device in polling mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory   
  * @retval SRAM status
  */
uint8_t BSP_SRAM_WriteData(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize) 
{ 
  if(HAL_SRAM_Write_16b(&sramHandle, (uint32_t *)uwStartAddress, pData, uwDataSize) != HAL_OK)
  {
    return SRAM_ERROR;
  }
  else
  {
    return SRAM_OK;
  }
}

/**
  * @brief  Writes an amount of data from the SRAM device in DMA mode.
  * @param  uwStartAddress: Write start address
  * @param  pData: Pointer to data to be written
  * @param  uwDataSize: Size of written data from the memory   
  * @retval SRAM status
  */
uint8_t BSP_SRAM_WriteData_DMA(uint32_t uwStartAddress, uint16_t *pData, uint32_t uwDataSize) 
{
  if(HAL_SRAM_Write_DMA(&sramHandle, (uint32_t *)uwStartAddress, (uint32_t *)pData, uwDataSize) != HAL_OK)
  {
    return SRAM_ERROR;
  }
  else
  {
    return SRAM_OK;
  } 
}

/**
  * @brief  Handles SRAM DMA transfer interrupt request.
  * @retval None
  */
void BSP_SRAM_DMA_IRQHandler(void)
{
  HAL_DMA_IRQHandler(sramHandle.hdma); 
}

/**
  * @brief  Initializes SRAM MSP.
  * @retval None
  */
static void SRAM_MspInit(void)
{
  static DMA_HandleTypeDef hdma1;
  GPIO_InitTypeDef gpioinitstruct = {0};
  /* Enable FSMC clock */
  __HAL_RCC_FSMC_CLK_ENABLE();
  
  /* Enable DMA1 and DMA2 clocks */
  __HAL_RCC_DMA1_CLK_ENABLE();

  /* Enable GPIOs clock */
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOG_CLK_ENABLE();
  
  /* Common GPIO configuration */
  gpioinitstruct.Mode      = GPIO_MODE_AF_PP;
  gpioinitstruct.Pull      = GPIO_NOPULL;
  gpioinitstruct.Speed     = GPIO_SPEED_FREQ_HIGH;
  
/*-- GPIO Configuration ------------------------------------------------------*/
  /*!< SRAM Data lines configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_8 | GPIO_PIN_9 |
                                GPIO_PIN_10 | GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct); 
  
  gpioinitstruct.Pin = GPIO_PIN_7 | GPIO_PIN_8 | GPIO_PIN_9 | GPIO_PIN_10 |
                                GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13 | GPIO_PIN_14 | 
                                GPIO_PIN_15;
  HAL_GPIO_Init(GPIOE, &gpioinitstruct);
  
  /*!< SRAM Address lines configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
                                GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_12 | GPIO_PIN_13 | 
                                GPIO_PIN_14 | GPIO_PIN_15;
  HAL_GPIO_Init(GPIOF, &gpioinitstruct);
  
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1 | GPIO_PIN_2 | GPIO_PIN_3 | 
                                GPIO_PIN_4 | GPIO_PIN_5;
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);
  
  gpioinitstruct.Pin = GPIO_PIN_11 | GPIO_PIN_12 | GPIO_PIN_13; 
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);
   
  /*!< NOE and NWE configuration */  
  gpioinitstruct.Pin = GPIO_PIN_4 |GPIO_PIN_5;
  HAL_GPIO_Init(GPIOD, &gpioinitstruct);
  
  /*!< NE3 configuration */
  gpioinitstruct.Pin = GPIO_PIN_10 |GPIO_PIN_12; 
  HAL_GPIO_Init(GPIOG, &gpioinitstruct);
  
  /*!< NBL0, NBL1 configuration */
  gpioinitstruct.Pin = GPIO_PIN_0 | GPIO_PIN_1; 
  HAL_GPIO_Init(GPIOE, &gpioinitstruct); 

  /* Configure common DMA parameters */
  hdma1.Init.Direction           = DMA_MEMORY_TO_MEMORY;
  hdma1.Init.PeriphInc           = DMA_PINC_ENABLE;
  hdma1.Init.MemInc              = DMA_MINC_ENABLE;
  hdma1.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
  hdma1.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
  hdma1.Init.Mode                = DMA_NORMAL;
  hdma1.Init.Priority            = DMA_PRIORITY_HIGH;

  hdma1.Instance = DMA1_Channel1;
  
  /* Deinitialize the Stream for new transfer */
  HAL_DMA_DeInit(&hdma1);
  
  /* Configure the DMA Stream */
  HAL_DMA_Init(&hdma1);
  
  /* Associate the DMA handle to the FSMC SRAM one */
  sramHandle.hdma = &hdma1;
  
  /* NVIC configuration for DMA transfer complete interrupt */
  HAL_NVIC_SetPriority(DMA1_Channel1_IRQn, 0xC, 0);
  HAL_NVIC_EnableIRQ(DMA1_Channel1_IRQn);   
  
} 
/**
  * @}
  */  
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 
  
/**
  * @}
  */ 

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
