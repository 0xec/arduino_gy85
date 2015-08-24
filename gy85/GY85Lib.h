// GY85Lib.h

#ifndef _GY85LIB_h
#define _GY85LIB_h

#if defined(ARDUINO) && ARDUINO >= 100
#include "arduino.h"
#else
#include "WProgram.h"
#endif

namespace XecSensor
{
    class GY85
    {
      public:
        GY85();
        virtual ~GY85();

        void initialize();
        void reset_all_to_zero();

        void ReadAccelerometor(int* ax, int* ay, int* az);
        void ReadCompass(int* cx, int* cy, int* cz);
        void ReadGyro(int* gx, int* gy, int* gz);

      protected:

        void SetupAccelerometor();
        void SetupCompass();
        void SetupGyro();

        void GyroCalibrate();
        void AccCalibrate();
        void CompassCalibrate();

      private:
        int m_gyro_fix_x, m_gyro_fix_y, m_gyro_fix_z;
        int m_acc_fix_x, m_acc_fix_y, m_acc_fix_z;
        int m_compass_fix_x, m_compass_fix_y, m_compass_fix_z;
    };
}

#endif

