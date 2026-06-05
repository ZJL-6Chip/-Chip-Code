/*
 * =====================================================================
 * File: led_driver.c
 * Project: Low-Power Running Lamp Driver for 51 Singlechip
 * Description: LED driver implementation with bit manipulation
 * Author: ZJL-6Chip
 * Date: 2026-06-05
 * =====================================================================
 *
 * PURPOSE:
 *   Implement low-level LED control functions. This file contains
 *   the actual hardware interface code using efficient bit manipulation
 *   and timing control optimized for 8051 performance.
 *
 * IMPLEMENTATION NOTES:
 *   - Uses bit-shift operations for minimal code size
 *   - Direct port I/O for speed
 *   - No RAM overhead for shadow registers
 *   - Interrupt-safe if called from main context only
 *
 * COMPILE OPTIONS:
 *   - Optimize for speed (-O2) recommended
 *   - Disable interrupts during critical operations if needed
 *
 * =====================================================================
 */

#include "led_driver.h"

/* =====================================================================
 * PORT DEFINITIONS (Hardware Specific)
 * ===================================================================== */

/*
 * 8051 Port Register Declarations:
 * These declare the actual hardware port pins to the C compiler.
 * The compiler knows these are memory-mapped I/O.
 */

/* SFR (Special Function Register) Declarations */
#if LED_PORT_SELECTION == 0
    sfr P0 = 0x80;  /* Port 0 register address */
#elif LED_PORT_SELECTION == 1
    sfr P1 = 0x90;  /* Port 1 register address */
#elif LED_PORT_SELECTION == 2
    sfr P2 = 0xA0;  /* Port 2 register address */
#else
    sfr P3 = 0xB0;  /* Port 3 register address */
#endif

/* =====================================================================
 * DRIVER STATE VARIABLES
 * ===================================================================== */

/*
 * Shadow Port Register:
 * Maintains a copy of port state in RAM for efficiency.
 * Avoids repeated port I/O reads which may be slow.
 * Optional optimization - can be disabled to save RAM.
 */
static UCHAR led_port_shadow = LED_INIT;

/* =====================================================================
 * FUNCTION IMPLEMENTATIONS
 * ===================================================================== */

/*
 * FUNCTION: led_driver_init
 * 
 * Initialize LED driver - set port to output, all LEDs off.
 */
void led_driver_init(void)
{
    /* Initialize port shadow register */
    led_port_shadow = LED_INIT;  /* All LEDs off (all bits = 1 for active-low) */
    
    /* Configure port direction (all pins as outputs) */
    /* Note: Most 8051 ports default to input/open-drain.
       For Port 1, pins default to input but can be driven as output.
       No explicit direction register needed for simple GPIO. */
    
    /* Write initial state to hardware port */
    LED_PORT = LED_INIT;
    
    /* Brief delay to ensure port stabilization */
    delay_ms(1);
}

/*
 * FUNCTION: led_set
 * 
 * Set individual LED on or off.
 * Uses bit manipulation for efficiency.
 */
void led_set(UCHAR led_index, UCHAR state)
{
    UCHAR bit_mask;
    
    /* Validate LED index */
    if (led_index >= LED_COUNT)
        return;  /* Out of range, do nothing */
    
    /* Calculate bit mask for this LED */
    /* Example: LED3 → bit_mask = 0x08 (1 << 3) */
    bit_mask = 1 << led_index;
    
    /* Update shadow register based on state */
    if (state == LED_ON)
    {
        /* Set bit to 0 for active-low logic (LED ON) */
        led_port_shadow &= ~bit_mask;  /* Clear bit */
    }
    else
    {
        /* Set bit to 1 for active-low logic (LED OFF) */
        led_port_shadow |= bit_mask;   /* Set bit */
    }
    
    /* Write updated state to hardware port */
    LED_PORT = led_port_shadow;
}

/*
 * FUNCTION: led_set_port
 * 
 * Set all 8 LEDs at once from a byte value.
 */
void led_set_port(UCHAR byte_value)
{
    /* Update shadow register */
    led_port_shadow = byte_value;
    
    /* Write to hardware port */
    LED_PORT = byte_value;
}

/*
 * FUNCTION: led_get_port
 * 
 * Read current port state.
 */
UCHAR led_get_port(void)
{
    /* Return shadow copy (more reliable than reading port) */
    return led_port_shadow;
}

/*
 * FUNCTION: led_running_lamp
 * 
 * Main animation: running lamp (chasing light effect).
 * Shows one LED lit, moving left to right.
 */
void led_running_lamp(void)
{
    UCHAR i;
    UCHAR pattern;  /* Current pattern byte */
    
    /* Infinite loop - runs continuously */
    while (1)
    {
        /* Iterate through each LED position */
        for (i = 0; i < LED_COUNT; i++)
        {
            /* Create pattern: only one bit set (LED ON at position i) */
            /* For active-low logic: 0 = ON, 1 = OFF */
            pattern = ~(1 << i);  /* Invert: 00000001 → 11111110 */
            
            /* Display this pattern */
            led_set_port(pattern);
            
            /* Hold this position for configured delay */
            delay_ms(DELAY_MAIN_MS);
        }
        /* After last LED, loop repeats starting from first LED */
    }
}

/*
 * FUNCTION: led_running_lamp_rev
 * 
 * Reverse animation: running lamp from right to left.
 */
void led_running_lamp_rev(void)
{
    UCHAR i;
    UCHAR pattern;  /* Current pattern byte */
    
    /* Infinite loop - runs continuously */
    while (1)
    {
        /* Iterate through LED positions in reverse */
        for (i = LED_COUNT; i > 0; i--)
        {
            /* Create pattern: only one bit set at position (i-1) */
            pattern = ~(1 << (i - 1));
            
            /* Display this pattern */
            led_set_port(pattern);
            
            /* Hold this position for configured delay */
            delay_ms(DELAY_MAIN_MS);
        }
        /* After first LED, loop repeats starting from last LED */
    }
}

/*
 * FUNCTION: led_pulse
 * 
 * Pulse (blink) a single LED specified number of times.
 */
void led_pulse(UCHAR led_index, UCHAR times)
{
    UCHAR i;
    
    /* Validate LED index */
    if (led_index >= LED_COUNT)
        return;  /* Out of range */
    
    /* Repeat pulse cycle 'times' times */
    for (i = 0; i < times; i++)
    {
        /* Turn LED on */
        led_set(led_index, LED_ON);
        delay_ms(DELAY_MAIN_MS);
        
        /* Turn LED off */
        led_set(led_index, LED_OFF);
        delay_ms(DELAY_MAIN_MS);
    }
}

/*
 * FUNCTION: led_all_on
 * 
 * Turn on all LEDs.
 */
void led_all_on(void)
{
    /* All LEDs on: 0x00 (active-low logic) */
    led_set_port(0x00);
}

/*
 * FUNCTION: led_all_off
 * 
 * Turn off all LEDs.
 */
void led_all_off(void)
{
    /* All LEDs off: 0xFF (active-low logic) */
    led_set_port(0xFF);
}

/* End of led_driver.c */
