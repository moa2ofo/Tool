/**
 * \file Tool.c
 * \brief Tool module implementation.
 * \author ChatGPT
 * \date 2026-01-23
 *
 * \defgroup Tool Tool Module
 * @{
 */

#include "Tool.h"
#include <stddef.h>

/*==================[local macros]===========================================*/

#define TOOL_CRC_INIT_U32 (0xFFFFFFFFUL)
#define TOOL_CRC_POLY_U32 (0xEDB88320UL)

/*==================[local data]=============================================*/

/* File-static variables: no prefix and PascalCasing (per naming convention). */
static uint8_t Buffer_u8[TOOL_BUFFER_SIZE_U32];
static uint32_t Head_u32;
static uint32_t Tail_u32;
static uint32_t Count_u32;

static uint32_t StatusFlg_u32;
static Tool_mode_e Mode_e;

/*==================[global functions]=======================================*/

void Tool_Init(void) {
  uint32_t l_i_u32 = 0U;

  /* Reset indices and counters. */
  Head_u32 = 0U;
  Tail_u32 = 0U;
  Count_u32 = 0U;

  /* Clear buffer content (bounded loop). */
  for(l_i_u32 = 0U; l_i_u32 < TOOL_BUFFER_SIZE_U32; l_i_u32++) {
    Buffer_u8[l_i_u32] = 0U;
  }

  /* Initialize status and mode. */
  Mode_e = Tool_modeIdle_e;
  StatusFlg_u32 = 0U;
  StatusFlg_u32 |= TOOL_STATUS_INIT_U32;
}

void Tool_DeInit(void) {
  uint32_t l_i_u32 = 0U;

  /* Bring module to a safe and deterministic state. */
  Mode_e = Tool_modeIdle_e;
  StatusFlg_u32 = 0U;

  /* Reset ring buffer bookkeeping. */
  Head_u32 = 0U;
  Tail_u32 = 0U;
  Count_u32 = 0U;

  /* Clear buffer content (bounded loop). */
  for(l_i_u32 = 0U; l_i_u32 < TOOL_BUFFER_SIZE_U32; l_i_u32++) {
    Buffer_u8[l_i_u32] = 0U;
  }
}

uint8_t Tool_SetMode_u8(Tool_mode_e mode) {
  uint8_t l_ret_u8 = 0U;

  /* Reject mode changes if not initialized. */
  if((StatusFlg_u32 & TOOL_STATUS_INIT_U32) == 0U) {
    StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
    l_ret_u8 = 1U;
  } else if((mode != Tool_modeIdle_e) && (mode != Tool_modeRun_e) && (mode != Tool_modeDiag_e)) {
    /* Invalid mode request. */
    StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
    l_ret_u8 = 2U;
  } else {
    /* Accept the new mode. */
    Mode_e = mode;
    StatusFlg_u32 &= ~TOOL_STATUS_ERR_U32;
    l_ret_u8 = 0U;
  }

  return l_ret_u8;
}

uint32_t Tool_GetStatus_u32(void) {
  uint32_t l_status_u32 = 0U;
  uint32_t l_mode_u32 = 0U;

  /* Snapshot internal flags first. */
  l_status_u32 = StatusFlg_u32;

  /* Encode mode in lowest 2 bits (kept simple). */
  l_mode_u32 = (uint32_t)Mode_e;
  l_status_u32 &= 0xFFFFFFFCUL;
  l_status_u32 |= (l_mode_u32 & 0x3UL);

  /* Expose current fill level in upper 16 bits. */
  l_status_u32 &= 0x0000FFFFUL;
  l_status_u32 |= ((Count_u32 & 0xFFFFUL) << 16U);

  return l_status_u32;
}

uint32_t Tool_ComputeCrc_u32(const uint8_t *data_pcu8, uint32_t length_u32) {
  uint32_t l_crc_u32 = TOOL_CRC_INIT_U32;
  uint32_t l_i_u32 = 0U;
  uint32_t l_len_u32 = 0U;

  /* Defensive: null pointer or zero length -> return init CRC. */
  if((data_pcu8 == NULL) || (length_u32 == 0U)) {
    l_crc_u32 = TOOL_CRC_INIT_U32;
  } else {
    /* Bounded iteration: cap to TOOL_MAX_CRC_LEN_U32. */
    l_len_u32 = (length_u32 > TOOL_MAX_CRC_LEN_U32) ? TOOL_MAX_CRC_LEN_U32 : length_u32;

    for(l_i_u32 = 0U; l_i_u32 < l_len_u32; l_i_u32++) {
      uint32_t l_bit_u32 = 0U;

      l_crc_u32 ^= (uint32_t)data_pcu8[l_i_u32];
      for(l_bit_u32 = 0U; l_bit_u32 < 8U; l_bit_u32++) {
        l_crc_u32 = ((l_crc_u32 & 1U) != 0U) ? ((l_crc_u32 >> 1U) ^ TOOL_CRC_POLY_U32) : (l_crc_u32 >> 1U);
      }
    }
  }

  /* Finalize CRC. */
  l_crc_u32 ^= 0xFFFFFFFFUL;

  return l_crc_u32;
}

uint8_t Tool_Push_u8(uint8_t value_u8) {
  uint8_t l_ret_u8 = 0U;

  /* Check initialization. */
  if((StatusFlg_u32 & TOOL_STATUS_INIT_U32) == 0U) {
    StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
    l_ret_u8 = 2U;
  } else if(Count_u32 >= TOOL_BUFFER_SIZE_U32) {
    /* Overflow condition. */
    StatusFlg_u32 |= TOOL_STATUS_OVF_U32;
    l_ret_u8 = 1U;
  } else {
    /* Store into ring buffer and update bookkeeping. */
    Buffer_u8[Head_u32] = value_u8;
    Head_u32 = (Head_u32 + 1U) % TOOL_BUFFER_SIZE_U32;
    Count_u32++;

    StatusFlg_u32 &= ~TOOL_STATUS_OVF_U32;
    l_ret_u8 = 0U;
  }

  return l_ret_u8;
}

uint8_t Tool_Pop_u8(uint8_t *value_pu8) {
  uint8_t l_ret_u8 = 0U;

  /* Validate output pointer. */
  if(value_pu8 == NULL) {
    StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
    l_ret_u8 = 3U;
  } else if((StatusFlg_u32 & TOOL_STATUS_INIT_U32) == 0U) {
    StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
    *value_pu8 = 0U;
    l_ret_u8 = 2U;
  } else if(Count_u32 == 0U) {
    /* Underflow condition. */
    *value_pu8 = 0U;
    l_ret_u8 = 1U;
  } else {
    /* Read from ring buffer and update bookkeeping. */
    *value_pu8 = Buffer_u8[Tail_u32];
    Buffer_u8[Tail_u32] = 0U;

    Tail_u32 = (Tail_u32 + 1U) % TOOL_BUFFER_SIZE_U32;
    Count_u32--;

    l_ret_u8 = 0U;
  }

  return l_ret_u8;
}

void Tool_Clear(void) {
  uint32_t l_index_u32;

  Head_u32 = 0U;
  Tail_u32 = 0U;
  Count_u32 = 0U;

  for(l_index_u32 = 0U; l_index_u32 < TOOL_BUFFER_SIZE_U32; l_index_u32++) {
    Buffer_u8[l_index_u32] = 0U;
  }

  StatusFlg_u32 &= ~TOOL_STATUS_ERR_U32;
  StatusFlg_u32 &= ~TOOL_STATUS_OVF_U32;
}
uint8_t Tool_RunTst_u8(void) {
  uint8_t l_ret_u8 = 0U;
  uint32_t l_sum_u32 = 0U;
  uint32_t l_i_u32 = 0U;

  /* Simple self-test: validate internal invariants and compute a checksum. */
  if(Count_u32 > TOOL_BUFFER_SIZE_U32) {
    StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
    l_ret_u8 = 1U;
  } else {
    for(l_i_u32 = 0U; l_i_u32 < TOOL_BUFFER_SIZE_U32; l_i_u32++) {
      l_sum_u32 += (uint32_t)Buffer_u8[l_i_u32];
    }

    /* Mark unexpected condition if checksum is suspiciously large (defensive).
     */
    if(l_sum_u32 > (255UL * TOOL_BUFFER_SIZE_U32)) {
      StatusFlg_u32 |= TOOL_STATUS_UDF_U32;
      l_ret_u8 = 2U;
    } else {
      StatusFlg_u32 &= ~TOOL_STATUS_UDF_U32;
      l_ret_u8 = 0U;
    }
  }

  return l_ret_u8;
}

void Tool_Process(void) {
  static uint32_t l_CycleCnt_u32 = 0U;
  uint32_t l_iter_u32;
  uint8_t l_val_u8;

  l_CycleCnt_u32++;

  for(l_iter_u32 = 0U; l_iter_u32 < TOOL_BUFFER_SIZE_U32; l_iter_u32++) {
    if((Mode_e == Tool_modeRun_e) && (Count_u32 != 0U)) {
      Tool_Pop_u8(&l_val_u8);
      l_val_u8 ^= (uint8_t)(l_CycleCnt_u32 & 0xFFU);
      Tool_Push_u8(l_val_u8);
    } else {
      /* no-op for deterministic timing */
    }
  }
}
/** @} */
