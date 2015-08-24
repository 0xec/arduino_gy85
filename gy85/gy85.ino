/*
 Name:      gy85.ino
 Created:   2015/8/24 14:33:08
 Author:    xec
*/
#include <Wire.h>

#define ADXL345 0x53 // Device address as specified in data sheet //ADXL345 accelerometer
#define DATAX0 0x32       //X-Axis Data 0

#define HMC5883 0x1E //0011110b, I2C 7bit address of HMC5883
#define ITG3200 0x68


float g_offx = 0;
float g_offy = 0;
float g_offz = 0;


void ReadGyro(float* fgx, float* fgy, float* fgz);

void SetAccelerometor()
{
    Wire.beginTransmission(ADXL345);     // start transmission to device
    Wire.write(0x31);                 // send register address
    Wire.write(0x01);                 // send value to write
    Wire.endTransmission();             // end transmission

    //Put the ADXL345 into Measurement Mode by writing 0x08 to the POWER_CTL register.
    Wire.beginTransmission(ADXL345);     // start transmission to device
    Wire.write(0x2D);                 // send register address  //Power Control Register
    Wire.write(0x08);                 // send value to write
    Wire.endTransmission();             // end transmission
}

void SetCompass()
{
    //Put the HMC5883 IC into the correct operating mode
    Wire.beginTransmission(HMC5883);//open communication with HMC5883
    Wire.write(0x02);               //select mode register
    Wire.write(0x00);               //continuous measurement mode
    Wire.endTransmission();
}

void SetGyro()
{
    Wire.beginTransmission(ITG3200);
    Wire.write(0x3E);
    Wire.write(0x00);
    Wire.endTransmission();

    Wire.beginTransmission(ITG3200);
    Wire.write(0x15);
    Wire.write(0x07);
    Wire.endTransmission();

    Wire.beginTransmission(ITG3200);
    Wire.write(0x16);
    Wire.write(0x1E);   // +/- 2000 dgrs/sec, 1KHz, 1E, 19
    Wire.endTransmission();

    Wire.beginTransmission(ITG3200);
    Wire.write(0x17);
    Wire.write(0x00);
    Wire.endTransmission();

    GyroCalibrate();
}

void GyroCalibrate()
{
    float tmpx = 0;
    float tmpy = 0;
    float tmpz = 0;

    g_offx = 0;
    g_offy = 0;
    g_offz = 0;

    for (uint8_t i = 0; i < 10; i++) { //take the mean from 10 gyro probes and divide it from the current probe
        delay(10);
        float gpx, gpy, gpz;
        ReadGyro(&gpx, &gpy, &gpz);
        tmpx += gpx;
        tmpy += gpy;
        tmpz += gpz;
    }
    g_offx = tmpx / 10.0f;
    g_offy = tmpy / 10.0f;
    g_offz = tmpz / 10.0f;
}
void setup()
{
    Serial.begin(9600);
    Wire.begin();

    SetGyro();


}

void ReadAccelerometor()
{
    Wire.beginTransmission(ADXL345);      // start transmission to device
    Wire.write(DATAX0);               // sends address to read from
    Wire.endTransmission();              // end transmission\

    //  Wire.beginTransmission(ADXL345);      // start transmission to device
    Wire.requestFrom(ADXL345, 6, true);         // request 6 bytes from device

    //int axh = Wire.read();
    //int axl = Wire.read();

    //Serial.print(axh, HEX); Serial.print(",");
    //Serial.print(axl, HEX);
    int16_t ax = Wire.read() | Wire.read() << 8;
    int16_t ay = Wire.read() | Wire.read() << 8;
    int16_t az = Wire.read() | Wire.read() << 8;


    Serial.print(ax); Serial.print(",");
    Serial.print(ay); Serial.print(",");
    Serial.print(az); Serial.print(",");
}

void ReadCompass()
{
    Wire.beginTransmission(HMC5883);
    Wire.write(0x03);               //select register 3, X MSB register
    Wire.endTransmission();


    Wire.requestFrom(HMC5883, 6);


    int16_t cx = Wire.read() << 8 | Wire.read();
    int16_t cz = Wire.read() << 8 | Wire.read();
    int16_t cy = Wire.read() << 8 | Wire.read();

    Serial.print(cx); Serial.print(",");
    Serial.print(cy); Serial.print(",");
    Serial.print(cz); Serial.print(",");


}


void ReadGyro(float* fgx, float* fgy, float* fgz)
{
    Wire.beginTransmission(ITG3200);
    Wire.write(0x1B);
    Wire.endTransmission();

    Wire.requestFrom(ITG3200, 8);    // request 8 bytes from ITG3200

    int16_t temp = Wire.read() << 8 | Wire.read();
    int16_t gx = Wire.read() << 8 | Wire.read();
    int16_t gy = Wire.read() << 8 | Wire.read();
    int16_t gz = Wire.read() << 8 | Wire.read();

    //Serial.print(gx); Serial.print(",");
    //Serial.print(gy); Serial.print(",");
    //Serial.print(gz); Serial.print(",");
    *fgx = gx - g_offx;
    *fgy = gy - g_offy;
    *fgz = gz - g_offz;

}


// the loop function runs over and over again until power down or reset
void loop()
{
    float gx, gy, gz;

    ReadGyro(&gx, &gy, &gz);
    Serial.print(gx); Serial.print(",");
    Serial.print(gy); Serial.print(",");
    Serial.print(gz); Serial.print(",");

    Serial.println();
    delay(10);
}
