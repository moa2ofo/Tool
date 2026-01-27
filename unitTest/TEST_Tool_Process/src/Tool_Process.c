
#include "Tool_Process.h"


uint8_t Buffer_u8[TOOL_BUFFER_SIZE_U32] = {0};
uint32_t Head_u32 = 0;
uint32_t Tail_u32 = 0;
uint32_t Count_u32 = 0;

uint32_t StatusFlg_u32 = 0;
Tool_mode_e Mode_e = Tool_modeIdle_e;



/* FUNCTION TO TEST */


void Tool_Process(void) {
  static uint32_t l_CycleCnt_u32 = 0U;
  uint32_t l_iter_u32;
  uint8_t l_val_u8;

  l_CycleCnt_u32++;

  for(l_iter_u32 = 0U; l_iter_u32 < TOOL_BUFFER_SIZE_U32; l_iter_u32++) {
    if((Mode_e == Tool_modeRun_e) && (Count_u32 != 0U)) {
      Tool_Pop_u8(&l_val_u8,0);
      l_val_u8 ^= (uint8_t)(l_CycleCnt_u32 & 0xFFU);
      Tool_Push_u8(l_val_u8);
    } else {
      /* Deterministic timing no-op */
    }
  }
}
