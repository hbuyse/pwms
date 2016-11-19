/**
 * @file main.c
 * @author hbuyse
 * @date 26/12/2015
 */

#include <wiringPi.h>          // wiringPiSetupGpio, pinMode
#include <stdio.h>          // printf
#include <signal.h>          // signal
#include <stdlib.h>          // exit


#define WHITE 13
#define BLUE 18
#define YELLOW 19


static unsigned char     tab_pins[] = {WHITE, BLUE, YELLOW};


static void sigint_handler(int signal __attribute__( (unused) ) )
{
    unsigned int     i = 0;


    // Turn off all LEDS
    for ( i = 0; i < (sizeof(tab_pins) / sizeof(unsigned char) ); ++i )
    {
        #ifdef __DEBUG__
        printf("[%s] pwmWrite(%d, 0)\n", __FUNCTION__, tab_pins[i]);
        #endif

        pwmWrite(tab_pins[i], 0);
    }

    exit(0);
}



int main(void)
{
    signal(SIGINT, sigint_handler);
    unsigned int        i       = 0;

    int                 bright  = 0;


    /* SETUP FUNCTIONS
     *
     * There are three ways to initialise wiringPi.
     *
     *  * int wiringPiSetup (void) ;
     *  * int wiringPiSetupGpio (void) ;
     *  * int wiringPiSetupSys (void) ;
     *
     * One of the setup functions must be called at the start of your program. If it returns -1 then the initialisation
     * of the GPIO has failed, and you should consult the global errno to see why.
     *
     * The differences between the setup functions are as follows:
     *  * wiringPiSetup(void) ;
     *    This initialises the wiringPi system and assumes that the calling program is going to be using the wiringPi
     *    pin numbering scheme. This is a simplified numbering scheme which provides a mapping from virtual pin numbers
     *    0 through 16 to the real underlying Broadcom GPIO pin numbers. See the pins page for a table which maps the
     *    wiringPi pin number to the Broadcom GPIO pin number to the physical location on the edge connector.
     *
     *    This function needs to be called with root privileges.
     *
     *  * wiringPiSetupGpio(void) ;
     *    This is identical to above, however it allows the calling programs to use the Broadcom GPIO pin numbers
     *    directly with no re-mapping.
     *
     *    As above, this function need to be called with root priveledges
     *
     *  * wiringPiSetupSys(void)
     *    This initialises the wiringPi system but uses the /sys/class/gpio interface rather than accessing the hardware
     *    directly. This can be called as a non-root user provided the GPIO pins have been exported before-hand using
     *    the gpio program. Pin number in this mode is the native Broadcom GPIO numbers.
     *
     *    Note: In this mode you can only use the pins which have been exported via the /sys/class/gpio interface. You
     *    must export these pins before you call your program. You can do this in a separate shell-script, or by using
     *    the system() function from inside your program.
     *
     *    Also note that some functions (noted below) have no effect when using this mode as they’re not currently
     *    possible to action unless called with root privileges.
     */
    wiringPiSetupGpio();


    /* void pinMode (int pin, int mode) ;
     * This sets the mode of a pin to either INPUT, OUTPUT, or PWM_OUTPUT. Note that only wiringPi pin 1 (BCM_GPIO 18)
     * supports PWM output. The pin number is the number obtained from the pins table.
     *
     * This function has no effect when in Sys mode.
     */
    for ( i = 0; i < (sizeof(tab_pins) / sizeof(unsigned char) ); ++i )
    {
        #ifdef __DEBUG__
        printf("[%s] pinMode(%d, PWM_OUTPUT)\n", __FUNCTION__, tab_pins[i]);
        #endif

        pinMode(tab_pins[i], PWM_OUTPUT);
    }

    // for ( i = 0; i < (sizeof(tab_pins) / sizeof(unsigned char) ); ++i )
    // {
    //     #ifdef __DEBUG__
    //     printf("[%s] pwmWrite(%d, 0)\n", __FUNCTION__, tab_pins[i]);
    //     #endif

    //     pwmWrite(tab_pins[i], 0);
    // }


    /*
     * void pwmWrite (int pin, int value) ;
     * Writes the value to the PWM register for the given pin. The value must be between 0 and 1024.
     * (Again, note that only pin 1 (BCM_GPIO 18) supports PWM [RPI1])
     */
    for ( ; ; )
    {
        for ( i = 0; i < (sizeof(tab_pins) / sizeof(unsigned char) ); ++i )
        // for ( i = 1; i < 2; ++i )
        {
            for ( bright = 0; bright < 1024; ++bright )
            {

                #ifdef __DEBUG__
                printf("[%s] pwmWrite(%d, %d)\n", __FUNCTION__, tab_pins[i], bright);
                #endif

                pwmWrite(tab_pins[i], bright);
                delay(1);
            }

            for ( bright = 1023; bright >= 0; --bright )
            {
                #ifdef __DEBUG__
                printf("[%s] pwmWrite(%d, %d)\n", __FUNCTION__, tab_pins[i], bright);
                #endif

                pwmWrite(tab_pins[i], bright);
                delay(1);
            }
        }
    }

    return (0);
}