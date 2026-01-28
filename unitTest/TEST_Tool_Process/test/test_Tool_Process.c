#include "Tool.h"
#include "Tool_Process.h"
#include "mock_Tool.h"
#include "unity.h"
#include <stdint.h>
#include <string.h>

/*==================[test helper functions]==================================*/

/**
 * @brief Reset all global variables before each test
 */
void setUp(void) {
  /* Reset buffer */
  memset(Buffer_u8, 0, TOOL_BUFFER_SIZE_U32);

  /* Reset ring buffer indices and count */
  Head_u32 = 0;
  Tail_u32 = 0;
  Count_u32 = 0;

  /* Reset status and mode */
  StatusFlg_u32 = 0;
  Mode_e = Tool_modeIdle_e;
}

static void Tool_AssertStateAndBuffer(uint8_t        expectedMode_e,
                                      uint32_t       expectedCount_u32,
                                      uint32_t       expectedHead_u32,
                                      uint32_t       expectedTail_u32,
                                      uint32_t       expectedStatusFlg_u32,
                                      const uint8_t *expectedBuf_pcu8,
                                      uint32_t       expectedBufLen_u32)
{
  TEST_ASSERT_EQUAL_UINT32(expectedMode_e,        Mode_e);
  TEST_ASSERT_EQUAL_UINT32(expectedCount_u32,     Count_u32);
  TEST_ASSERT_EQUAL_UINT32(expectedHead_u32,      Head_u32);
  TEST_ASSERT_EQUAL_UINT32(expectedTail_u32,      Tail_u32);
  TEST_ASSERT_EQUAL_UINT32(expectedStatusFlg_u32, StatusFlg_u32);

  TEST_ASSERT_NOT_NULL(expectedBuf_pcu8);
  TEST_ASSERT_EQUAL_UINT32(TOOL_BUFFER_SIZE_U32, expectedBufLen_u32);

  for (uint32_t i = 0U; i < expectedBufLen_u32; i++)
  {
    TEST_ASSERT_EQUAL_UINT8(expectedBuf_pcu8[i], Buffer_u8[i]);
  }
}

void tearDown(void) { /* nothing */ }

/*==================[test cases]=============================================*/
void test_Tool_Process_WhenRunModeWithData_ProcessesBuffer2(void) {
  Mode_e = Tool_modeRun_e;
  Count_u32 = 1U;
  uint8_t l_Buffer_u8[TOOL_BUFFER_SIZE_U32] = {0};

  const uint8_t cycle = 0x01; // only valid if l_CycleCnt_u32 was 0 before the call

  for (uint32_t i = 0U; i < TOOL_BUFFER_SIZE_U32; i++) {
    uint8_t popped = 0x00;
    uint8_t expected = (uint8_t)(popped ^ cycle);

    Tool_Pop_u8_ExpectAnyArgsAndReturn(1U);
    Tool_Pop_u8_ReturnThruPtr_value_pu8(&popped);
    Tool_Push_u8_ExpectAndReturn(expected, 0);
  }

  Tool_Process();

  Tool_AssertStateAndBuffer(Tool_modeRun_e,
                            1U,
                            0U,
                            0U,
                            0U,
                            l_Buffer_u8,
                            TOOL_BUFFER_SIZE_U32);
}

void test_Tool_Process_WhenNotRunModeWithData_ProcessesBuffer2(void) {
  Mode_e = Tool_modeIdle_e;
  Count_u32 = 1U;
  uint8_t l_Buffer_u8[TOOL_BUFFER_SIZE_U32] = {0};

  /* No mock expectations: in Idle mode nothing must be popped/pushed */
  Tool_Process();

  Tool_AssertStateAndBuffer(Tool_modeIdle_e,
                            1U,
                            0U,
                            0U,
                            0U,
                            l_Buffer_u8,
                            TOOL_BUFFER_SIZE_U32);
}

void test_Tool_Process_WhenNotRunModeWithNoData_ProcessesBuffer2(void) {
  Mode_e = Tool_modeIdle_e;
  Count_u32 = 0U;
  uint8_t l_Buffer_u8[TOOL_BUFFER_SIZE_U32] = {0};

  Tool_Process();

  Tool_AssertStateAndBuffer(Tool_modeIdle_e,
                            0U,
                            0U,
                            0U,
                            0U,
                            l_Buffer_u8,
                            TOOL_BUFFER_SIZE_U32);
}

void test_Tool_Process_WhenRunModeWithNoData_ProcessesBuffer2(void) {
  Mode_e = Tool_modeRun_e;
  Count_u32 = 0U;
  uint8_t l_Buffer_u8[TOOL_BUFFER_SIZE_U32] = {0};

  Tool_Process();

  Tool_AssertStateAndBuffer(Tool_modeRun_e,
                            0U,
                            0U,
                            0U,
                            0U,
                            l_Buffer_u8,
                            TOOL_BUFFER_SIZE_U32);
}

