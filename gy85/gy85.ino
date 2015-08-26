#include "GY85Lib.h"
#include <Wire.h>

XecSensor::GY85 gy85sensor;

void setup()
{
    Serial.begin(9600);
    Wire.begin();

    gy85sensor.initialize();
    //  gy85sensor.reset_all_to_zero();
}


// the loop function runs over and over again until power down or reset
void loop()
{
    int gx, gy, gz;
    int ax, ay, az;
    int cx, cy, cz;

    gy85sensor.ReadAccelerometor(&ax, &ay, &az);
    gy85sensor.ReadGyro(&gx, &gy, &gz);
    gy85sensor.ReadCompass(&cx, &cy, &cz);

    Serial.print(gx); Serial.print(",");
    Serial.print(gy); Serial.print(",");
    Serial.print(gz); Serial.print(",");

    Serial.println();
    delay(10);
}
