#include <SPI.h>
#include <RF24.h>
#include <BTLE.h>
#include <SparkFunLSM6DS3.h>
LSM6DS3 myIMU; //Default constructor is I2C, addr 0x6B
//LSM6DS3Core myIMU( I2C_MODE, 0x6B );
RF24 radio(9,10);

BTLE btle(&radio);
int counter = 0;
int steps = 0;
float prev = 0 , curr = 0;
void setup() {

  Serial.begin(9600);
  while (!Serial) { }
  //Serial.println("BTLE advertisement sender");
  btle.begin("Thomas");
  myIMU.begin();
  if( myIMU.beginCore() != 0 )
  {
    Serial.print("Error at beginCore().\n");
  }
  else
  {
    Serial.print("\nbeginCore() passed.\n");
  }

  //Error accumulation variable
  uint8_t errorAccumulator = 0;

  uint8_t dataToWrite = 0;  //Temporary variable

  //Setup the accelerometer******************************
  dataToWrite = 0; //Start Fresh!
  //  dataToWrite |= LSM6DS3_ACC_GYRO_BW_XL_200Hz;
  dataToWrite |= LSM6DS3_ACC_GYRO_FS_XL_2g;
  dataToWrite |= LSM6DS3_ACC_GYRO_ODR_XL_26Hz;

  // //Now, write the patched together data
  errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_CTRL1_XL, dataToWrite);

  //Set the ODR bit
  errorAccumulator += myIMU.readRegister(&dataToWrite, LSM6DS3_ACC_GYRO_CTRL4_C);
  dataToWrite &= ~((uint8_t)LSM6DS3_ACC_GYRO_BW_SCAL_ODR_ENABLED);

  
  // Enable embedded functions -- ALSO clears the pdeo step count
  errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_CTRL10_C, 0x3E);
  // Enable pedometer algorithm
  errorAccumulator += myIMU.writeRegister(LSM6DS3_ACC_GYRO_TAP_CFG1, 0x40);
  // Step Detector interrupt driven to INT1 pin
  errorAccumulator += myIMU.writeRegister( LSM6DS3_ACC_GYRO_INT1_CTRL, 0x10 );
 
  if( errorAccumulator )
  {
    Serial.println("Problem configuring the device.");
  }
  else
  {
    Serial.println("Device O.K.");
  } 
  delay(200);
}

void loop() {
  float temp = (float)myIMU.readFloatAccelX();
  float temp1 = (float)myIMU.readFloatAccelY();

  float sum = (temp * temp) + (temp1 * temp1);
  
  if(sum > 1.5)
    {
      curr = 2;
    }
  if(sum < 1)
    curr = 0;
  
  if(curr == 2 && prev == 0)
    counter++;
    
  prev = curr;

 // steps += (counter % 3);
  
  
  Serial.println(counter);
  //Serial.println(temp);
  
  nrf_service_data buf;
  buf.service_uuid = NRF_TEMPERATURE_SERVICE_UUID;
  buf.value = BTLE::to_nRF_Float(counter);
  //Serial.println(temp);
  if (!btle.advertise(0x16, &buf, sizeof(buf))) {
    Serial.println("BTLE advertisement failed..!");
  }
  btle.hopChannel();
  //Serial.print(".");
  delay(400);
}
