/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file    i2c.c
 * @brief   This file provides code for the configuration
 *          of the I2C instances.
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
#include "i2c.h"

/* USER CODE BEGIN 0 */
#define LCD_ADDRESS 0x27
#define I2C2_TX_DMA_REQUEST 0xE
#define I2C2_RX_DMA_REQUEST 0xD

volatile uint8_t tx_finished;
volatile uint8_t rx_finished;
/* USER CODE END 0 */

/* I2C2 init function */
void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  LL_I2C_InitTypeDef I2C_InitStruct = {0};

  LL_GPIO_InitTypeDef GPIO_InitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = {0};

  /** Initializes the peripherals clocks
  */
  PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_I2C2;
  PeriphClkInitStruct.I2c2ClockSelection = RCC_I2C2CLKSOURCE_PCLK1;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOA);
  LL_AHB2_GRP1_EnableClock(LL_AHB2_GRP1_PERIPH_GPIOB);
  /**I2C2 GPIO Configuration
  PA15   ------> I2C2_SDA
  PB15   ------> I2C2_SCL
  */
  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.Pin = LL_GPIO_PIN_15;
  GPIO_InitStruct.Mode = LL_GPIO_MODE_ALTERNATE;
  GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
  GPIO_InitStruct.Alternate = LL_GPIO_AF_4;
  LL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* Peripheral clock enable */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_I2C2);

  /* I2C2 DMA Init */

  /* I2C2_TX Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_2, LL_DMAMUX_REQ_I2C2_TX);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_2, LL_DMA_DIRECTION_MEMORY_TO_PERIPH);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_2, LL_DMA_MDATAALIGN_BYTE);

  /* I2C2_RX Init */
  LL_DMA_SetPeriphRequest(DMA1, LL_DMA_CHANNEL_3, LL_DMAMUX_REQ_I2C2_RX);

  LL_DMA_SetDataTransferDirection(DMA1, LL_DMA_CHANNEL_3, LL_DMA_DIRECTION_PERIPH_TO_MEMORY);

  LL_DMA_SetChannelPriorityLevel(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PRIORITY_LOW);

  LL_DMA_SetMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MODE_NORMAL);

  LL_DMA_SetPeriphIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PERIPH_NOINCREMENT);

  LL_DMA_SetMemoryIncMode(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MEMORY_INCREMENT);

  LL_DMA_SetPeriphSize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_PDATAALIGN_BYTE);

  LL_DMA_SetMemorySize(DMA1, LL_DMA_CHANNEL_3, LL_DMA_MDATAALIGN_BYTE);

  /* USER CODE BEGIN I2C2_Init 1 */
    // /* TX DMA Configuration */
    //   DMAMUX1_Channel2->CCR |= (I2C2_TX_DMA_REQUEST << DMAMUX_CxCR_DMAREQ_ID_Pos); // Set I2C2_RX as Channel 2 Request
    //   DMA1_Channel2->CCR  |= DMA_CCR_DIR;   // mem to per
    //   DMA1_Channel2->CCR  &= ~DMA_CCR_PL;   // low priority
    //   DMA1_Channel2->CCR  &= ~DMA_CCR_CIRC; // normal mode
    //   DMA1_Channel2->CCR  &= ~DMA_CCR_PINC; // no per inc
    //   DMA1_Channel2->CCR  |= DMA_CCR_MINC;  // mem inc
    //   DMA1_Channel2->CCR  &= ~DMA_CCR_PSIZE;// per size byte
    //   DMA1_Channel2->CCR  &= ~DMA_CCR_MSIZE;// mem size byte
    //   /* RX DMA Configuration */
    //   DMAMUX1_Channel3->CCR |= (I2C2_RX_DMA_REQUEST << DMAMUX_CxCR_DMAREQ_ID_Pos); // Set I2C2_TX as Channel 3 Request
    //   DMA1_Channel3->CCR  &= ~DMA_CCR_DIR;   // per to mem
    //   DMA1_Channel3->CCR  &= ~DMA_CCR_PL;   // low priority
    //   DMA1_Channel3->CCR  &= ~DMA_CCR_CIRC; // normal mode
    //   DMA1_Channel3->CCR  &= ~DMA_CCR_PINC; // no per inc
    //   DMA1_Channel3->CCR  |= DMA_CCR_MINC;  // mem inc
    //   DMA1_Channel3->CCR  &= ~DMA_CCR_PSIZE;// per size byte
    //   DMA1_Channel3->CCR  &= ~DMA_CCR_MSIZE;// mem size byte

  /* USER CODE END I2C2_Init 1 */

  /** I2C Initialization
  */
  LL_I2C_EnableAutoEndMode(I2C2);
  LL_I2C_DisableOwnAddress2(I2C2);
  LL_I2C_DisableGeneralCall(I2C2);
  LL_I2C_EnableClockStretching(I2C2);
  I2C_InitStruct.PeripheralMode = LL_I2C_MODE_I2C;
  I2C_InitStruct.Timing = 0x20303E5D;
  I2C_InitStruct.AnalogFilter = LL_I2C_ANALOGFILTER_ENABLE;
  I2C_InitStruct.DigitalFilter = 0;
  I2C_InitStruct.OwnAddress1 = 0;
  I2C_InitStruct.TypeAcknowledge = LL_I2C_ACK;
  I2C_InitStruct.OwnAddrSize = LL_I2C_OWNADDRESS1_7BIT;
  LL_I2C_Init(I2C2, &I2C_InitStruct);
  LL_I2C_SetOwnAddress2(I2C2, 0, LL_I2C_OWNADDRESS2_NOMASK);
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/* USER CODE BEGIN 1 */
/**
 * @brief Sends data over I2C2 using DMA
 * @param data
 */
void i2c2CommandDMA(uint8_t data)
{

    DMA1_Channel2->CCR &= ~DMA_CCR_EN; // disable channel
    DMA1_Channel2->CNDTR = 1;
    DMA1_Channel2->CMAR = (uint32_t)&data;
    DMA1_Channel2->CCR |= DMA_CCR_EN;

    I2C2->CR1 |= I2C_CR1_TXDMAEN;
    I2C2->CR2 |= (LCD_ADDRESS << 1U);
    I2C2->CR2 &= ~I2C_CR2_RD_WRN; // write operation
    I2C2->CR2 &= ~I2C_CR2_NBYTES;
    I2C2->CR2 |= (1 << 16U);
    I2C2->CR2 |= I2C_CR2_AUTOEND;
    I2C2->CR2 |= I2C_CR2_START;

    while (!tx_finished)
        ;
    tx_finished = 0;
}

void i2c2WriteMemDMA(uint8_t *pData, uint8_t memadd, uint8_t size)
{
    uint8_t combinedData[size + 1];

    combinedData[0] = memadd;
    memcpy((combinedData + 0x1), pData, size);

    DMA1_Channel2->CCR &= ~DMA_CCR_EN; // disable channel
    DMA1_Channel2->CNDTR = 1;
    DMA1_Channel2->CMAR = (uint32_t)&combinedData;
    DMA1_Channel2->CCR |= DMA_CCR_EN;

    I2C2->CR1 |= I2C_CR1_TXDMAEN;
    I2C2->CR2 |= (LCD_ADDRESS << 1U);
    I2C2->CR2 &= ~I2C_CR2_RD_WRN; // write operation
    I2C2->CR2 &= ~I2C_CR2_NBYTES;
    I2C2->CR2 |= (1 << 16U);
    I2C2->CR2 |= I2C_CR2_AUTOEND;
    I2C2->CR2 |= I2C_CR2_START;

    while (!tx_finished)
        ;
    tx_finished = 0;
}

void i2c2ReadMemoryDMA(uint8_t *pData, uint8_t memadd, uint8_t size)
{
    I2C2->CR2 |= (LCD_ADDRESS << 1U);
    I2C2->CR2 &= ~I2C_CR2_RD_WRN; // write operation
    I2C2->CR2 &= ~I2C_CR2_NBYTES;
    I2C2->CR2 |= (1 << I2C_CR2_NBYTES_Pos);
    I2C2->CR2 |= I2C_CR2_AUTOEND;
    I2C2->CR2 |= I2C_CR2_START;

    while (!(I2C2->ISR & I2C_ISR_TXIS))
        ;
    I2C2->TXDR = (uint32_t)memadd;
    while (!(I2C2->ISR & I2C_ISR_TXE))
        ;

    /* Set up DMA Receiver*/
    DMA1_Channel3->CCR &= ~DMA_CCR_EN;
    DMA1_Channel3->CNDTR = size;
    DMA1_Channel3->CMAR = (uint32_t)pData;
    DMA1_Channel3->CCR |= DMA_CCR_EN;

    I2C2->CR1 |= I2C_CR1_RXDMAEN;
    I2C2->CR2 |= (LCD_ADDRESS << 1U);
    I2C2->CR2 |= I2C_CR2_RD_WRN; // write operation
    I2C2->CR2 &= ~I2C_CR2_NBYTES;
    I2C2->CR2 |= (size << I2C_CR2_NBYTES_Pos);
    I2C2->CR2 |= I2C_CR2_AUTOEND;
    I2C2->CR2 |= I2C_CR2_START;

    while (!rx_finished)
        ;
    rx_finished = 0;
}

void DMA1_Channel2_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF2)
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF2;
        tx_finished = 1;
    }
}

void DMA1_Channel3_IRQHandler(void)
{
    if (DMA1->ISR & DMA_ISR_TCIF3)
    {
        DMA1->IFCR |= DMA_IFCR_CTCIF3;
        rx_finished = 1;
    }
}
/* USER CODE END 1 */
