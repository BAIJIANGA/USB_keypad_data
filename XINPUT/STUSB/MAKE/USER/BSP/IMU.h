#ifndef __IMU_H__
#define __IMU_H__

#define Quaternion_T 0.005 // updata time 5ms
#define Quaternion_Square(x) ((x)*(x))
typedef struct Quaternion_struct
{
    float Q_R; // Quaternion Real Part
    float Q_IM[3]; // Quaternion Imaginary Part
}Quaternion_struct;


#endif