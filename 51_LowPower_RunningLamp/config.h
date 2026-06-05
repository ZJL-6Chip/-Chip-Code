/*
 * =====================================================================
 * File: config.h
 * Project: Low-Power Running Lamp Driver for 51 Singlechip
 * Description: Central configuration and hardware abstraction macros
 * Author: ZJL-6Chip
 * Date: 2026-06-05
 * =====================================================================
 *
 * PURPOSE:
 *   Define all hardware-specific constants, pin mappings, and timing
 *   parameters. This file serves as the single point of hardware
 *   configuration, making it easy to port code to different hardware.
 *
 * MODIFICATION HISTORY:
 *   - v1.0: Initial version with P1 LED port configuration
 *
 * =====================================================================
 */

#ifndef __CONFIG_H__
#define __CONFIG_H__

/* =====================================================================
 * COMPILER AND STANDARD DEFINITIONS
 * ===================================================================== */

/* Common 8051 Register Definitions */
#define UCHAR unsigned char
#define UINT  unsigned int
#define USHORT unsigned short

/* Boolean Definitions */
#ifndef TRUE
#define TRUE  1
#endif
#ifndef FALSE
#define FALSE 0
#endif

/* =====================================================================
 * HARDWARE CONFIGURATION - LED PORT MAPPING
 * ===================================================================== */

/* 
 * LED Port Selection:
 * The running lamp uses an 8-bit I/O port. Each bit controls one LED.
 * IMPORTANT: This is the main hardware configuration point.
 * 
 * Available options:
 *   - P0: Port 0 (requires external pull-ups)
 *   - P1: Port 1 (built-in pull-ups) - RECOMMENDED
 *   - P2: Port 2 (address/data multiplex in some designs)
 *   - P3: Port 3 (I/O + special functions)
 */
#define LED_PORT_SELECTION 1  /* 0=P0, 1=P1, 2=P2, 3=P3 */

/* 
 * LED Port Actual Assignment:
 * This selects which port the LEDs are connected to.
 * Based on LED_PORT_SELECTION above.
 */
#if LED_PORT_SELECTION == 0
  #define LED_PORT P0
  #define LED_PORT_ADDR 0x80
#elif LED_PORT_SELECTION == 1
  #define LED_PORT P1
  #define LED_PORT_ADDR 0x90
#elif LED_PORT_SELECTION == 2
  #define LED_PORT P2
  #define LED_PORT_ADDR 0xA0
#else
  #define LED_PORT P3
  #define LED_PORT_ADDR 0xB0
#endif

/* =====================================================================
 * LED CONTROL DEFINITIONS
 * ===================================================================== */

/* Number of LEDs in the running lamp array */
#define LED_COUNT 8

/* LED On/Off Logic (adjust based on your LED circuit) */
#define LED_ON    0   /* Active LOW (LED on when pin=0) */
#define LED_OFF   1   /* Active HIGH (LED off when pin=1) */
#define LED_INIT  0xFF /* All LEDs off initially (all bits = 1) */

/* =====================================================================
 * TIMING CONFIGURATION
 * ===================================================================== */

/*
 * Main Loop Delay:
 * Controls the speed of the running lamp pattern.
 * Measured in milliseconds.
 * 
 * Recommended values:
 *   100ms  - Fast movement
 *   150ms  - Normal movement (default)
 *   200ms  - Slow movement
 *   300ms+ - Very slow movement (good for observation)
 */
#define DELAY_MAIN_MS 150

/* 
 * Crystal Frequency:
 * Set this to your MCU's actual crystal frequency.
 * Common values: 12MHz, 11.0592MHz, 24MHz, 30MHz
 * This affects timing calculation precision.
 */
#define CRYSTAL_FREQ_MHZ 12

/* Machine Cycles per Instruction (12 is standard for 8051) */
#define MACHINE_CYCLES_PER_INST 12

/* =====================================================================
 * POWER MANAGEMENT CONFIGURATION
 * ===================================================================== */

/*
 * IDLE Mode Enable:
 * Set to TRUE to enable IDLE power-saving mode between cycles.
 * In IDLE mode, CPU is stopped but peripherals remain active.
 * RAM contents are preserved.
 * 
 * CAUTION: Only use if your application design supports it.
 * May affect timing accuracy.
 */
#define USE_IDLE_MODE FALSE

/*
 * Port Pin Drive Capability:
 * 8051 pins have limited drive current (~5-10mA per pin).
 * For more current, use external driver or transistor.
 */
#define MAX_LED_CURRENT_MA 10  /* Maximum safe current per pin */

/* =====================================================================
 * DEBUG AND DIAGNOSTIC CONFIGURATION
 * ===================================================================== */

/* Debug Mode Enable */
#define DEBUG_MODE FALSE

#if DEBUG_MODE
  /* Serial debug output (if UART available) */
  #define DEBUG_SERIAL FALSE
  /* LED state tracking (uses RAM) */
  #define DEBUG_LED_STATE_TRACKING TRUE
#endif

/* =====================================================================
 * CODE OPTIMIZATION MACROS
 * ===================================================================== */

/*
 * Inline functions for performance-critical code.
 * Reduces function call overhead in tight loops.
 */
#define INLINE_FUNCTIONS TRUE

/*
 * Pre-calculated lookup tables for patterns.
 * Trades RAM for CPU speed (2^LED_COUNT bytes = 256 bytes).
 */
#define USE_PATTERN_LUT FALSE

/* =====================================================================
 * VALIDATION CHECKS
 * ===================================================================== */

/* Compile-time validation of configuration */
#if LED_COUNT > 8
  #error "LED_COUNT cannot exceed 8 (port width)"
#endif

#if DELAY_MAIN_MS < 10
  #warning "DELAY_MAIN_MS < 10ms may cause timing issues"
#endif

/* =====================================================================
 * DERIVED CONFIGURATION VALUES
 * ===================================================================== */

/* Calculate LED mask (only use specified number of LEDs) */
#if LED_COUNT == 8
  #define LED_MASK 0xFF
#elif LED_COUNT == 7
  #define LED_MASK 0x7F
#elif LED_COUNT == 6
  #define LED_MASK 0x3F
#elif LED_COUNT == 5
  #define LED_MASK 0x1F
#elif LED_COUNT == 4
  #define LED_MASK 0x0F
#else
  #error "LED_COUNT must be between 1 and 8"
#endif

/* End of config.h */
#endif
