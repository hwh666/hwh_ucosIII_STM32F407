#ifndef __StepMotor
#define __StepMotor
#include "ST_Include.h"

//===================
#define  M_LC   0
#define  M_LO   1
#define  M_L1   2
#define  M_L2   3
#define  M_L3   4

#define  M_RC   5
#define  M_RO   6
#define  M_R1   7
#define  M_R2   8
#define  M_R3   9
//====================
#define  L_C   0
#define  L_O   1
#define  L_1   2
#define  L_2   3
#define  L_3   4

#define  R_C   5
#define  R_O   6
#define  R_1   7
#define  R_2   8
#define  R_3   9
//====================


//====================
#define  CCW   1       ///ƒÊ ±’Î
#define  CW   0        ///À≥ ±’Î


#define  OPEN   1       
#define  CLOSE  0
//====================


void ST_LeftHandAdjustment(u8 dir,int pulses);
void ST_RightHandAdjustment(u8 dir,int pulses);
void ST_LeftHand_KZ90(void);
void ST_LeftHand_KZ180(void);
void ST_LeftHand_ND90(void);
void ST_LeftHand_ND180(void);
void ST_LeftHand_DD90(void);
void ST_LeftHand_DD180(void);

void ST_RightHand_KZ90(void);
void ST_RightHand_KZ180(void);
void ST_RightHand_ND90(void);
void ST_RightHand_ND180(void);
void ST_RightHand_DD90(void);
void ST_RightHand_DD180(void);

void ST_LeftHand_Move(u8 act);
void ST_RightHand_Move(u8 act);
void Hand_Move(u8 act);


#endif

