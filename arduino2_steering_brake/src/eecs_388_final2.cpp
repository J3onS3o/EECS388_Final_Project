/******************************************************************************
 *   Header Includes
 *******************************************************************************/
#include <Arduino.h>
#include <stdint.h>
#include <stdio.h>

#include "eecs_388_lib.h"

/******************************************************************************
 *   Constant definitions
 *******************************************************************************/

/*ServoMotor constants*/
#define SERVO_PULSE_MAX (2400)    /* 2400 us */
#define SERVO_PULSE_MIN (544)     /* 544 us */
#define SERVO_PERIOD    (20000)   /* 20000 us (20ms) */
#define SERVO_PULSE_RANGE (SERVO_PULSE_MAX - SERVO_PULSE_MIN) 
#define MIN_ANGLE       (0)       /* degrees */
#define MAX_ANGLE       (180)     /* degrees */


/******************************************************************************
 *   Local function prototypes
 *******************************************************************************/
static void auto_brake();
static void steering(int pos);

/******************************************************************************
 *   Function: auto_brake() - Auto Brake
 *      Pre condition: 
 *          None
 *      Post condition: 
 *          Checks the LiDAR distance and configures LEDs
 *******************************************************************************/
static void auto_brake()
{
    // Task-2: 
    // Your code goes here (Use Lab 2 & 4 for reference)
    // Check the project document to understand the task

    /*Task 2: LED Breaking Indicator – auto_brake()
    For the purposes of this project, we define any object that is further than 200 cm as a safe
    distance away and no braking is needed. If any object is less than 200 cm but more than 100 cm
    away, we assume that it is getting close enough to warrant braking, but only lightly. For
    anything less than 100 cm away, we assume that the object is getting too close to the vehicle,
    and the brakes need to be applied in full force. Finally, for something that is less than 60 cm
    away, we assume it is too close to us to keep the vehicle in motion and so it needs to stop and
    be stationary. To simulate braking, you will change the color of the LED to indicate the level of
    braking. The levels of braking, and their representation using LED colors, are detailed below: 
    
    > 200cm - safe distance, no breaking - turn ON green LED
    100cm < d <= 200cm - close, break lightly - turn ON Yellow LED (Red + Green)
    60cm < d <= 100cm - very close, break hard - turn ON Red LED
    d <= 60cm - too close, stop - turn ON flashing Red LED (Flashing period = 100 ms))
    */

    // Get distance from LiDAR sensor 
    uint16_t dist = 0;              /* LIDAR distance data is 16 bits. */
    uint8_t dist_l = 0;           /* LIDAR distance data is sent as two bytes, low byte first. */
    uint8_t dist_h = 0;          /* LIDAR distance data is sent as two bytes, high byte second. */

    ser_printline("Setup completed.");
    ser_write( '\n' );

    while( 1 ) // polling the whole time..might need to change
        { 
 
          // Get the distance data from the TFmini sensor and store it in the variable `dist`
          ser_printline("reading in ser_read() data frame from TFmini sensor...");
          ser_write( '\n' ); 
          // Read the next 7 bytes of the data frame (we only need the distance data, which is 2 bytes)
          dist_l = ser_read(); // Read the low byte of the distance data
          dist_h = ser_read(); // Read the high byte of the distance data
          dist = (dist_h << 8) | dist_l; // Combine the two bytes into a 16-bit value

          ser_printf("Distance: %d cm", dist);
            

      if (dist > 200) 
          {
          // Safe distance, no braking - turn ON green LED
          gpio_write(GPIO_13, OFF); // RED OFF
          gpio_write(GPIO_12, ON);  // GREEN ON
          gpio_write(GPIO_11, OFF); // BLUE OFF
          } 
      else if (dist > 100 && dist <= 200) 
          {
          // Close, break lightly - turn ON Yellow LED (Red + Green)
          gpio_write(GPIO_13, ON);  // RED ON
          gpio_write(GPIO_12, ON);  // GREEN ON
          gpio_write(GPIO_11, OFF); // BLUE OFF
          } 
      else if (dist > 60 && dist <= 100) 
          {
          // Very close, break hard - turn ON Red LED
          gpio_write(GPIO_13, ON);  // RED ON
          gpio_write(GPIO_12, OFF); // GREEN OFF
          gpio_write(GPIO_11, OFF); // BLUE OFF
          } 
      else if (dist <= 60) 
          {
          // Too close, stop - turn ON flashing Red LED (Flashing period = 100 ms)
          gpio_write(GPIO_13, ON);  // RED ON
          gpio_write(GPIO_12, OFF); // GREEN OFF
          gpio_write(GPIO_11, OFF); // BLUE OFF
          delay_ms(100);
          gpio_write(GPIO_13, OFF); // RED OFF
          delay_ms(100);
          }
      }
}


/******************************************************************************
 *   Function: steering() - Steering
 *      Pre condition: 
 *          None
 *      Post condition: 
 *          Control the servomotor with GPIO_6
 *******************************************************************************/
static void steering(int pos)
{
    // Task-4: 
    // Your code goes here (Use Lab 05 for reference)
    /*Task 4: Steering with Servomotor
    This task will involve getting the servomotor setup to turn to a specific angle value. To test the
    function and confirm the servomotor is working, try to just turn it between 0 and 180 degrees. You
    can leverage your code from Lab05 for this, but remember that the main loop has other things it
    needs to accomplish, so using delay() functions may affect other timings.
    You will need to setup an array of angle values in your Arduino code. You will loop through these
    values and use them as angles to drive the servomotor. Setup the following angles to loop through.
    Angle_values = [10, 25, 75, 45, 100, 40, 125, 15, 150, 50, 170]. These values will not be setup
    for you in the startup code, you will have to implement this so that it rotates through all the steering
    angles in this list and repeats back at the beginning */

    /*
    if (angle == 0) {
        gpio_write(gpio, 1);
        delay_us(SERVO_PULSE_MIN);
        gpio_write(gpio, 0); // turn off
        delay_us(SERVO_PERIOD - SERVO_PULSE_MIN);
    } else if (angle == 30) {
        uint32_t pulse_width = SERVO_PULSE_MIN + (SERVO_PULSE_RANGE / 6) * 1;
        gpio_write(gpio, 1); // turn on
        delay_us(pulse_width);
        gpio_write(gpio, 0); // turn off
        delay_us(SERVO_PERIOD - pulse_width);
    } else if (angle == 60) {
        uint32_t pulse_width = SERVO_PULSE_MIN + (SERVO_PULSE_RANGE / 6) * 2;
        gpio_write(gpio, 1); // turn on
        delay_us(pulse_width);
        gpio_write(gpio, 0); // turn off
        delay_us(SERVO_PERIOD - pulse_width);
    } else if (angle == 90) {
        uint32_t pulse_width = SERVO_PULSE_MIN + (SERVO_PULSE_RANGE / 6) * 3;
        gpio_write(gpio, 1); 
        delay_us(pulse_width);
        gpio_write(gpio, 0);
        delay_us(SERVO_PERIOD - pulse_width);
    } else if (angle == 120) {
        uint32_t pulse_width = SERVO_PULSE_MIN + (SERVO_PULSE_RANGE / 6) * 4;
        gpio_write(gpio, 1);
        delay_us(pulse_width);
        gpio_write(gpio, 0);
        delay_us(SERVO_PERIOD - pulse_width);
    } else if (angle == 150) {
        uint32_t pulse_width = SERVO_PULSE_MIN + (SERVO_PULSE_RANGE / 6) * 5;
        gpio_write(gpio, 1);
        delay_us(pulse_width);
        gpio_write(gpio, 0);
        delay_us((SERVO_PERIOD - pulse_width));
    } else if (angle == 180) {
        uint32_t pulse_width = SERVO_PULSE_MAX;
        gpio_write(gpio, 1);
        delay_us(pulse_width);
        gpio_write(gpio, 0);
        delay_us((SERVO_PERIOD - pulse_width));
    }
    */

    // help me figure out how to do this task:

    // i think i should 1) get the position, 
    // then make the pulse width move in increments of 5 degrees, 
    // turn on the gpio, delay for the pulse width, turn off the gpio, 
    // then delay for the rest of the period. then repeat until it reaches the desired position.

    // initialize the desired position 
    int angle = pos; // desired position
    int current_position = 0; // current position of the servo

    // Loop until the desired position is reached
    while (current_position != angle) {
        if (current_position < angle) {
            current_position += 5; // Increment position by 5 degrees
            if (current_position > angle) current_position = angle; // Ensure we don't exceed the desired angle
        } else {
            current_position -= 5; // Decrement position by 5 degrees
            if (current_position < angle) current_position = angle; // Ensure we don't go below the desired angle
        }

        uint32_t pulse_width = SERVO_PULSE_MIN + (SERVO_PULSE_RANGE * current_position) / MAX_ANGLE;
        gpio_write(GPIO_6, 1); // Turn on GPIO for servo control
        delay_us(pulse_width); // Delay for the pulse width
        gpio_write(GPIO_6, 0); // Turn off GPIO
        delay_us(SERVO_PERIOD - pulse_width); // Delay for the rest of the period
    }



}

/******************************************************************************
 *   Function: setup() - Initializes the Arduino System
 *      Pre condition: 
 *          Hardware must be properly connected (BMP180 sensors, etc.)
 *      Post condition: 
 *          Runs initialization calls one time on power up
 *          UART is initialized for ser_printf()
 *          I2C is initialized for communication with BMP180
 *          calib_data is filled with calibration data from BMP180
 *******************************************************************************/
void setup() 
{
uart_init();     // Initialize UART for serial output

//Setup Auto-break LEDs for Distance
gpio_mode(GPIO_13, GPIO_OUTPUT); //RED
gpio_mode(GPIO_12, GPIO_OUTPUT); //GREEN
gpio_mode(GPIO_11, GPIO_OUTPUT); //BLUE

//Setup GPIO_6 for PWM output
gpio_mode(GPIO_6, GPIO_OUTPUT);

ser_printf("System Initialized");
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
// Task-4: 
// Setup simulated code for the angles from the lab sheet
int angle;

// Task-1&2:
// Receive data from arudino1 for the distance and setup the LEDs
auto_brake();

//  Task-4:
for (int i = 0; i < 10; i++)
  {
  // Here, we set the angle to 180 if the prediction from the DNN is a positive angle
  // and 0 if the prediction is a negative angle.
  // This is so that it is easier to see the movement of the servo.
  // You are welcome to pass the angle values directly to the steering function.
  // If the servo function is written correctly, it should still work,
  // only the movements of the servo will be more subtle
  if( angle > 0 )
    {
    steering(180);
    }
  else 
    {
    steering(0);
    }
           
  // Uncomment the line below to see the actual angles on the servo.
  // Remember to comment out the if-else statement above!
  // steering(angle);
  }
}
