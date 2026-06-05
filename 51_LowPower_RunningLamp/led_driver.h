/*
 * =====================================================================
 * File: led_driver.h
 * Project: Low-Power Running Lamp Driver for 51 Singlechip
 * Description: LED driver hardware abstraction layer interface
 * Author: ZJL-6Chip
 * Date: 2026-06-05
 * =====================================================================
 *
 * PURPOSE:
 *   Define the interface for low-level LED control and pattern
 *   generation. This header provides function prototypes and
 *   constants for LED manipulation without exposing hardware details.
 *
 * API FUNCTIONS:
 *   - led_driver_init()      : Initialize LED subsystem
 *   - led_set()              : Control individual LED
 *   - led_set_port()         : Set entire port byte
 *   - led_get_port()         : Read current port state
 *   - led_running_lamp()     : Generate running lamp pattern
 *   - led_running_lamp_rev() : Reverse running lamp pattern
 *   - led_pulse()            : Single LED pulse effect
 *   - led_all_on()           : Turn on all LEDs
 *   - led_all_off()          : Turn off all LEDs
 *
 * DEPENDENCIES:
 *   - config.h : Hardware configuration
 *   - delay.h  : Timing functions
 *
 * COMPILATION:
 *   Designed for Keil µVision with 8051 target support.
 *   Compatible with STC89C52, AT89S52, and similar devices.
 *
 * =====================================================================
 */

#ifndef __LED_DRIVER_H__
#define __LED_DRIVER_H__

#include "config.h"
#include "delay.h"

/* =====================================================================
 * TYPE DEFINITIONS AND CONSTANTS
 * ===================================================================== */

/*
 * LED State Enumeration:
 * Defines possible states for individual LED elements.
 */
typedef enum
{
    LED_STATE_OFF = 0,    /* LED is off */
    LED_STATE_ON = 1,     /* LED is on */
    LED_STATE_BLINK = 2   /* LED is blinking */
} LED_State_t;

/*
 * Pattern Direction Enumeration:
 * Controls direction of running lamp animation.
 */
typedef enum
{
    LED_DIRECTION_FORWARD = 0,   /* Left to right */
    LED_DIRECTION_REVERSE = 1    /* Right to left */
} LED_Direction_t;

/* =====================================================================
 * PUBLIC FUNCTION DECLARATIONS
 * ===================================================================== */

/*
 * FUNCTION: led_driver_init
 * 
 * PURPOSE:
 *   Initialize the LED driver hardware and set initial state.
 *   Must be called once during startup before using LED functions.
 *
 * PARAMETERS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 * EFFECTS:
 *   - Configures LED port direction (output)
 *   - Sets all LEDs to OFF state
 *   - Clears any previous driver state
 *
 * EXAMPLE:
 *   led_driver_init();
 *
 * NOTES:
 *   - This function assumes port is GPIO capable
 *   - Call this once at system startup
 */
void led_driver_init(void);

/*
 * FUNCTION: led_set
 * 
 * PURPOSE:
 *   Set the state of a single LED by position/index.
 *
 * PARAMETERS:
 *   led_index : Position of LED (0-7)
 *   state     : LED_ON or LED_OFF
 *
 * RETURN VALUE:
 *   None
 *
 * EFFECTS:
 *   - Updates the specified LED state
 *   - Other LEDs remain unchanged
 *   - Changes propagate to hardware immediately
 *
 * EXAMPLE:
 *   led_set(0, LED_ON);   // Turn on LED at position 0
 *   led_set(3, LED_OFF);  // Turn off LED at position 3
 *
 * NOTES:
 *   - Index must be 0-7 (enforced by LED_COUNT in config.h)
 *   - Function uses bit manipulation for efficiency
 */
void led_set(UCHAR led_index, UCHAR state);

/*
 * FUNCTION: led_set_port
 * 
 * PURPOSE:
 *   Set all 8 LEDs at once by writing a complete byte.
 *   Useful for pattern-based control.
 *
 * PARAMETERS:
 *   byte_value : 8-bit value where each bit controls one LED
 *                Bit 0 = LED0, Bit 1 = LED1, ..., Bit 7 = LED7
 *                Set bit = LED_ON, Clear bit = LED_OFF
 *
 * RETURN VALUE:
 *   None
 *
 * EFFECTS:
 *   - All 8 LEDs updated simultaneously
 *   - Previous states completely replaced
 *
 * EXAMPLE:
 *   led_set_port(0x55);  // Pattern: LED0,2,4,6 ON; LED1,3,5,7 OFF
 *   led_set_port(0x00);  // All LEDs off
 *   led_set_port(0xFF);  // All LEDs on
 *
 * NOTES:
 *   - Most efficient way to update multiple LEDs
 *   - Consider using with pre-calculated lookup table
 */
void led_set_port(UCHAR byte_value);

/*
 * FUNCTION: led_get_port
 * 
 * PURPOSE:
 *   Read the current state of all LEDs on the port.
 *   Useful for monitoring or debugging.
 *
 * PARAMETERS:
 *   None
 *
 * RETURN VALUE:
 *   UCHAR : Current port byte value
 *           Each bit represents one LED state
 *
 * EFFECTS:
 *   - No hardware changes
 *   - Reads current port state (may show external changes)
 *
 * EXAMPLE:
 *   UCHAR current_state = led_get_port();
 *
 * NOTES:
 *   - Returns actual port register value
 *   - Useful for debugging or state verification
 */
UCHAR led_get_port(void);

/*
 * FUNCTION: led_running_lamp
 * 
 * PURPOSE:
 *   Display a running lamp (chasing light) animation pattern.
 *   Typically shows one LED lit at a time, moving from left to right.
 *   This is the core animation function of the project.
 *
 * PARAMETERS:
 *   None
 *
 * RETURN VALUE:
 *   None (blocking function - runs continuously)
 *
 * BEHAVIOR:
 *   - Displays sequential pattern: LED0 → LED1 → ... → LED7 → repeat
 *   - Each LED illuminated for DELAY_MAIN_MS milliseconds
 *   - All other LEDs off
 *   - Infinite loop (use external interrupt or timeout to break)
 *
 * TIMING:
 *   Total cycle time = LED_COUNT * DELAY_MAIN_MS
 *   Example: 8 LEDs * 150ms = 1.2 seconds per full cycle
 *
 * EXAMPLE:
 *   led_driver_init();  // Initialize first
 *   led_running_lamp(); // Run animation (infinite loop)
 *
 * NOTES:
 *   - This is a blocking function
 *   - Modify DELAY_MAIN_MS in config.h to adjust speed
 *   - Can be interrupted by system timer interrupt
 */
void led_running_lamp(void);

/*
 * FUNCTION: led_running_lamp_rev
 * 
 * PURPOSE:
 *   Display running lamp pattern in reverse direction (right to left).
 *   Mirror operation of led_running_lamp().
 *
 * PARAMETERS:
 *   None
 *
 * RETURN VALUE:
 *   None (blocking function)
 *
 * BEHAVIOR:
 *   - Displays sequential pattern: LED7 → LED6 → ... → LED0 → repeat
 *   - Each LED illuminated for DELAY_MAIN_MS milliseconds
 *   - All other LEDs off
 *
 * EXAMPLE:
 *   led_driver_init();
 *   led_running_lamp_rev();  // Run reverse animation
 *
 * NOTES:
 *   - Same behavior as led_running_lamp() but opposite direction
 */
void led_running_lamp_rev(void);

/*
 * FUNCTION: led_pulse
 * 
 * PURPOSE:
 *   Create a pulse/breathing effect on a single LED.
 *   Good for status indication.
 *
 * PARAMETERS:
 *   led_index : Which LED to pulse (0-7)
 *   times     : Number of pulses to repeat
 *
 * RETURN VALUE:
 *   None
 *
 * BEHAVIOR:
 *   - Turns specified LED on and off repeatedly
 *   - Each pulse: ON (150ms) → OFF (150ms)
 *   - Repeats 'times' times
 *
 * EXAMPLE:
 *   led_pulse(0, 5);  // Pulse LED0 five times
 *
 * NOTES:
 *   - Useful for blinking status indicators
 *   - Frequency: 1/(2*DELAY_MAIN_MS) Hz
 */
void led_pulse(UCHAR led_index, UCHAR times);

/*
 * FUNCTION: led_all_on
 * 
 * PURPOSE:
 *   Turn on all LEDs simultaneously.
 *   Creates a "full brightness" or "ready" indicator.
 *
 * PARAMETERS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 * EFFECTS:
 *   - All LEDs set to ON state
 *   - Remains on until changed by another function
 *
 * EXAMPLE:
 *   led_all_on();   // Illuminate all LEDs
 *
 * NOTES:
 *   - Equivalent to led_set_port(0x00) for active-low logic
 *   - May require external power management if total current is high
 */
void led_all_on(void);

/*
 * FUNCTION: led_all_off
 * 
 * PURPOSE:
 *   Turn off all LEDs simultaneously.
 *   Creates a "powered down" or "idle" indicator.
 *
 * PARAMETERS:
 *   None
 *
 * RETURN VALUE:
 *   None
 *
 * EFFECTS:
 *   - All LEDs set to OFF state
 *   - Remains off until changed by another function
 *
 * EXAMPLE:
 *   led_all_off();  // Turn off all LEDs
 *
 * NOTES:
 *   - Equivalent to led_set_port(0xFF) for active-low logic
 *   - Lowest power consumption state
 */
void led_all_off(void);

/* End of led_driver.h */
#endif
