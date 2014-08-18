/* smooth   v0.2
 Paul Badger 2007
 Smooth is a simple digital low-pass filter that is useful for smoothing sensor jitter or creating a delayed response to 
 fast moving data. It uses a buffer variable and limits the amount of new data that reaches the output each time through
 the loop. Old data is used to make up the difference so that the response of the filter is slowed down.
 Someone with some more math might want to add a little more explanation here. This would be called an integrating function 
 I believe.

 Smoothing can also be used to puposefully slow down (number-based) actions. 


 Details: Smooth takes three parameters, the value to smoothed, the filter value (filterVal), and smoothedVal.

 int  sensVal - the sensor variable - raw material to be smoothed

 float  filterVal - The filter value is a float and must be between 0 and .9999 say. 0 is off (no smoothing) and .9999 is maximum smoothing.
 The actual performance of the filter is going to be dependent on fast you are sampling your sensor (the total loop time), so 
 some trial and error will probably be neccessary to get the desired response.

 smoothedVal    -  Use this for the output of the sensor and also feed it back into the loop. Each sensor needs its own value.
 Don't use this variable for any other purpose.

 This function can easily be rewritten with all-integer math, if you need more speed or want to avoid floats.

 Note that an earlier version of the filter I had posted here had the parameters reversed from current version and also would malfunction if used on more than one sensor.
 */

int sensVal;           // for raw sensor values 
float filterVal;       // this determines smoothness  - .0001 is max  1 is off (no smoothing)
float smoothedVal;     // this holds the last loop value just use a unique variable for every different sensor that needs smoothing

float smoothedVal2;   // this would be the buffer value for another sensor if you needed to smooth two different sensors - not used in this sketch


int i, j;              // loop counters or demo     

void setup()
{
  Serial.begin(9600);
  Serial.println("start ");
}

/* the main loop is a test of the smooth function. It generates a simulated square wave and then 
 switches in four different smoothing values. Watch the numbers scroll by as the filter value
 slows down the response.  */


void loop()
{ 
  for (i = 0; i < 7; i++){    // substitute some different filter values
    filterVal = i * .15; 


    for (j = 0; j< 60; j++){      

      if (j < 30){            // generate a simulated square wave
        sensVal = 1023;
      }
      else
      {
        sensVal = 0; 
      }

      // sensVal = analogRead(0);   this is what one would do normally
      smoothedVal =  smooth(sensVal, filterVal, smoothedVal);   // second parameter determines smoothness  - 0 is off,  .9999 is max smooth 

      Serial.print(sensVal);
      Serial.print("   ");
      Serial.print(smoothedVal, DEC);
      Serial.print("      ");
      Serial.print("filterValue * 100 =  ");   // print doesn't work with floats
      Serial.println(filterVal * 100, DEC);
      delay(30); 
    }
  }
}

int smooth(int data, float filterVal, float smoothedVal){


  if (filterVal > 1){      // check to make sure param's are within range
    filterVal = .99;
  }
  else if (filterVal <= 0){
    filterVal = 0;
  }

  smoothedVal = (data * (1 - filterVal)) + (smoothedVal  *  filterVal);

  return (int)smoothedVal;
}
