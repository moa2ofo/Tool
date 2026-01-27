// CEEDLING NOTICE: This generated file only to be consumed by CMock

#ifndef _TOOL_H_ // Ceedling-generated include guard
#define _TOOL_H_

extern uint8_t Buffer_u8[(64U)];
extern uint32_t Head_u32;
extern uint32_t Tail_u32;
extern uint32_t Count_u32;

extern uint32_t StatusFlg_u32;

typedef enum { Tool_modeIdle_e = 0, Tool_modeRun_e = 1, Tool_modeDiag_e = 2 } Tool_mode_e;

extern Tool_mode_e Mode_e;
void Tool_Init(void);
void Tool_DeInit(void);
uint8_t Tool_SetMode_u8(Tool_mode_e mode);
uint32_t Tool_GetStatus_u32(void);
uint32_t Tool_ComputeCrc_u32(const uint8_t *data_pcu8, uint32_t length_u32);
uint8_t Tool_Push_u8(uint8_t value_u8);
uint8_t Tool_Pop_u8(uint8_t *value_pu8);
uint8_t Tool_RunTst_u8(void);
uint8_t ModuleName_FunctionEx_(uint32_t in_id_u32, int32_t in_mode_e, const uint8_t *in_cfg_pcu8, uint8_t *in_buf_pu8, uint16_t in_bufLen_u16, uint8_t *out_status_pu8);

#endif // _TOOL_H_
