/*
 * can.c
 *
 *  Created on: Sep 7, 2024
 *      Author: WANG
 */


#include "can.h"
#include "qbuffer.h"
#include "cli.h"


#ifdef _USE_HW_CAN

CAN_HandleTypeDef hcan;

typedef struct
{
  uint32_t prescaler;
  uint32_t sjw;
  uint32_t tseg1;
  uint32_t tseg2;
} can_baud_cfg_t;

const can_baud_cfg_t can_baud_cfg_80m_normal[] =
    {
        {20, 1, 15, 2}, // 100K, 87.5%
        {16, 1, 15, 2}, // 125K, 87.5%
        {9,  1, 13, 2}, // 250K, 87.5%
        {4,  1, 15, 2}, // 500K, 87.5%
        {2,  1, 15, 2}, // 1M,   87.5%
    };

const can_baud_cfg_t *p_baud_data   = can_baud_cfg_80m_normal;


const uint32_t dlc_len_tbl[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 12, 16, 20, 24, 32, 48, 64};

const uint32_t dlc_tbl[] =
    {
        CAN_DLC_BYTES_0,
        CAN_DLC_BYTES_1,
        CAN_DLC_BYTES_2,
        CAN_DLC_BYTES_3,
        CAN_DLC_BYTES_4,
        CAN_DLC_BYTES_5,
        CAN_DLC_BYTES_6,
        CAN_DLC_BYTES_7,
        CAN_DLC_BYTES_8
    };

static const uint32_t mode_tbl[] =
    {
        CAN_MODE_NORMAL,
        CAN_MODE_LOOPBACK
    };


typedef struct
{
  bool is_init;
  bool is_open;

  uint32_t err_code;
  uint8_t  state;
  uint32_t recovery_cnt;

  uint32_t q_rx_full_cnt;
  uint32_t q_tx_full_cnt;
  uint32_t fifo_full_cnt;
  uint32_t fifo_lost_cnt;

  uint32_t fifo_idx;
  uint32_t enable_int;
  uint32_t interrupt_line;
  CanMode_t  mode;
  CanFrame_t frame;
  CanBaud_t  baud;
  CanBaud_t  baud_data;

  uint32_t rx_cnt;
  uint32_t tx_cnt;

  CAN_HandleTypeDef hcan;
  CAN_FilterTypeDef hcan_filter;
  bool (*handler)(uint8_t ch, CanEvent_t evt, can_msg_t *arg);

  qbuffer_t q_msg;
  can_msg_t can_msg[CAN_MSG_RX_BUF_MAX];
} can_tbl_t;

static can_tbl_t can_tbl[CAN_MAX_CH];

static volatile uint32_t err_int_cnt = 0;

#ifdef _USE_HW_RTOS
static osMutexId mutex_lock;
#else
static bool is_lock = false;
#endif

#ifdef _USE_HW_CLI
static void cliCan(cli_args_t *args);
#endif

static void canErrUpdate(uint8_t ch);





bool canInit(void)
{
  bool ret = true;

  uint8_t i;


  for(i = 0; i < CAN_MAX_CH; i++)
  {
    can_tbl[i].is_init  = true;
    can_tbl[i].is_open  = false;
    can_tbl[i].err_code = CAN_ERR_NONE;
    can_tbl[i].state    = 0;
    can_tbl[i].recovery_cnt = 0;

    can_tbl[i].q_rx_full_cnt = 0;
    can_tbl[i].q_tx_full_cnt = 0;
    can_tbl[i].fifo_full_cnt = 0;
    can_tbl[i].fifo_lost_cnt = 0;

    can_tbl[i].rx_cnt = 0;
    can_tbl[i].tx_cnt = 0;

    qbufferCreateBySize(&can_tbl[i].q_msg, (uint8_t *)&can_tbl[i].can_msg[0], sizeof(can_msg_t), CAN_MSG_RX_BUF_MAX);
  }

  #ifdef _USE_HW_RTOS
  osMutexDef(mutex_lock);
  mutex_lock = osMutexCreate (osMutex(mutex_lock));
  #else
  is_lock = false;
  #endif

#ifdef _USE_HW_CLI
  cliAdd("can", cliCan);
#endif
  return ret;
}

bool canLock(void)
{
  bool ret = true;
  uint32_t timeout = 0;


#ifdef _USE_HW_RTOS
  if (timeout > 0)
    osMutexWait(mutex_lock, timeout);
  else
    osMutexWait(mutex_lock, osWaitForever);
#else
  uint32_t pre_time;

  pre_time = millis();
  while(1)
  {
    if (is_lock == false)
      break;

    if (timeout > 0 && millis()-pre_time >= timeout)
    {
      ret = false;
      break;
    }
  }
#endif

  return ret;
}

bool canUnLock(void)
{
#ifdef _USE_HW_RTOS
  osMutexRelease(mutex_lock);
#else
  is_lock = false;
#endif

  return true;
}

bool canOpen(uint8_t ch, CanMode_t mode, CanFrame_t frame, CanBaud_t baud, CanBaud_t baud_data)
{
  bool ret = true;
  CAN_HandleTypeDef  *p_can;

  if (ch >= CAN_MAX_CH) return false;

  can_tbl[ch].hcan = hcan;

  p_can = &can_tbl[ch].hcan;

  switch(ch)
  {
    case _DEF_CAN1:
      p_can->Instance                   = CAN1;
      p_can->Init.Mode                  = mode_tbl[mode];
      p_can->Init.TimeTriggeredMode   = DISABLE;
      p_can->Init.AutoBusOff          = DISABLE;
      p_can->Init.AutoWakeUp          = DISABLE;
      p_can->Init.ReceiveFifoLocked   = DISABLE;
      p_can->Init.TransmitFifoPriority = DISABLE;
      p_can->Init.AutoRetransmission   = DISABLE;
      p_can->Init.Prescaler           = p_baud_data[baud_data].prescaler;
      p_can->Init.SyncJumpWidth       = p_baud_data[baud_data].sjw;
      p_can->Init.TimeSeg1            = p_baud_data[baud_data].tseg1;
      p_can->Init.TimeSeg2            = p_baud_data[baud_data].tseg2;


      can_tbl[ch].hcan_filter.FilterActivation = CAN_FILTER_ENABLE;
      can_tbl[ch].hcan_filter.FilterBank = 18;
      can_tbl[ch].hcan_filter.FilterFIFOAssignment = CAN_FILTER_FIFO0;
      can_tbl[ch].hcan_filter.FilterIdHigh = 0x103<<5;
      can_tbl[ch].hcan_filter.FilterIdLow = 0;
      can_tbl[ch].hcan_filter.FilterMaskIdHigh = 0x103<<5;
      can_tbl[ch].hcan_filter.FilterMaskIdLow = 0x0000;
      can_tbl[ch].hcan_filter.FilterMode = CAN_FILTERMODE_IDMASK;
      can_tbl[ch].hcan_filter.FilterScale = CAN_FILTERSCALE_32BIT;
      can_tbl[ch].hcan_filter.SlaveStartFilterBank = 20;

      can_tbl[ch].mode                = mode;
      can_tbl[ch].frame               = frame;
      can_tbl[ch].baud                = baud;
      can_tbl[ch].baud_data           = baud_data;

      ret = true;
      break;

    default:
      ret = false;
      break;
  }

  if (ret != true)
  {
    return false;
  }

  if (HAL_CAN_Init(p_can) != HAL_OK)
  {
    return false;
  }


  if (HAL_CAN_ConfigFilter(p_can, &can_tbl[ch].hcan_filter) != HAL_OK)
  {
    return false;
  }

  HAL_CAN_ActivateNotification(p_can, CAN_IT_TX_MAILBOX_EMPTY | CAN_IT_RX_FIFO0_MSG_PENDING |
                               CAN_IT_BUSOFF);

  if (HAL_CAN_Start(p_can) != HAL_OK)
  {
    return false;
  }

  can_tbl[ch].is_open = true;


  return ret;
}

bool canIsOpen(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return false;

  return can_tbl[ch].is_open;
}

void canClose(uint8_t ch)
{

}

bool canGetInfo(uint8_t ch, can_info_t *p_info)
{
  if(ch >= CAN_MAX_CH) return false;

  p_info->baud = can_tbl[ch].baud;
  p_info->baud_data = can_tbl[ch].baud_data;
  p_info->frame = can_tbl[ch].frame;
  p_info->mode = can_tbl[ch].mode;

  return true;
}

CanDlc_t canGetDlc(uint8_t length)
{
  CanDlc_t ret;

  if (length >= 64)
    ret = CAN_DLC_64;
  else if (length >= 48)
    ret = CAN_DLC_48;
  else if (length >= 32)
    ret = CAN_DLC_32;
  else if (length >= 24)
    ret = CAN_DLC_24;
  else if (length >= 20)
    ret = CAN_DLC_20;
  else if (length >= 16)
    ret = CAN_DLC_16;
  else if (length >= 12)
    ret = CAN_DLC_12;
  else if (length >= 8)
    ret = CAN_DLC_8;
  else
    ret = (CanDlc_t)length;

  return ret;
}

uint8_t canGetLen(CanDlc_t dlc)
{
  return dlc_len_tbl[(int)dlc];
}

uint32_t canMsgAvailable(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return 0;

  return qbufferAvailable(&can_tbl[ch].q_msg);
}

bool canMsgInit(can_msg_t *p_msg, CanFrame_t frame, CanIdType_t  id_type, CanDlc_t dlc)
{
  p_msg->frame   = frame;
  p_msg->id_type = id_type;
  p_msg->dlc     = dlc;
  p_msg->length  = dlc_len_tbl[dlc];
  return true;
}

bool canMsgWrite(uint8_t ch, can_msg_t *p_msg, uint32_t timeout)
{
  CAN_HandleTypeDef  *p_can;
  CAN_TxHeaderTypeDef tx_header;
  uint32_t pre_time;
  bool ret = true;


  if(ch > CAN_MAX_CH) return false;

  if (can_tbl[ch].is_open != true) return false;
  if (can_tbl[ch].err_code & CAN_ERR_PASSIVE) return false;
  if (can_tbl[ch].err_code & CAN_ERR_BUS_OFF) return false;


  p_can = &can_tbl[ch].hcan;

  switch(p_msg->id_type)
  {
    case CAN_STD :
      tx_header.IDE = CAN_ID_STD;
      break;

    case CAN_EXT :
      tx_header.IDE = CAN_ID_EXT;
      break;
  }

  tx_header.IDE          = p_msg->id;
  tx_header.RTR          = CAN_RTR_DATA;
  tx_header.DLC          = dlc_tbl[p_msg->dlc];

  if (ret == true)
  {
    can_tbl[ch].tx_cnt++;
  }
  return ret;
}

bool canMsgRead(uint8_t ch, can_msg_t *p_msg)
{
  bool ret = true;


  if(ch >= CAN_MAX_CH) return 0;

  ret = qbufferRead(&can_tbl[ch].q_msg, (uint8_t *)p_msg, 1);

  return ret;
}



uint32_t canGetError(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return 0;

  return can_tbl[ch].err_code;
}

uint32_t canGetRxCount(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return 0;

  return can_tbl[ch].rx_cnt;
}

uint32_t canGetTxCount(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return 0;

  return can_tbl[ch].tx_cnt;
}

uint32_t canGetState(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return 0;

  return HAL_CAN_GetState(&can_tbl[ch].hcan);
}

void canAttachRxInterrupt(uint8_t ch, bool (*handler)(uint8_t ch, CanEvent_t evt, can_msg_t *arg))
{
  if(ch >= CAN_MAX_CH) return;

  can_tbl[ch].handler = handler;
}

void canDetachRxInterrupt(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return;

  can_tbl[ch].handler = NULL;
}

void canRecovery(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return;

  HAL_CAN_Stop(&can_tbl[ch].hcan);
  HAL_CAN_Start(&can_tbl[ch].hcan);

  can_tbl[ch].recovery_cnt++;
}

bool canUpdate(void)
{
  bool ret = false;
  can_tbl_t *p_can;


  for (int i=0; i<CAN_MAX_CH; i++)
  {
    p_can = &can_tbl[i];


    switch(p_can->state)
    {
      case 0:
        if (p_can->err_code & CAN_ERR_BUS_OFF)
        {
          canRecovery(i);
          p_can->state = 1;
          ret = true;
        }
        break;

      case 1:
        if ((p_can->err_code & CAN_ERR_BUS_OFF) == 0)
        {
          p_can->state = 0;
        }
        break;
    }
  }

  return ret;
}

void HAL_CAN_RxFifo0MsgPendingCallback(CAN_HandleTypeDef *hcan)
{
  can_msg_t *rx_buf;
  CAN_RxHeaderTypeDef rx_header;


  rx_buf  = (can_msg_t *)qbufferPeekWrite(&can_tbl[0].q_msg);

  if (HAL_CAN_GetRxMessage(hcan, can_tbl[0].fifo_idx, &rx_header, rx_buf->data) == HAL_OK)
  {
    if(rx_header.IDE == CAN_ID_STD)
    {
      rx_buf->id      = rx_header.StdId;
      rx_buf->id_type = CAN_STD;
    }
    else
    {
      rx_buf->id      = rx_header.ExtId;
      rx_buf->id_type = CAN_EXT;
    }
    rx_buf->length = dlc_len_tbl[(rx_header.DLC >> 16) & 0x0F];
    rx_buf->dlc = canGetDlc(rx_buf->length);

    rx_buf->frame = CAN_CLASSIC;

    can_tbl[0].rx_cnt++;

    if (qbufferWrite(&can_tbl[0].q_msg, NULL, 1) != true)
    {
      can_tbl[0].q_rx_full_cnt++;
    }

    if( can_tbl[0].handler != NULL )
    {
      if ((*can_tbl[0].handler)(0, CAN_EVT_MSG, (void *)rx_buf) == true)
      {
        qbufferRead(&can_tbl[0].q_msg, NULL, 1);
      }
    }
  }
}

void canErrClear(uint8_t ch)
{
  if(ch >= CAN_MAX_CH) return;

  can_tbl[ch].err_code = CAN_ERR_NONE;
}

void canErrPrint(uint8_t ch)
{
  uint32_t err_code;


  if(ch >= CAN_MAX_CH) return;

  err_code = can_tbl[ch].err_code;

//  if (err_code & CAN_ERR_PASSIVE) logPrintf("  ERR : CAN_ERR_PASSIVE\n");
//  if (err_code & CAN_ERR_WARNING) logPrintf("  ERR : CAN_ERR_WARNING\n");
//  if (err_code & CAN_ERR_BUS_OFF) logPrintf("  ERR : CAN_ERR_BUS_OFF\n");
}

void canInfoPrint(uint8_t ch)
{
  can_tbl_t *p_can = &can_tbl[ch];

  cliPrintf("ch            : ");
  switch(ch)
  {
    case _DEF_CAN1:
      cliPrintf("_DEF_CAN1\n");
      break;
    case _DEF_CAN2:
      cliPrintf("_DEF_CAN2\n");
      break;
  }

  cliPrintf("is_open       : ");
  if (p_can->is_open)
    cliPrintf("true\n");
  else
    cliPrintf("false\n");

  cliPrintf("baud          : ");
  switch(p_can->baud)
  {
    case CAN_100K:
      cliPrintf("100K\n");
      break;
    case CAN_125K:
      cliPrintf("125K\n");
      break;
    case CAN_250K:
      cliPrintf("250\n");
      break;
    case CAN_500K:
      cliPrintf("250\n");
      break;
    case CAN_1M:
      cliPrintf("1M\n");
      break;

    default:
      break;
  }

  cliPrintf("baud data     : ");
  switch(p_can->baud)
  {
    case CAN_100K:
      cliPrintf("100K\n");
      break;
    case CAN_125K:
      cliPrintf("125K\n");
      break;
    case CAN_250K:
      cliPrintf("250\n");
      break;
    case CAN_500K:
      cliPrintf("250\n");
      break;
    case CAN_1M:
      cliPrintf("1M\n");
      break;

    case CAN_2M:
      cliPrintf("1M\n");
      break;
    case CAN_4M:
      cliPrintf("1M\n");
      break;
    case CAN_5M:
      cliPrintf("1M\n");
      break;
  }

  cliPrintf("mode          : ");
  switch(p_can->mode)
  {
    case CAN_NORMAL:
      cliPrintf("NORMAL\n");
      break;
    case CAN_MONITOR:
      cliPrintf("MONITOR\n");
      break;
    case CAN_LOOPBACK:
      cliPrintf("LOOPBACK\n");
      break;
  }

  cliPrintf("frame         : ");
  switch(p_can->frame)
  {
    case CAN_CLASSIC:
      cliPrintf("CAN_CLASSIC\n");
      break;
    case CAN_FD_NO_BRS:
      cliPrintf("CAN_FD_NO_BRS\n");
      break;
    case CAN_FD_BRS:
      cliPrintf("CAN_FD_BRS\n");
      break;
  }
}

void HAL_CAN_ErrorCallback(CAN_HandleTypeDef *hcan)
{
  uint8_t ch = _DEF_CAN1;

  err_int_cnt++;

  if (hcan->ErrorCode > 0)
  {
    can_tbl[ch].err_code |= CAN_ERR_ETC;
  }
}

void FDCAN1_IT0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&can_tbl[_DEF_CAN1].hcan);
}

void CAN2_IT0_IRQHandler(void)
{
  HAL_CAN_IRQHandler(&can_tbl[_DEF_CAN2].hcan);
}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_8;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_9;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_CAN1_2();

    /* CAN1 interrupt Init */
    HAL_NVIC_SetPriority(USB_LP_CAN1_RX0_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(USB_LP_CAN1_RX0_IRQn);
    HAL_NVIC_SetPriority(CAN1_RX1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PB8     ------> CAN_RX
    PB9     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOB, GPIO_PIN_8|GPIO_PIN_9);

    /* CAN1 interrupt Deinit */
  /* USER CODE BEGIN CAN1:USB_LP_CAN1_RX0_IRQn disable */
    /**
    * Uncomment the line below to disable the "USB_LP_CAN1_RX0_IRQn" interrupt
    * Be aware, disabling shared interrupt may affect other IPs
    */
    /* HAL_NVIC_DisableIRQ(USB_LP_CAN1_RX0_IRQn); */
  /* USER CODE END CAN1:USB_LP_CAN1_RX0_IRQn disable */

    HAL_NVIC_DisableIRQ(CAN1_RX1_IRQn);
  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}


#ifdef _USE_HW_CLI
void cliCan(cli_args_t *args)
{
  bool ret = false;


  canLock();

  if (args->argc == 1 && args->isStr(0, "info"))
  {
    for (int i=0; i<CAN_MAX_CH; i++)
    {
      canInfoPrint(i);
      cliPrintf("q_rx_full_cnt : %d\n", can_tbl[i].q_rx_full_cnt);
      cliPrintf("q_tx_full_cnt : %d\n", can_tbl[i].q_tx_full_cnt);
      cliPrintf("fifo_full_cnt : %d\n", can_tbl[i].fifo_full_cnt);
      cliPrintf("fifo_lost_cnt : %d\n", can_tbl[i].fifo_lost_cnt);
//      cliPrintf("rx error cnt  : %d\n", canGetRxErrCount(i));
//      cliPrintf("tx error cnt  : %d\n", canGetTxErrCount(i));

      canErrPrint(i);

      cliPrintf("\n");
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "read"))
  {
    uint32_t index = 0;
    uint8_t ch;

    ch = constrain(args->getData(1), 0, CAN_MAX_CH - 1);

    while(cliKeepLoop())
    {
      if (canMsgAvailable(ch))
      {
        can_msg_t msg;

        canMsgRead(ch, &msg);

        index %= 1000;
        cliPrintf("ch %d %03d(R) <- id ", ch, index++);
        if (msg.id_type == CAN_STD)
        {
          cliPrintf("std ");
        }
        else
        {
          cliPrintf("ext ");
        }
        cliPrintf(": 0x%08X, L:%02d, ", msg.id, msg.length);
        for (int i=0; i<msg.length; i++)
        {
          cliPrintf("0x%02X ", msg.data[i]);
        }
        cliPrintf("\n");
      }
    }
    ret = true;
  }

  if (args->argc == 2 && args->isStr(0, "send"))
  {
    uint32_t pre_time;
    uint32_t index = 0;
    uint32_t err_code;
    uint8_t ch;

    ch = constrain(args->getData(1), 0, CAN_MAX_CH - 1);


    err_code = can_tbl[ch].err_code;

    while(cliKeepLoop())
    {
      can_msg_t msg;

      if (millis()-pre_time >= 1000)
      {
        pre_time = millis();

        msg.frame   = CAN_CLASSIC;
        msg.id_type = CAN_EXT;
        msg.dlc     = CAN_DLC_2;
        msg.id      = 0x314;
        msg.length  = 2;
        msg.data[0] = 1;
        msg.data[1] = 2;
        if (canMsgWrite(_DEF_CAN1, &msg, 10) > 0)
        {
          index %= 1000;
          cliPrintf("ch %d %03d(T) -> id ", ch, index++);
          if (msg.id_type == CAN_STD)
          {
            cliPrintf("std ");
          }
          else
          {
            cliPrintf("ext ");
          }
          cliPrintf(": 0x%08X, L:%02d, ", msg.id, msg.length);
          for (int i=0; i<msg.length; i++)
          {
            cliPrintf("0x%02X ", msg.data[i]);
          }
          cliPrintf("\n");
        }

//        if (canGetRxErrCount(ch) > 0 || canGetTxErrCount(ch) > 0)
//        {
//          cliPrintf("ch %d ErrCnt : %d, %d\n", ch, canGetRxErrCount(_DEF_CAN1), canGetTxErrCount(_DEF_CAN1));
//        }
//
//        if (err_int_cnt > 0)
//        {
//          cliPrintf("ch %d Cnt : %d\n", ch, err_int_cnt);
//          err_int_cnt = 0;
//        }
      }

      if (can_tbl[ch].err_code != err_code)
      {
        cliPrintf("ch %d ErrCode : 0x%X\n", ch, can_tbl[ch].err_code);
        canErrPrint(ch);
        err_code = can_tbl[ch].err_code;
      }

      if (canUpdate())
      {
        cliPrintf("ch %d BusOff Recovery\n", ch);
      }


      if (canMsgAvailable(ch))
      {
        canMsgRead(ch, &msg);

        index %= 1000;
        cliPrintf("ch %d %03d(R) <- id ", ch, index++);
        if (msg.id_type == CAN_STD)
        {
          cliPrintf("std ");
        }
        else
        {
          cliPrintf("ext ");
        }
        cliPrintf(": 0x%08X, L:%02d, ", msg.id, msg.length);
        for (int i=0; i<msg.length; i++)
        {
          cliPrintf("0x%02X ", msg.data[i]);
        }
        cliPrintf("\n");
      }
    }
    ret = true;
  }

  canUnLock();

  if (ret == false)
  {
    cliPrintf("can info\n");
    cliPrintf("can read ch[0~%d]\n", CAN_MAX_CH-1);
    cliPrintf("can send ch[0~%d]\n", CAN_MAX_CH-1);
  }
}
#endif

#endif
