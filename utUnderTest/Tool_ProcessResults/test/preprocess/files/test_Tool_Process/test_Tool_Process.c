// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/Tool.h"
#include "utExecutionAndResults/utUnderTest/src/Tool_Process.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_Tool.h"

void setUp(void)
{
  memset(Buffer_u8, 0, (64U));

  Head_u32 = 0;
  Tail_u32 = 0;
  Count_u32 = 0;

  StatusFlg_u32 = 0;
  Mode_e = Tool_modeIdle_e;
}

void tearDown(void)
{ }

void test_Tool_Process_WhenRunModeWithData_ProcessesBuffer2(void)
{
  Mode_e = Tool_modeRun_e;
  Count_u32 = 1U;

  const uint8_t cycle = 0x01;

  for(uint32_t i = 0U; i < (64U); i++)
{
    uint8_t popped = 0x00;
    uint8_t expected = (uint8_t)(popped ^ cycle);

    Tool_Pop_u8_CMockExpectAnyArgsAndReturn(40, 1U);
    Tool_Pop_u8_CMockIgnoreArg_testA(41);
    Tool_Pop_u8_CMockReturnMemThruPtr_value_pu8(42, &popped, sizeof(uint8_t));

    Tool_Push_u8_CMockExpectAndReturn(44, expected, 0);
  }

  Tool_Process();
}

void test_Tool_Process_WhenNotRunModeWithData_ProcessesBuffer2(void)
{
  Mode_e = Tool_modeIdle_e;
  Count_u32 = 1U;

  const uint8_t cycle = 0x01;

  Tool_Process();
}