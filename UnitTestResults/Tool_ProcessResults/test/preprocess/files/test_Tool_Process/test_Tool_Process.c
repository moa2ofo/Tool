// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/Tool.h"
#include "utExecutionAndResults/utUnderTest/src/Tool_Process.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_Tool.h"

static void Tool_Reset(uint8_t expectedMode_e, uint32_t expectedCount_u32, uint32_t expectedHead_u32,
                      uint32_t expectedTail_u32, uint32_t expectedStatusFlg_u32,
                      const uint8_t *expectedBuf_pcu8, uint32_t expectedBufLen_u32)
{
  Mode_e = expectedMode_e;
  Count_u32 = expectedCount_u32;
  Head_u32 = expectedHead_u32;
  Tail_u32 = expectedTail_u32;
  StatusFlg_u32 = expectedStatusFlg_u32;
  for(uint32_t i = 0U; i < expectedBufLen_u32; i++)
{
    Buffer_u8[i] = expectedBuf_pcu8[i];
  }
}

static void Tool_AssertCheck(uint8_t expectedMode_e, uint32_t expectedCount_u32, uint32_t expectedHead_u32,
                      uint32_t expectedTail_u32, uint32_t expectedStatusFlg_u32,
                      const uint8_t *expectedBuf_pcu8, uint32_t expectedBufLen_u32)
{
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expectedMode_e)), (UNITY_INT)(UNITY_UINT32)((Mode_e)), (
 ((void *)0)
 ), (UNITY_UINT)(25), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expectedCount_u32)), (UNITY_INT)(UNITY_UINT32)((Count_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(26), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expectedHead_u32)), (UNITY_INT)(UNITY_UINT32)((Head_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(27), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expectedTail_u32)), (UNITY_INT)(UNITY_UINT32)((Tail_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(28), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((expectedStatusFlg_u32)), (UNITY_INT)(UNITY_UINT32)((StatusFlg_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(29), UNITY_DISPLAY_STYLE_UINT32);
  do { if ((((expectedBuf_pcu8)) !=
 ((void *)0)
 ))
{ } else { UnityFail( (((" Expected Non-NULL"))), (UNITY_UINT)(((30)))); } } while (0);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)(((64U))), (UNITY_INT)(UNITY_UINT32)((expectedBufLen_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(31), UNITY_DISPLAY_STYLE_UINT32);

  for(uint32_t i = 0U; i < expectedBufLen_u32; i++)
{
    UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((expectedBuf_pcu8[i])), (UNITY_INT)(UNITY_UINT8 )((Buffer_u8[i])), (
   ((void *)0)
   ), (UNITY_UINT)(34), UNITY_DISPLAY_STYLE_UINT8);
  }
}

void setUp(void)
{
  uint8_t l_Buffer_u8[(64U)] = {0};
  Tool_Reset(Tool_modeIdle_e,0,0,0,0,l_Buffer_u8,(64U));
}

void tearDown(void)
{
}

void test_Tool_Process_WhenRunModeWithData_ProcessesBuffer2(void)
{
  Mode_e = Tool_modeRun_e;
  Count_u32 = 1U;
  uint8_t l_Buffer_u8[(64U)] = {0};

  const uint8_t cycle = 0x01;

  for(uint32_t i = 0U; i < (64U); i++)
{
    uint8_t popped = 0x00;
    uint8_t expected = (uint8_t)(popped ^ cycle);

    Tool_Pop_u8_CMockExpectAnyArgsAndReturn(59, 1U);
    Tool_Pop_u8_CMockReturnMemThruPtr_value_pu8(60, &popped, sizeof(uint8_t));
    Tool_Push_u8_CMockExpectAndReturn(61, expected, 0);
  }

  Tool_Process();

  Tool_AssertCheck(Tool_modeRun_e, 1U, 0U, 0U, 0U, l_Buffer_u8, (64U));
}

void test_Tool_Process_WhenNotRunModeWithData_ProcessesBuffer2(void)
{
  Mode_e = Tool_modeIdle_e;
  Count_u32 = 1U;
  uint8_t l_Buffer_u8[(64U)] = {0};

  Tool_Process();

  Tool_AssertCheck(Tool_modeIdle_e, 1U, 0U, 0U, 0U, l_Buffer_u8, (64U));
}

void test_Tool_Process_WhenNotRunModeWithNoData_ProcessesBuffer2(void)
{
  Mode_e = Tool_modeIdle_e;
  Count_u32 = 0U;
  uint8_t l_Buffer_u8[(64U)] = {0};

  Tool_Process();

  Tool_AssertCheck(Tool_modeIdle_e, 0U, 0U, 0U, 0U, l_Buffer_u8, (64U));
}

void test_Tool_Process_WhenRunModeWithNoData_ProcessesBuffer2(void)
{
  Mode_e = Tool_modeRun_e;
  Count_u32 = 0U;
  uint8_t l_Buffer_u8[(64U)] = {0};

  Tool_Process();

  Tool_AssertCheck(Tool_modeRun_e, 0U, 0U, 0U, 0U, l_Buffer_u8, (64U));
}