
#include "Tool_Clear.h"
#include "Tool.h"

uint8_t Buffer_u8[TOOL_BUFFER_SIZE_U32] = {0};
uint32_t Head_u32 = 0;
uint32_t Tail_u32 = 0;
uint32_t Count_u32 = 0;

uint32_t StatusFlg_u32 = 0;
Tool_mode_e Mode_e = Tool_modeIdle_e;
/* FUNCTION TO TEST */

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
