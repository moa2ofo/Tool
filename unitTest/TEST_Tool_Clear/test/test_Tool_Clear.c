#include "Tool.h"
#include "Tool_Clear.h"
#include "mock_Tool.h"
#include "unity.h"
#include <stdint.h>
#include <string.h>

/*==================[test helper functions]==================================*/

void setUp(void) {
  /* Reset Tool internal globals to a known state */
  (void)memset(Buffer_u8, 0, TOOL_BUFFER_SIZE_U32);

  Head_u32 = 0U;
  Tail_u32 = 0U;
  Count_u32 = 0U;

  StatusFlg_u32 = 0U;
  Mode_e = Tool_modeIdle_e;
}

void tearDown(void) { /* nothing */ }

/*==================[test cases]=============================================*/

void test_Tool_Clear_ResetsRingBufferIndices(void) {
  /* Arrange */
  Head_u32 = 10U;
  Tail_u32 = 20U;
  Count_u32 = 15U;

  /* Act */
  Tool_Clear();

  /* Assert */
  TEST_ASSERT_EQUAL_UINT32(0U, Head_u32);
  TEST_ASSERT_EQUAL_UINT32(0U, Tail_u32);
  TEST_ASSERT_EQUAL_UINT32(0U, Count_u32);
}

void test_Tool_Clear_ClearsBufferContent(void) {
  /* Arrange */
  for(uint32_t i = 0U; i < TOOL_BUFFER_SIZE_U32; i++) {
    Buffer_u8[i] = 0xFFU;
  }

  /* Act */
  Tool_Clear();

  /* Assert */
  for(uint32_t i = 0U; i < TOOL_BUFFER_SIZE_U32; i++) {
    TEST_ASSERT_EQUAL_UINT8(0U, Buffer_u8[i]);
  }
}

void test_Tool_Clear_ClearsErrorFlag(void) {
  /* Arrange */
  StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_ERR_U32) != 0U);

  /* Act */
  Tool_Clear();

  /* Assert */
  TEST_ASSERT_FALSE((StatusFlg_u32 & TOOL_STATUS_ERR_U32) != 0U);
}

void test_Tool_Clear_ClearsOverflowFlag(void) {
  /* Arrange */
  StatusFlg_u32 |= TOOL_STATUS_OVF_U32;
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_OVF_U32) != 0U);

  /* Act */
  Tool_Clear();

  /* Assert */
  TEST_ASSERT_FALSE((StatusFlg_u32 & TOOL_STATUS_OVF_U32) != 0U);
}

void test_Tool_Clear_PreservesInitFlag(void) {
  /* Arrange */
  StatusFlg_u32 = TOOL_STATUS_INIT_U32;
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_INIT_U32) != 0U);

  /* Act */
  Tool_Clear();

  /* Assert */
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_INIT_U32) != 0U);
}

void test_Tool_Clear_ClearsOnlyErrorAndOverflowFlags(void) {
  /* Arrange */
  StatusFlg_u32 = (TOOL_STATUS_INIT_U32 | TOOL_STATUS_ERR_U32 | TOOL_STATUS_OVF_U32 | TOOL_STATUS_UDF_U32);

  /* Pre-assert */
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_INIT_U32) != 0U);
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_ERR_U32) != 0U);
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_OVF_U32) != 0U);
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_UDF_U32) != 0U);

  /* Act */
  Tool_Clear();

  /* Assert: only ERR/OVF cleared, INIT/UDF preserved */
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_INIT_U32) != 0U);
  TEST_ASSERT_FALSE((StatusFlg_u32 & TOOL_STATUS_ERR_U32) != 0U);
  TEST_ASSERT_FALSE((StatusFlg_u32 & TOOL_STATUS_OVF_U32) != 0U);
  TEST_ASSERT_TRUE((StatusFlg_u32 & TOOL_STATUS_UDF_U32) != 0U);
}
