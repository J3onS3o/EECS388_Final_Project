/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "eecs_388_lib.h"

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
uart_init();

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
uint8_t dist_l = 0;           /* LIDAR distance data is sent as two bytes, low byte first. */
uint8_t dist_h = 0;          /* LIDAR distance data is sent as two bytes, high byte second. */

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
        /* 

            - print the measured value to the console (i.e. the serial monitor) 
            - You can use ser_printf() or ser_printline() or ser_write()
            
            - Helpful guide for printing with variadic function similiar to printf()
            - https://en.cppreference.com/w/cpp/io/c/fprintf


			- Send the distance to Arduino2
        */
            ser_printline("reading in ser_read() data frame from TFmini sensor...");
            ser_write( '\n' ); 
            // Read the next 7 bytes of the data frame (we only need the distance data, which is 2 bytes)
            dist_l = ser_read(); // Read the low byte of the distance data
            dist_h = ser_read(); // Read the high byte of the distance data
            dist = (dist_h << 8) | dist_l; // Combine the two bytes into a 16-bit value
            ser_printf("Distance is: %d cm\n", dist); // Print the distance to the console

            // write the distance to Arduino2
            ser_write(dist_l); // Send the low byte of the distance data
            ser_write(dist_h); // Send the high byte of the distance data
            // @NOTE: might have to recalculate distance in arduino 2 since it's a 16 byte value...? 
 
        }
    }

return;
}