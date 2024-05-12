#include <Adafruit_NeoPixel.h>
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>
#include <Wire.h>

//  Which pin on the Arduino is connected to the NeoPixels?
#define PIN        16 // On Trinket or Gemma, suggest changing this to 1
#define BP1        18
#define Buz        20

// How many NeoPixels are attached to the Arduino?
#define NUMPIXELS 30 // Popular NeoPixel ring size
#define DELAYVAL 85// Time (in milliseconds) to pause between pixels


// When setting up the NeoPixel library, we tell it how many pixels,
// and which pin to use to send signals. Note that for older NeoPixel
// strips you might need to change the third parameter -- see the
// strandtest example for more information on possible values.
Adafruit_NeoPixel pixels(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_MPU6050 mpu;


int BP1_prec = LOW;
int etat_BP1;



void setup() {
  pinMode(BP1,INPUT);   //BP
  pinMode(Buz,OUTPUT);  //Buzzer
  digitalWrite(Buz,1);
  delay(500);
  digitalWrite(Buz,0);
  pixels.begin(); // INITIALIZE NeoPixel strip object (REQUIRED)
  Serial.begin(9600);

  Wire.setSDA(8);
  Wire.setSCL(9);
    // Try to initialize!
  while (!mpu.begin()) {
    Serial.println("Failed to find MPU6050 chip");
    delay(100);
  }

    Serial.println("MPU6050 Found!");

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);
  mpu.setFilterBandwidth(MPU6050_BAND_21_HZ);
  delay(100);
}

void loop() {

  /* Get new sensor events with the readings */
  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  //if (g.gyro.x > 4 | g.gyro.y > 4 | g.gyro.z > 4  )
  if (a.acceleration.x > 3 | a.acceleration.y > 3 |a.acceleration.z > 3  )
  
  {
    digitalWrite(Buz,1);
    delay(2000);
    digitalWrite(Buz,0);
  }

  etat_BP1 = digitalRead(BP1);

  if (etat_BP1==HIGH and BP1_prec==LOW){
      Serial.println("Allume");
      for(int i=0; i<NUMPIXELS; i++) { // For each pixel... 
  
       // pixels.Color() takes RGB values, from 0,0,0 up to 255,255,255
      // Here we're using a moderately bright green color:
         if (i<5) {
             pixels.setPixelColor(i, pixels.Color(255, 255, 255));
        }
         if (i<15 and i >4) {
             pixels.setPixelColor(i, pixels.Color(0, 255, 0));
       }
         if (i<25 and i >14) {
             pixels.setPixelColor(i, pixels.Color(255, 0, 0));
       }
         if (i>24) {
            pixels.setPixelColor(i, pixels.Color(255, 255, 255)); 
       }
    }
    pixels.show();   // Send the updated pixel colors to the hardware.
    BP1_prec=HIGH;
    delay(1000);
  }
  etat_BP1 = digitalRead(BP1);
  if (etat_BP1==HIGH and BP1_prec==HIGH){
    Serial.println("Eteind");
    pixels.clear();
    pixels.show();   // Send the updated pixel colors to the hardware.
    BP1_prec=LOW;
    delay(1000);
  }
    

    delay(DELAYVAL); // Pause before next pass through loop
  
}
