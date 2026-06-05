/*
 * =====================================================================
 * File: main.c
 * Project: Low-Power Running Lamp Driver for 51 Singlechip
 * Description: Main application entry point and demonstration
 * Author: ZJL-6Chip
 * Date: 2026-06-05
 * =====================================================================
 *
 * PURPOSE:
 *   Main program entry point. Demonstrates the LED running lamp
 *   driver with various animation patterns and transitions.
 *
 * OPERATION:
 *   1. Initialize hardware (LED driver, timing)
 *   2. Display startup pattern (LED pulse)
 *   3. Run main animation loop (running lamp forward/reverse)
 *
 * POWER CONSUMPTION:
 *   - Idle (all LEDs off):        ~5mA
 *   - Running animation (active): ~30-50mA depending on LED count
 *   - All LEDs on:                ~50-80mA
 *
 * COMPILE & LINK:
 *   Keil µVision IDE:
 *   1. New project → Device: STC89C52 (or compatible)
 *   2. Add files: config.h, delay.h, led_driver.h, led_driver.c, main.c
 *   3. Project → Options for Target → Target → Crystal: 12.0MHz
 *   4. Project → Build → Compile
 *   5. Flash to MCU via programmer
 *
 * =====================================================================
 */

#include "config.h"
#include "delay.h"
#include "led_driver.h"

/* =====================================================================
 * MAIN FUNCTION
 * ===================================================================== */

/*
 * FUNCTION: main
 * 
 * PURPOSE:
 *   Application entry point. Initializes hardware and runs main loop.
 *
 * FLOW:
 *   1. Initialize LED driver
 *   2. Display startup sequence (visual confirmation)
 *   3. Enter main animation loop
 *
 * RETURN VALUE:
 *   Never returns (infinite loop)
 *
 * NOTES:
 *   - This is the entry point when MCU boots
 *   - Execution begins here after reset
 *   - Does not return in normal operation
 */
void main(void)
{
    /* ================================================================
     * INITIALIZATION PHASE
     * ================================================================ */
    
    /*
     * Initialize LED Driver:
     * Configures port direction, sets initial states, clears any noise.
     * Must be called once during startup before using any LED functions.
     */
    led_driver_init();
    
    /* Brief stabilization delay */
    delay_ms(100);
    
    /* ================================================================
     * STARTUP SEQUENCE
     * ================================================================ */
    
    /*
     * Startup Pattern: Pulse LED 0 five times
     * This visual confirmation indicates:
     * - Hardware is functioning
     * - LED driver is initialized
     * - Ready to start main animation
     */
    led_pulse(0, 5);
    
    /* Inter-pattern delay for visual separation */
    delay_ms(200);
    
    /* ================================================================
     * MAIN APPLICATION LOOP
     * ================================================================ */
    
    /*
     * ANIMATION SEQUENCE:
     * The main loop demonstrates different LED animation patterns.
     * Each pattern runs continuously until modified or interrupted.
     *
     * PATTERN 1: Running Lamp Forward
     * Displays: LED0 → LED1 → LED2 → ... → LED7 → repeat
     * This is a left-to-right chasing light effect.
     * Speed controlled by DELAY_MAIN_MS in config.h
     */
    
    while (1)  /* Main infinite loop */
    {
        /* ============================================================
         * PATTERN 1: RUNNING LAMP FORWARD
         * ============================================================ */
        
        /*
         * Display running lamp animation for 5 complete cycles.
         * This function is blocking - it will not return until complete.
         * One cycle = 8 LEDs * DELAY_MAIN_MS = ~1.2 seconds (default)
         */
        
        UCHAR cycle;
        for (cycle = 0; cycle < 5; cycle++)
        {
            UCHAR i;
            
            /* One complete forward cycle */
            for (i = 0; i < LED_COUNT; i++)
            {
                /* Create pattern with only one LED on */
                UCHAR pattern = ~(1 << i);  /* Invert for active-low */
                led_set_port(pattern);
                
                /* Display this step */
                delay_ms(DELAY_MAIN_MS);
            }
        }
        
        /* ============================================================
         * PATTERN 2: RUNNING LAMP REVERSE
         * ============================================================ */
        
        /*
         * Display running lamp animation in reverse (right to left).
         * Same as forward but positions iterate in opposite order.
         * Creates a "bouncing" effect when paired with forward pattern.
         */
        
        for (cycle = 0; cycle < 5; cycle++)
        {
            UCHAR i;
            
            /* One complete reverse cycle */
            for (i = LED_COUNT; i > 0; i--)
            {
                /* Create pattern with only one LED on at position (i-1) */
                UCHAR pattern = ~(1 << (i - 1));
                led_set_port(pattern);
                
                /* Display this step */
                delay_ms(DELAY_MAIN_MS);
            }
        }
        
        /* ============================================================
         * PATTERN 3: ALL ON / ALL OFF BLINK
         * ============================================================ */
        
        /*
         * Alternative pattern: All LEDs on, then all off, repeated.
         * Demonstrates different lighting effect.
         * Useful for status indication (e.g., system heartbeat).
         */
        
        UCHAR blink;
        for (blink = 0; blink < 4; blink++)
        {
            led_all_on();              /* All 8 LEDs on */
            delay_ms(DELAY_MAIN_MS);
            
            led_all_off();             /* All 8 LEDs off */
            delay_ms(DELAY_MAIN_MS);
        }
        
        /* ============================================================
         * PAUSE AND REPEAT
         * ============================================================ */
        
        /*
         * Pause before repeating the entire sequence.
         * This gives a visual "breathing" to the animation,
         * preventing constant visual stimulation.
         */
        delay_ms(500);
        
        /* Loop returns to PATTERN 1 and repeats */
    }
    
    /* This point is never reached in normal operation */
}

/* =====================================================================
 * OPTIONAL: TIMER INTERRUPT HANDLER
 * ===================================================================== */

/*
 * ADVANCED FEATURE (Currently Disabled):
 * 
 * For more sophisticated animation control, you can use Timer 0
 * interrupt to update LED patterns at fixed intervals without
 * blocking the main program.
 *
 * Uncomment and implement the following to use timer-based animation:
 */

/*
// Timer 0 Interrupt Service Routine
// Triggers every DELAY_MAIN_MS milliseconds
void Timer0_ISR(void) interrupt 1  // Interrupt vector 1 is Timer 0
{
    // Reload timer for next interval
    // (Calculation depends on crystal frequency and target interval)
    
    // Update LED pattern
    // (Can implement state machine here for non-blocking animation)
}

// Initialize Timer 0
void Timer0_Init(void)
{
    TMOD |= 0x01;        // Timer 0, mode 1 (16-bit)
    TL0 = 0x00;          // Load initial value (low byte)
    TH0 = 0x00;          // Load initial value (high byte)
    ET0 = 1;             // Enable Timer 0 interrupt
    TR0 = 1;             // Start Timer 0
    EA = 1;              // Enable all interrupts
}
*/

/* End of main.c */
