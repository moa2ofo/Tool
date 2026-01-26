// CEEDLING NOTICE: This generated file only to be consumed for test runner creation

#include "utExecutionAndResults/utUnderTest/src/Tool.h"
#include "utExecutionAndResults/utUnderTest/src/Tool_Clear.h"
#include "utExecutionAndResults/utUnderTest/build/vendor/unity/src/unity.h"
#include "mock_Tool.h"

void setUp(void)
{
  (void)memset(Buffer_u8, 0, (64U));

  Head_u32 = 0U;
  Tail_u32 = 0U;
  Count_u32 = 0U;

  StatusFlg_u32 = 0U;
  Mode_e = Tool_modeIdle_e;
}

void tearDown(void)
{ }

void test_Tool_Clear_ResetsRingBufferIndices(void)
{
  Head_u32 = 10U;
  Tail_u32 = 20U;
  Count_u32 = 15U;

  Tool_Clear();

  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((0U)), (UNITY_INT)(UNITY_UINT32)((Head_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(36), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((0U)), (UNITY_INT)(UNITY_UINT32)((Tail_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(37), UNITY_DISPLAY_STYLE_UINT32);
  UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT32)((0U)), (UNITY_INT)(UNITY_UINT32)((Count_u32)), (
 ((void *)0)
 ), (UNITY_UINT)(38), UNITY_DISPLAY_STYLE_UINT32);
}

void test_Tool_Clear_ClearsBufferContent(void)
{
  for(uint32_t i = 0U; i < (64U); i++)
{ Buffer_u8[i] = 0xFFU; }

  Tool_Clear();

  for(uint32_t i = 0U; i < (64U); i++)
{ UnityAssertEqualNumber((UNITY_INT)(UNITY_UINT8 )((0U)), (UNITY_INT)(UNITY_UINT8 )((Buffer_u8[i])), (
                                                       ((void *)0)
                                                       ), (UNITY_UINT)(49), UNITY_DISPLAY_STYLE_UINT8); }
}

void test_Tool_Clear_ClearsErrorFlag(void)
{
  StatusFlg_u32 |= (1U << 1U);
  do { if (((StatusFlg_u32 & (1U << 1U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((55))); } } while (0);

  Tool_Clear();

  do { if (!((StatusFlg_u32 & (1U << 1U)) != 0U))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((61))); } } while (0);
}

void test_Tool_Clear_ClearsOverflowFlag(void)
{
  StatusFlg_u32 |= (1U << 2U);
  do { if (((StatusFlg_u32 & (1U << 2U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((67))); } } while (0);

  Tool_Clear();

  do { if (!((StatusFlg_u32 & (1U << 2U)) != 0U))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((73))); } } while (0);
}

void test_Tool_Clear_PreservesInitFlag(void)
{
  StatusFlg_u32 = (1U << 0U);
  do { if (((StatusFlg_u32 & (1U << 0U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((79))); } } while (0);

  Tool_Clear();

  do { if (((StatusFlg_u32 & (1U << 0U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((85))); } } while (0);
}

void test_Tool_Clear_ClearsOnlyErrorAndOverflowFlags(void)
{
  StatusFlg_u32 = ((1U << 0U) | (1U << 1U) | (1U << 2U) | (1U << 3U));

  do { if (((StatusFlg_u32 & (1U << 0U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((93))); } } while (0);
  do { if (((StatusFlg_u32 & (1U << 1U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((94))); } } while (0);
  do { if (((StatusFlg_u32 & (1U << 2U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((95))); } } while (0);
  do { if (((StatusFlg_u32 & (1U << 3U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((96))); } } while (0);

  Tool_Clear();

  do { if (((StatusFlg_u32 & (1U << 0U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((102))); } } while (0);
  do { if (!((StatusFlg_u32 & (1U << 1U)) != 0U))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((103))); } } while (0);
  do { if (!((StatusFlg_u32 & (1U << 2U)) != 0U))
{ } else { UnityFail( ((" Expected FALSE Was TRUE")), (UNITY_UINT)((104))); } } while (0);
  do { if (((StatusFlg_u32 & (1U << 3U)) != 0U))
{ } else { UnityFail( ((" Expected TRUE Was FALSE")), (UNITY_UINT)((105))); } } while (0);
}