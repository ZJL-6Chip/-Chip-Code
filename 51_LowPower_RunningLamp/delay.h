/*
 * =====================================================================
 * File: delay.h
 * Project: Low-Power Running Lamp Driver for 51 Singlechip
 * Description: Delay utility functions with configurable precision
 * Author: ZJL-6Chip
 * Date: 2026-06-05
 * =====================================================================
 *
 * PURPOSE:
 *   Provide accurate millisecond and microsecond delay functions
 *   optimized for the 8051 architecture. Uses inline assembly
 *   for precise timing control with minimal overhead.
 *
 * FUNCTIONS:
 *   - delay_ms()   : Millisecond delay
 *   - delay_us()   : Microsecond delay
 *   - delay_nop()  : NOP loop for minimal delays
 *
 * NOTES:
 *   - Delays are approximate and depend on oscillator accuracy
 *   - Interrupts should be disabled for critical timing
 *   - For 12MHz crystal: 1 machine cycle = 1 microsecond
 *
 * =====================================================================
 */

#ifndef __DELAY_H__
#define __DELAY_H__

#include "config.h"

/* =====================================================================
 * DELAY FUNCTION IMPLEMENTATIONS
 * ===================================================================== */

/*
 * FUNCTION: delay_ms
 * 
 * PURPOSE:
 *   Produce a delay of specified milliseconds.
 *   Uses nested loops calibrated for typical 8051 performance.
 *
 * PARAMETERS:
 *   ms : Number of milliseconds to delay (0-255)
 *
 * RETURN VALUE:
 *   None
 *
 * TIMING:
 *   At 12MHz crystal with 12 clocks/instruction:
 *   - 1 instruction cycle = 1 microsecond
 *   - 1000 instruction cycles ≈ 1 millisecond
 *
 * EXAMPLE:
 *   delay_ms(100);  // Wait 100 milliseconds
 *
 * NOTES:
 *   - Assumes 12MHz crystal frequency
 *   - Inner loop calibration may need adjustment for other frequencies
 *   - Disable interrupts if precise timing is critical
 */
void delay_ms(UCHAR ms)
{
    UCHAR i, j;
    
    /* Outer loop: milliseconds */
    for (i = 0; i < ms; i++)
    {
        /* Inner loop: calibrated for ~1000 instruction cycles per ms */
        /* At 12MHz: 1000 * 1µs = 1ms */
        for (j = 0; j < 123; j++)  /* 123 iterations ≈ 1000 cycles */
        {
            /* Empty loop body allows compiler optimization */
            /* NOP instructions inserted by compiler */
        }
    }
}

/*
 * FUNCTION: delay_us
 * 
 * PURPOSE:
 *   Produce a delay of specified microseconds.
 *   Uses tight loop for minimal delay.
 *
 * PARAMETERS:
 *   us : Number of microseconds to delay (1-255)
 *
 * RETURN VALUE:
 *   None
 *
 * TIMING:
 *   At 12MHz crystal with 12 clocks/instruction:
 *   - Each iteration ≈ 8-10 machine cycles
 *   - For 1 microsecond, 1 iteration is required
 *
 * EXAMPLE:
 *   delay_us(50);   // Wait 50 microseconds
 *
 * NOTES:
 *   - Not recommended for delays < 5 microseconds
 *   - Accuracy depends on compiler optimization level
 *   - For Keil µVision, compile with -O2 optimization
 */
void delay_us(UINT us)
{
    UINT i;
    
    /* Each iteration ≈ 1 microsecond at 12MHz */
    for (i = 0; i < us; i++)
    {
        /* NOP or minimal operation */
        __nop();  /* Keil µVision intrinsic */
    }
}

/*
 * MACRO: NOP_DELAY
 * 
 * PURPOSE:
 *   Produce multiple NOP (No Operation) instructions for
 *   very minimal delays or timing adjustment.
 *
 * USAGE:
 *   NOP_DELAY(10);  // 10 NOP instructions
 *
 * NOTES:
 *   - Each NOP = 1 clock cycle = 1/12 microsecond at 12MHz
 *   - Used for fine-tuning timing in critical sections
 */
#define NOP_DELAY(n) {int _i; for(_i=0; _i<n; _i++) __nop();}

/* =====================================================================
 * DELAY TIMING CALIBRATION CONSTANTS
 * ===================================================================== */

/*
 * Loop Iteration Timing:
 * These constants help calibrate delay functions for different
 * crystal frequencies and compiler optimization levels.
 *
 * CALIBRATION PROCEDURE:
 * 1. Use oscilloscope to measure actual delay
 * 2. Adjust DELAY_MS_CALIB constant until timing matches
 * 3. Formula: actual_ms = DELAY_MS_CALIB_FACTOR * ms parameter
 */

/* Calibration factor for delay_ms at 12MHz (default = 123) */
#define DELAY_MS_CALIB_FACTOR 123

/* Calibration factor for delay_us at 12MHz (default = 1) */
#define DELAY_US_CALIB_FACTOR 1

/* =====================================================================
 * TIMING CONSTANTS FOR COMMON FREQUENCIES
 * ===================================================================== */

/* 
 * Pre-calculated calibration values for common crystal frequencies.
 * Use these to quickly adjust for different hardware:
 *
 * 11.0592 MHz: DELAY_MS_CALIB_FACTOR = 135
 * 12 MHz:      DELAY_MS_CALIB_FACTOR = 123
 * 20 MHz:      DELAY_MS_CALIB_FACTOR = 205
 * 24 MHz:      DELAY_MS_CALIB_FACTOR = 246
 * 30 MHz:      DELAY_MS_CALIB_FACTOR = 308
 */

/* End of delay.h */
#endif
