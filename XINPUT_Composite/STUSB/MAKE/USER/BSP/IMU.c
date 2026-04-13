#include "IMU.h"
#include "math.h"

void Quaternion_Updata(Quaternion_struct *Q,float* W_B)
{
    Quaternion_struct Q_BUF = *Q;
    float a = 0.5*Quaternion_T;
    Q->Q_R = -a*(Q_BUF.Q_IM[0]*W_B[0]+Q_BUF.Q_IM[1]*W_B[1]+Q_BUF.Q_IM[2]*W_B[2])+Q_BUF.Q_R;
    Q->Q_IM[0] = a*(Q_BUF.Q_R*W_B[0]+Q_BUF.Q_IM[2]*W_B[1]-Q_BUF.Q_IM[1]*W_B[2])+Q_BUF.Q_IM[0];
    Q->Q_IM[1] = a*(Q_BUF.Q_R*W_B[1]+Q_BUF.Q_IM[0]*W_B[2]-Q_BUF.Q_IM[2]*W_B[0])+Q_BUF.Q_IM[1];
    Q->Q_IM[2] = a*(Q_BUF.Q_R*W_B[2]+Q_BUF.Q_IM[1]*W_B[0]-Q_BUF.Q_IM[0]*W_B[1])+Q_BUF.Q_IM[2];

    //transfer unit Quaternion
    Q_BUF.Q_R = Quaternion_Square(Q->Q_R)+Quaternion_Square(Q->Q_IM[0])+Quaternion_Square(Q->Q_IM[1])+Quaternion_Square(Q->Q_IM[2]);
    Q_BUF.Q_R = sqrtf(Q_BUF.Q_R);
    Q->Q_R = Q->Q_R/Q_BUF.Q_R;
    Q->Q_IM[0] = Q->Q_IM[0]/Q_BUF.Q_R;
    Q->Q_IM[1] = Q->Q_IM[1]/Q_BUF.Q_R;
    Q->Q_IM[2] = Q->Q_IM[2]/Q_BUF.Q_R;
}

void Quaternion_Get_Euler_angles(Quaternion_struct *Q,float* Euler_angles)
{
    //pitch 
    Euler_angles[0] = asinf(2*(Q->Q_R*Q->Q_IM[1]-Q->Q_IM[0]*Q->Q_IM[2]));
    //roll
    Euler_angles[1] = atan2f(2*(Q->Q_R*Q->Q_IM[0]+Q->Q_IM[1]*Q->Q_IM[2]),1-2*(Q->Q_IM[0]*Q->Q_IM[0]+Q->Q_IM[1]*Q->Q_IM[1]));
    //yaw
    Euler_angles[2] = atan2f(2*(Q->Q_R*Q->Q_IM[2]+Q->Q_IM[0]*Q->Q_IM[1]),1-2*(Q->Q_IM[1]*Q->Q_IM[1]+Q->Q_IM[2]*Q->Q_IM[2]));
}