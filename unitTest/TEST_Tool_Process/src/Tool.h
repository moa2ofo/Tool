/**
 * \file Tool.h
 * \brief Tool module public interface.
 * \author ChatGPT
 * \date 2026-01-23
 *
 * \defgroup Tool Tool Module
 * @{
 */

#ifndef TOOL_H
#define TOOL_H

#include <stdbool.h>
#include <stdint.h>

/*==================[macros]=================================================*/

#define TOOL_BUFFER_SIZE_U32 (64U)
#define TOOL_MAX_CRC_LEN_U32 (256U)

#define TOOL_STATUS_INIT_U32 (1U << 0U)
#define TOOL_STATUS_ERR_U32 (1U << 1U)
#define TOOL_STATUS_OVF_U32 (1U << 2U)
#define TOOL_STATUS_UDF_U32 (1U << 3U)

#define TOOL_CRC_INIT_U32 (0xFFFFFFFFUL)
#define TOOL_CRC_POLY_U32 (0xEDB88320UL)

/*==================[local data]=============================================*/

/* File-static variables: no prefix and PascalCasing (per naming convention). */
extern uint8_t Buffer_u8[TOOL_BUFFER_SIZE_U32];
extern uint32_t Head_u32;
extern uint32_t Tail_u32;
extern uint32_t Count_u32;

extern uint32_t StatusFlg_u32;

/*==================[types]==================================================*/

/**
 * \brief Tool operating mode.
 */
typedef enum { Tool_modeIdle_e = 0, Tool_modeRun_e = 1, Tool_modeDiag_e = 2 } Tool_mode_e;

extern Tool_mode_e Mode_e;
/*==================[function prototypes]====================================*/

/**
 * @brief Initialize the Tool module.
 *
 * @details
 * **Goal of the function**
 *
 * Bring the Tool module into a known, deterministic state and mark it as
 * initialized.
 *
 * The processing logic:
 * - Reset ring-buffer bookkeeping:
 *   - Set `Head_u32 = 0`, `Tail_u32 = 0`, `Count_u32 = 0`.
 * - Clear the internal buffer content with a bounded loop over
 * `TOOL_BUFFER_SIZE_U32`.
 * - Initialize operating mode:
 *   - Set `Mode_e = Tool_modeIdle_e`.
 * - Initialize status flags:
 *   - Clear `StatusFlg_u32`.
 *   - Set `TOOL_STATUS_INIT_U32`.
 *
 * @par Interface summary
 *
 * | Interface                 | In | Out | Data type / Signature     | Param |
 * Data factor | Data offset | Data size | Data range                 | Data
 * unit |
 * |--------------------------|:--:|:---:|----------------------------|:-----:|------------:|------------:|----------:|----------------------------|-----------|
 * | Tool internal buffer      |    |  X  | uint8_t[TOOL_BUFFER_SIZE]  |   -   |
 * 1 |           0 |        64 | 0..255                      | [-]      | | Tool
 * ring indices         |    |  X  | uint32_t (static)          |   -   | 1 | 0
 * |         1 | 0..TOOL_BUFFER_SIZE_U32-1   | [-]      | | Tool element count
 * |    |  X  | uint32_t (static)          |   -   |           1 |           0 |
 * 1 | 0..TOOL_BUFFER_SIZE_U32     | [-]      | | Tool mode                 | |
 * X  | Tool_mode_e (static)       |   -   |           1 |           0 | 1 | 0 /
 * 1 / 2                   | [-]      | | Tool status flags         |    |  X  |
 * uint32_t (static)          |   -   |           1 |           0 |         1 |
 * bitmask                      | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * :Head_u32 = 0;
 * :Tail_u32 = 0;
 * :Count_u32 = 0;
 * :for i in [0..TOOL_BUFFER_SIZE_U32-1];
 * :Buffer_u8[i] = 0;
 * :Mode_e = Tool_modeIdle_e;
 * :StatusFlg_u32 = 0;
 * :StatusFlg_u32 |= TOOL_STATUS_INIT_U32;
 * stop
 * @enduml
 *
 * @return void
 */
void Tool_Init(void);

/**
 * @brief De-initialize the Tool module.
 *
 * @details
 * **Goal of the function**
 *
 * Bring the Tool module back to a safe, deterministic state and clear runtime
 * flags.
 *
 * The processing logic:
 * - Set `Mode_e = Tool_modeIdle_e`.
 * - Clear `StatusFlg_u32` (removes INIT/ERR/OVF/UDF flags).
 * - Reset ring-buffer bookkeeping:
 *   - Set `Head_u32 = 0`, `Tail_u32 = 0`, `Count_u32 = 0`.
 * - Clear the internal buffer content with a bounded loop over
 * `TOOL_BUFFER_SIZE_U32`.
 *
 * @par Interface summary
 *
 * | Interface                 | In | Out | Data type / Signature     | Param |
 * Data factor | Data offset | Data size | Data range                 | Data
 * unit |
 * |--------------------------|:--:|:---:|----------------------------|:-----:|------------:|------------:|----------:|----------------------------|-----------|
 * | Tool internal buffer      |    |  X  | uint8_t[TOOL_BUFFER_SIZE]  |   -   |
 * 1 |           0 |        64 | 0..255                      | [-]      | | Tool
 * ring indices         |    |  X  | uint32_t (static)          |   -   | 1 | 0
 * |         1 | 0..TOOL_BUFFER_SIZE_U32-1   | [-]      | | Tool element count
 * |    |  X  | uint32_t (static)          |   -   |           1 |           0 |
 * 1 | 0..TOOL_BUFFER_SIZE_U32     | [-]      | | Tool mode                 | |
 * X  | Tool_mode_e (static)       |   -   |           1 |           0 | 1 | 0 /
 * 1 / 2                   | [-]      | | Tool status flags         |    |  X  |
 * uint32_t (static)          |   -   |           1 |           0 |         1 |
 * bitmask                      | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * :Mode_e = Tool_modeIdle_e;
 * :StatusFlg_u32 = 0;
 * :Head_u32 = 0;
 * :Tail_u32 = 0;
 * :Count_u32 = 0;
 * :for i in [0..TOOL_BUFFER_SIZE_U32-1];
 * :Buffer_u8[i] = 0;
 * stop
 * @enduml
 *
 * @return void
 */
void Tool_DeInit(void);

/**
 * @brief Set the Tool operating mode.
 *
 * @details
 * **Goal of the function**
 *
 * Validate and apply a requested operating mode for the Tool module.
 *
 * The processing logic:
 * - If the module is not initialized (`TOOL_STATUS_INIT_U32` not set):
 *   - Set `TOOL_STATUS_ERR_U32`.
 *   - Return `1`.
 * - Else if `mode` is not one of:
 *   - `Tool_modeIdle_e`, `Tool_modeRun_e`, `Tool_modeDiag_e`:
 *   - Set `TOOL_STATUS_ERR_U32`.
 *   - Return `2`.
 * - Else:
 *   - Set `Mode_e = mode`.
 *   - Clear `TOOL_STATUS_ERR_U32`.
 *   - Return `0`.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Data type / Signature | Param | Data
 * factor | Data offset | Data size | Data range | Data unit |
 * |---------------------|:--:|:---:|------------------------|:-----:|------------:|------------:|----------:|------------|-----------|
 * | mode                | X  |     | Tool_mode_e           |   X   | 1 | 0 | 1
 * | 0 / 1 / 2  | [-]      | | Tool init flag      | X  |     | uint32_t
 * (static)     |   -   |           - |           - |         - | bitmask    |
 * [-]      | | Tool mode           |    |  X  | Tool_mode_e (static)  |   -   |
 * 1 |           0 |         1 | 0 / 1 / 2  | [-]      | | Tool status flags   |
 * X  |  X  | uint32_t (static)     |   -   |           1 |           0 | 1 |
 * bitmask    | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * if ((StatusFlg_u32 & TOOL_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
 *   :return 1;
 * else (init)
 *   if (mode is invalid) then (invalid)
 *     :StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
 *     :return 2;
 *   else (valid)
 *     :Mode_e = mode;
 *     :StatusFlg_u32 &= ~TOOL_STATUS_ERR_U32;
 *     :return 0;
 *   endif
 * endif
 * @enduml
 *
 * @param mode
 * Requested operating mode.
 *
 * @return uint8_t
 * Return code:
 * - 0: mode accepted
 * - 1: module not initialized
 * - 2: invalid mode parameter
 */
uint8_t Tool_SetMode_u8(Tool_mode_e mode);

/**
 * @brief Get a packed snapshot of Tool status.
 *
 * @details
 * **Goal of the function**
 *
 * Provide a single 32-bit status word that contains:
 * - Current internal status flags (bitmask), with:
 *   - Bits [1:0] overwritten by the current mode encoding.
 * - Current ring-buffer fill level (Count) in the upper 16 bits.
 *
 * The processing logic:
 * - Copy `StatusFlg_u32` into a local snapshot.
 * - Encode `Mode_e` in bits [1:0] by:
 *   - Clearing bits [1:0] in the snapshot.
 *   - OR-ing `(uint32_t)Mode_e & 0x3`.
 * - Encode `Count_u32` into bits [31:16] by:
 *   - Keeping only lower 16 bits of the snapshot.
 *   - OR-ing `((Count_u32 & 0xFFFF) << 16)`.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Data type / Signature | Param | Data
 * factor | Data offset | Data size | Data range                       | Data
 * unit |
 * |---------------------|:--:|:---:|------------------------|:-----:|------------:|------------:|----------:|----------------------------------|-----------|
 * | Tool status flags   | X  |     | uint32_t (static)     |   -   | 1 | 0 | 1
 * | bitmask                          | [-]      | | Tool mode           | X  |
 * | Tool_mode_e (static)  |   -   |           1 |           0 |         1 | 0 /
 * 1 / 2                         | [-]      | | Tool element count  | X  |     |
 * uint32_t (static)     |   -   |           1 |           0 |         1 |
 * 0..TOOL_BUFFER_SIZE_U32          | [-]      | | packed status       |    |  X
 * | uint32_t              |   -   |           1 |           0 |         1 |
 * bits[31:16]=count, bits[1:0]=mode| [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * :l_status_u32 = StatusFlg_u32;
 * :l_status_u32 bits[1:0] = (uint32_t)Mode_e & 0x3;
 * :l_status_u32 bits[31:16] = (Count_u32 & 0xFFFF);
 * :return l_status_u32;
 * stop
 * @enduml
 *
 * @return uint32_t
 * Packed status word:
 * - bits[31:16] = current ring-buffer element count (lower 16 bits)
 * - bits[1:0]   = current mode (0/1/2)
 * - other bits  = internal status flags (implementation-defined masking)
 */
uint32_t Tool_GetStatus_u32(void);

/**
 * @brief Compute a CRC-32 (LSB-first) over a byte array (bounded length).
 *
 * @details
 * **Goal of the function**
 *
 * Compute a CRC value over input data using a simple bitwise CRC-32 algorithm.
 * The computation is bounded to `TOOL_MAX_CRC_LEN_U32` bytes for deterministic
 * runtime.
 *
 * The processing logic:
 * - If `data_pcu8 == NULL` or `length_u32 == 0`:
 *   - Return the initial CRC seed (`0xFFFFFFFF`).
 * - Else:
 *   - Let `l_len_u32 = min(length_u32, TOOL_MAX_CRC_LEN_U32)`.
 *   - Initialize CRC with `TOOL_CRC_INIT_U32`.
 *   - For each byte in `0..l_len_u32-1`:
 *     - XOR CRC with the byte value.
 *     - For 8 bits:
 *       - If LSB of CRC is 1: shift right and XOR with `TOOL_CRC_POLY_U32`.
 *       - Else: shift right.
 * - Finalize by XOR-ing CRC with `0xFFFFFFFF`.
 *
 * @par Interface summary
 *
 * | Interface        | In | Out | Data type / Signature     | Param | Data
 * factor | Data offset | Data size | Data range                | Data unit |
 * |-----------------|:--:|:---:|----------------------------|:-----:|------------:|------------:|----------:|---------------------------|-----------|
 * | data_pcu8       | X  |     | const uint8_t*            |   X   | 1 | 0 | 1
 * | pointer / NULL            | [-]      | | length_u32      | X  |     |
 * uint32_t                  |   X   |           1 |           0 |         1 |
 * 0..0xFFFFFFFF             | [byte]   | | TOOL_MAX_CRC... | X  |     |
 * uint32_t (macro)          |   -   |           1 |           0 |         1 |
 * 256                        | [byte]   | | crc_u32         |    |  X  |
 * uint32_t                  |   -   |           1 |           0 |         1 |
 * 0..0xFFFFFFFF             | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * if (data_pcu8 == NULL or length_u32 == 0) then (yes)
 *   :return 0xFFFFFFFF;
 * else (no)
 *   :l_len = min(length_u32, TOOL_MAX_CRC_LEN_U32);
 *   :crc = 0xFFFFFFFF;
 *   :for each byte;
 *   :crc ^= byte;
 *   :repeat 8 times;
 *     if (crc & 1) then (yes)
 *       :crc = (crc >> 1) ^ POLY;
 *     else (no)
 *       :crc = (crc >> 1);
 *     endif
 *   :end;
 *   :crc ^= 0xFFFFFFFF;
 *   :return crc;
 * endif
 * stop
 * @enduml
 *
 * @param data_pcu8
 * Pointer to the input byte array.
 *
 * @param length_u32
 * Requested number of bytes to process. The effective processed length is
 * limited to `TOOL_MAX_CRC_LEN_U32`.
 *
 * @return uint32_t
 * Computed CRC-32 value. If input is NULL or length is 0, returns `0xFFFFFFFF`.
 */
uint32_t Tool_ComputeCrc_u32(const uint8_t *data_pcu8, uint32_t length_u32);

/**
 * @brief Push one byte into the Tool ring buffer.
 *
 * @details
 * **Goal of the function**
 *
 * Insert a byte into the internal ring buffer with overflow protection.
 *
 * The processing logic:
 * - If module is not initialized:
 *   - Set `TOOL_STATUS_ERR_U32` and return `2`.
 * - Else if the buffer is full (`Count_u32 >= TOOL_BUFFER_SIZE_U32`):
 *   - Set `TOOL_STATUS_OVF_U32` and return `1`.
 * - Else:
 *   - Store `value_u8` at `Buffer_u8[Head_u32]`.
 *   - Increment `Head_u32` modulo `TOOL_BUFFER_SIZE_U32`.
 *   - Increment `Count_u32`.
 *   - Clear `TOOL_STATUS_OVF_U32`.
 *   - Return `0`.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Data type / Signature    | Param | Data
 * factor | Data offset | Data size | Data range                 | Data unit |
 * |---------------------|:--:|:---:|---------------------------|:-----:|------------:|------------:|----------:|----------------------------|-----------|
 * | value_u8            | X  |     | uint8_t                  |   X   | 1 | 0 |
 * 1 | 0..255                      | [-]      | | Tool init flag      | X  | |
 * uint32_t (static)        |   -   |           - |           - |         - |
 * bitmask                     | [-]      | | Buffer_u8           |    |  X  |
 * uint8_t[TOOL_BUFFER_SIZE]|   -   |           1 |           0 |        64 |
 * 0..255                      | [-]      | | Head/Count          | X  |  X  |
 * uint32_t (static)        |   -   |           1 |           0 |         1 |
 * 0..TOOL_BUFFER_SIZE_U32    | [-]      | | Status flags        | X  |  X  |
 * uint32_t (static)        |   -   |           1 |           0 |         1 |
 * bitmask                     | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * if ((StatusFlg_u32 & TOOL_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
 *   :return 2;
 * elseif (Count_u32 >= TOOL_BUFFER_SIZE_U32) then (full)
 *   :StatusFlg_u32 |= TOOL_STATUS_OVF_U32;
 *   :return 1;
 * else (space)
 *   :Buffer_u8[Head_u32] = value_u8;
 *   :Head_u32 = (Head_u32 + 1) % TOOL_BUFFER_SIZE_U32;
 *   :Count_u32++;
 *   :StatusFlg_u32 &= ~TOOL_STATUS_OVF_U32;
 *   :return 0;
 * endif
 * stop
 * @enduml
 *
 * @param value_u8
 * Byte value to push into the ring buffer.
 *
 * @return uint8_t
 * Return code:
 * - 0: pushed successfully
 * - 1: buffer full (overflow)
 * - 2: module not initialized
 */
uint8_t Tool_Push_u8(uint8_t value_u8);

/**
 * @brief Pop one byte from the Tool ring buffer.
 *
 * @details
 * **Goal of the function**
 *
 * Extract the oldest byte from the internal ring buffer with pointer and
 * underflow checks.
 *
 * The processing logic:
 * - If `value_pu8 == NULL`:
 *   - Set `TOOL_STATUS_ERR_U32` and return `3`.
 * - Else if module is not initialized:
 *   - Set `TOOL_STATUS_ERR_U32`, write `*value_pu8 = 0`, and return `2`.
 * - Else if the buffer is empty (`Count_u32 == 0`):
 *   - Write `*value_pu8 = 0` and return `1`.
 * - Else:
 *   - Read `Buffer_u8[Tail_u32]` into `*value_pu8`.
 *   - Clear `Buffer_u8[Tail_u32]` to `0`.
 *   - Increment `Tail_u32` modulo `TOOL_BUFFER_SIZE_U32`.
 *   - Decrement `Count_u32`.
 *   - Return `0`.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Data type / Signature    | Param | Data
 * factor | Data offset | Data size | Data range                 | Data unit |
 * |---------------------|:--:|:---:|---------------------------|:-----:|------------:|------------:|----------:|----------------------------|-----------|
 * | value_pu8           | X  |  X  | uint8_t*                 |   X   | 1 | 0 |
 * 1 | pointer / NULL            | [-]      | | Tool init flag      | X  |     |
 * uint32_t (static)        |   -   |           - |           - |         - |
 * bitmask                     | [-]      | | Buffer_u8           | X  |  X  |
 * uint8_t[TOOL_BUFFER_SIZE]|   -   |           1 |           0 |        64 |
 * 0..255                      | [-]      | | Tail/Count          | X  |  X  |
 * uint32_t (static)        |   -   |           1 |           0 |         1 |
 * 0..TOOL_BUFFER_SIZE_U32    | [-]      | | Status flags        | X  |  X  |
 * uint32_t (static)        |   -   |           1 |           0 |         1 |
 * bitmask                     | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * if (value_pu8 == NULL) then (null)
 *   :StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
 *   :return 3;
 * elseif ((StatusFlg_u32 & TOOL_STATUS_INIT_U32) == 0) then (not init)
 *   :StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
 *   :*value_pu8 = 0;
 *   :return 2;
 * elseif (Count_u32 == 0) then (empty)
 *   :*value_pu8 = 0;
 *   :return 1;
 * else (has data)
 *   :*value_pu8 = Buffer_u8[Tail_u32];
 *   :Buffer_u8[Tail_u32] = 0;
 *   :Tail_u32 = (Tail_u32 + 1) % TOOL_BUFFER_SIZE_U32;
 *   :Count_u32--;
 *   :return 0;
 * endif
 * stop
 * @enduml
 *
 * @param value_pu8
 * Output pointer receiving the popped byte.
 *
 * @return uint8_t
 * Return code:
 * - 0: popped successfully
 * - 1: buffer empty (underflow)
 * - 2: module not initialized
 * - 3: NULL output pointer
 */
uint8_t Tool_Pop_u8(uint8_t *value_pu8, int testA);

/**
 * @brief Run a lightweight self-test on Tool internal state.
 *
 * @details
 * **Goal of the function**
 *
 * Perform a simple runtime check of internal invariants and a bounded checksum
 * over the internal buffer. The function may set flags to indicate anomalous
 * conditions.
 *
 * The processing logic:
 * - If `Count_u32 > TOOL_BUFFER_SIZE_U32`:
 *   - Set `TOOL_STATUS_ERR_U32`.
 *   - Return `1`.
 * - Else:
 *   - Compute `l_sum_u32 = sum(Buffer_u8[i])` for all `i` in
 * `0..TOOL_BUFFER_SIZE_U32-1`.
 *   - If `l_sum_u32 > (255 * TOOL_BUFFER_SIZE_U32)`:
 *     - Set `TOOL_STATUS_UDF_U32`.
 *     - Return `2`.
 *   - Else:
 *     - Clear `TOOL_STATUS_UDF_U32`.
 *     - Return `0`.
 *
 * @par Interface summary
 *
 * | Interface            | In | Out | Data type / Signature     | Param | Data
 * factor | Data offset | Data size | Data range              | Data unit |
 * |---------------------|:--:|:---:|----------------------------|:-----:|------------:|------------:|----------:|-------------------------|-----------|
 * | Buffer_u8           | X  |     | uint8_t[TOOL_BUFFER_SIZE]  |   -   | 1 |
 * 0 |        64 | 0..255                   | [-]      | | Count_u32           |
 * X  |     | uint32_t (static)          |   -   |           1 |           0 |
 * 1 | 0..TOOL_BUFFER_SIZE_U32  | [-]      | | Status flags        | X  |  X  |
 * uint32_t (static)          |   -   |           1 |           0 |         1 |
 * bitmask                   | [-]      | | return code         |    |  X  |
 * uint8_t                    |   -   |           1 |           0 |         1 |
 * 0 / 1 / 2                | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * if (Count_u32 > TOOL_BUFFER_SIZE_U32) then (bad)
 *   :StatusFlg_u32 |= TOOL_STATUS_ERR_U32;
 *   :return 1;
 * else (ok)
 *   :sum = 0;
 *   :for i in [0..TOOL_BUFFER_SIZE_U32-1];
 *   :sum += Buffer_u8[i];
 *   if (sum > 255*TOOL_BUFFER_SIZE_U32) then (unexpected)
 *     :StatusFlg_u32 |= TOOL_STATUS_UDF_U32;
 *     :return 2;
 *   else (normal)
 *     :StatusFlg_u32 &= ~TOOL_STATUS_UDF_U32;
 *     :return 0;
 *   endif
 * endif
 * stop
 * @enduml
 *
 * @return uint8_t
 * Return code:
 * - 0: self-test passed
 * - 1: internal invariant violated (Count too large)
 * - 2: unexpected checksum condition (UDF flag set)
 */
uint8_t Tool_RunTst_u8(void);

/**
 * @brief Run one deterministic processing step of the Tool module.
 *
 * @details
 * **Goal of the function**
 *
 * Execute a bounded amount of work per call. When in RUN mode and data is
 * available, the function rotates data through the ring buffer while applying a
 * simple XOR transformation based on a function-static cycle counter.
 *
 * The processing logic:
 * - Maintain a function-static cycle counter:
 *   - `l_CycleCnt_u32++` each call (wraps naturally).
 * - Run a bounded loop for `TOOL_BUFFER_SIZE_U32` iterations:
 *   - If `Mode_e == Tool_modeRun_e` AND `Count_u32 != 0`:
 *     - Pop one byte into `l_val_u8`.
 *     - Transform: `l_val_u8 = l_val_u8 ^ (l_CycleCnt_u32 & 0xFF)`.
 *     - Push the transformed byte back into the buffer.
 *   - Else:
 *     - Do no buffer manipulation (keep deterministic timing).
 *
 * @par Interface summary
 *
 * | Interface                 | In | Out | Data type / Signature      | Param |
 * Data factor | Data offset | Data size | Data range                 | Data
 * unit |
 * |--------------------------|:--:|:---:|-----------------------------|:-----:|------------:|------------:|----------:|----------------------------|-----------|
 * | Mode_e                   | X  |     | Tool_mode_e (static)        |   -   |
 * 1 |           0 |         1 | 0 / 1 / 2                   | [-]      | |
 * Count_u32                | X  |  X  | uint32_t (static)           |   -   |
 * 1 |           0 |         1 | 0..TOOL_BUFFER_SIZE_U32     | [-]      | | Ring
 * buffer              | X  |  X  | uint8_t[TOOL_BUFFER_SIZE]   |   -   | 1 | 0
 * |        64 | 0..255                      | [-]      | | l_CycleCnt_u32 | X
 * |  X  | uint32_t (static local)     |   -   |           1 |           0 | 1 |
 * 0..0xFFFFFFFF               | [-]      | | Tool_Pop_u8 / Tool_Push_u8 | X | X
 * | function calls              |   -   |           - |           - |         -
 * | see respective interfaces   | [-]      |
 *
 * @par Activity diagram (PlantUML)
 *
 * @startuml
 * start
 * :l_CycleCnt_u32++;
 * :for iter in [0..TOOL_BUFFER_SIZE_U32-1];
 * if (Mode_e == RUN and Count_u32 != 0) then (yes)
 *   :Tool_Pop_u8(&val);
 *   :val = val XOR (l_CycleCnt_u32 & 0xFF);
 *   :Tool_Push_u8(val);
 * else (no)
 *   :no-op (bounded);
 * endif
 * :endfor
 * stop
 * @enduml
 *
 * @return void
 */
//void Tool_Process(void);
uint8_t ModuleName_FunctionEx_(uint32_t in_id_u32, int32_t in_mode_e, const uint8_t *in_cfg_pcu8, uint8_t *in_buf_pu8, uint16_t in_bufLen_u16, uint8_t *out_status_pu8);

#endif /* TOOL_H */

/** @} */
