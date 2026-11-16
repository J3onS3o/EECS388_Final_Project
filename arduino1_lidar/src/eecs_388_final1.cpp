/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "eecs_388_lib.h"
/** While testing Task 1 alone, set to 1 to print distances to the Serial Monitor.
Set to 0 for the real project: Arduino1's TX pin (pin 1) is wired to Arduino2,
so any text printed here would also be sent to Arduino2 as garbage. **/
#define DEBUG_PRINT (1)
#define SEND_PERIOD_MS (100) /* how often to send a distance to Arduino2 (Task 3, partner) /**


/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition: 
 *          Hardware must be properly connected (sensors, LEDS, etc.)
 *      Post condition: 
 *          Runs initialization calls one time on power up
 *          Input/Output (IO) pins are configured
 *******************************************************************************/
void setup() 
{
uart_init(); /* must be 115200 baud: ubrr = 16 with U2X0 (double speed) set */

return;
}

/******************************************************************************
 *   Function: loop() - Main execution loop
 *      Pre condition: 
 *          setup() has been executed and system is initialized
 *      Post condition: 
 *          Performs a single iteration of the system's function
 *          Repeates indefinetely unless the board is reset or powered off
 *******************************************************************************/
void loop() 
{
uint16_t dist = 0;              /* LIDAR distance data is 16 bits. */
uint8_t dist_L = 0;            /* LIDAR distance data low byte. */
uint8_t dist_H = 0;            /* LIDAR distance data high byte. */

ser_printline("Setup completed.");
ser_write( '\n' );

while( 1 )
    { 
    /* 
        - read a data frame from the TFmini sensor
        
        - from Table 6, we know each LIDAR measurement sends 9 bytes
        - the function `ser_read()` can process 1 byte per function call

        - This if-statement below will read (and check) the first two bytes of a valid data frame
        - You will need to write code to read the subsequent bytes to get the distance data
        - You will need to process the data received from the sensor to get the correct distance value
    */
    
    if ('Y' == ser_read() && 'Y' == ser_read()) 
        {
            dist_L = ser_read(); // byte 3
            dist_H = ser_read(); // byte 4
            dist = dist_L | ((uint16_t) dist_H << 8);

            ser_printf("distance = %d cm", dist);
        /* 

            - print the measured value to the console (i.e. the serial monitor) 
            - You can use ser_printf() or ser_printline() or ser_write()
            
            - Helpful guide for printing with variadic function similiar to printf()
            - https://en.cppreference.com/w/cpp/io/c/fprintf


			- Send the distance to Arduino2
        */
            ser_write(dist_L); // send low byte first
            ser_write(dist_H); // send high byte second
            ser_printline("Distance sent to Arduino2.");
        }
    }

return;
}