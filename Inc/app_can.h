/**
  ******************************************************************************
  * @file    CAN/CAN_Networking/Inc/main.h
  * @author  MCD Application Team
  * @brief   Header for main.c module
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2017 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __APP_CAN_H
#define __APP_CAN_H

/* Includes ------------------------------------------------------------------*/
#include <stdint.h>
#include <main.h>
/* Define ------------------------------------------------------------------*/
#define PROTNO	0x060
#define PTP			0x01
#define CNT			0x00
#define RES1		0x01
#define RES2		0x01
#define DSTADDR	0x01
#define SRCADDR 0xF0

#define FIXED_HEAD	0x6080003
#define ADDRESS_UPPER	0xF0
#define ADDRESS_PFC	0x01
#define ADDRESS_DCAC	0x02
#define ADDRESS_UPS	0x04
#define ADDRESS_BROCAST	0xFF

#define _REQUEST		0x01
#define ACK				0x41
#define NO_ERROR	0xF0
#define COM_INVAL	0xF2

#define PFC_ON		0x01
#define PFC_OFF		0x00

//Pfc command
#define COMMAND_WRITE_UP_AND_DOWN					0X0001
#define COMMAND_WRITE_SET_OUTPUT_VOLTAGE	0X0002

#define COMMAND_READ_UP_AND_DOWN					0X0101
#define COMMAND_READ_SET_OUTPUT_VOLTAGE		0X0102

#define COMMAND_READ_INTPUT_VOLTAGE_AB				0X0201
#define COMMAND_READ_INTPUT_VOLTAGE_BC		0X0202
#define COMMAND_READ_INTPUT_VOLTAGE_CA		0X0203

#define COMMAND_READ_INTPUT_CURRENT_A				0X0204
#define COMMAND_READ_INTPUT_CURRENT_B		0X0205
#define COMMAND_READ_INTPUT_CURRENT_C		0X0206

#define COMMAND_READ_BUS_VOLTAGE_P				0X0207
#define COMMAND_READ_BUS_VOLTAGE_N				0X0208
#define COMMAND_READ_BUS_VOLTAGE_PN		0X0209

#define COMMAND_READ_PFC_TEMP							0X020A
#define COMMAND_READ_AMBIENT_TEMP							0X020B
#define COMMAND_READ_AC_FRE					0X020C
#define COMMAND_READ_DC_OUTPUT_VOLTAGE					0X020D
#define COMMAND_READ_PFC_SOFTWARE_VERSION					0X020E
#define COMMAND_READ_PFC_HARDWARE_VERSION					0X020F
#define COMMAND_READ_PFC_WORK_SATEUS					0X0210
#define COMMAND_READ_PFC_FAULT_ID					0X0211
#define COMMAND_READ_PFC_WARN_ID					0X0212

//Dcac command
#define COMMAND_WRITE_DCAC_CONTROL					0X0401
#define COMMAND_WRITE_SET_FRE					0X0402
#define COMMAND_WRITE_AUTO_CAL_1V5					0X0410
#define COMMAND_WRITE_CAL_AC_VOLTAGE_K					0X0411
#define COMMAND_WRITE_CAL_AC_CURRENT_K					0X0412
#define COMMAND_WRITE_CAL_INDUCTANCE_CURRENT_K					0X0413
#define COMMAND_WRITE_CAL_BUS_VOLTAGE_K					0X0414
#define COMMAND_WRITE_CAL_BUS_VOLTAGE_B					0X0415

#define COMMAND_READ_DCAC_CONTROL					0X0501
#define COMMAND_READ_SET_FRE					0X0502
#define COMMAND_READ_AUTO_CAL_1V5					0X0510
#define COMMAND_READ_CAL_AC_VOLTAGE_K					0X0511
#define COMMAND_READ_CAL_AC_CURRENT_K					0X0512
#define COMMAND_READ_CAL_INDUCTANCE_CURRENT_K					0X0513
#define COMMAND_READ_CAL_BUS_VOLTAGE_K					0X0514
#define COMMAND_READ_CAL_BUS_VOLTAGE_B					0X0515

#define COMMAND_READ_DC_VOLTAGE_CAL_1V5_B					0X0516
#define COMMAND_READ_DC_CURRENT_CAL_1V5_B					0X0517
#define COMMAND_READ_INDUCTANCE_CURRENT_CAL_1V5_B					0X0518
#define COMMAND_READ_DCAC_WORK_SATEUS					0X0601
#define COMMAND_READ_DCAC_FAULT_ID					0X0602
#define COMMAND_READ_DCAC_SOFTWARE_VERSION					0X0605
#define COMMAND_READ_DCAC_HARDWARE_VERSION					0X0606

#define COMMAND_READ_DCAC_OUTPUT_VOLTAGE					0X0607
#define COMMAND_READ_DCAC_OUTPUT_CURRENT					0X0608
#define COMMAND_READ_DCAC_OUTPUT_POWER					0X0609
#define COMMAND_READ_DCAC_OUTPUT_FRE					0X060A
#define COMMAND_READ_DCAC_BUS_VOLTAGE					0X060B
#define COMMAND_READ_DCDC_TEMP					0X060C
#define COMMAND_READ_DCAC_AMBIENT_TEMP					0X060D
#define COMMAND_READ_DCAC_TEMP					0X060E
#define COMMAND_READ_DCAC_INDUCTANCE_CURRENT					0X060F

//UPS command
#define COMMAND_WRITE_UPS_CONTROL					0X1401
#define COMMAND_WRITE_UPS_CHARGE_MODE						0X1402
#define COMMAND_WRITE_UPS_OUTPUT_VOL			0X1403
#define COMMAND_WRITE_UPS_OUTPUT_CUR			0X1404
#define COMMAND_WRITE_UPS_CONTROL_MODE			0X1405

#define COMMAND_READ_UPS_CONTROL					0X1501
#define COMMAND_READ_UPS_CHARGE_MODE						0X1502
#define COMMAND_READ_UPS_OUTPUT_VOL			0X1503
#define COMMAND_READ_UPS_OUTPUT_CUR			0X1504
#define COMMAND_READ_UPS_CONTROL_MODE			0X1505

#define COMMAND_READ_UPS_WORK_STATUS			0X1421
#define COMMAND_READ_UPS_FAULT_ID					0X1422
#define COMMAND_READ_UPS_WARN_ID					0X1423
#define COMMAND_READ_UPS_CONNECT_ID					0X1424
#define COMMAND_READ_UPS_SOFTWARE_VERSION					0X1425
#define COMMAND_READ_UPS_HARDWARE_VERSION					0X1426
#define COMMAND_READ_UPS_BAT_VOL					0X1427
#define COMMAND_READ_UPS_MIDDLE_VOL					0X1428
#define COMMAND_READ_UPS_BUS_VOL					0X1429
#define COMMAND_READ_UPS_BAT_CUR					0X142A
#define COMMAND_READ_UPS_LC_CUR					0X142B
#define COMMAND_READ_UPS_CHARGE_CUR					0X142C
#define COMMAND_READ_UPS_DISCHAG_CUR_I					0X142D
#define COMMAND_READ_UPS_DISCHAG_CUR_II					0X142E
#define COMMAND_READ_UPS_BUS_CUR					0X142F
#define COMMAND_READ_UPS_TEMP					0X1430
#define COMMAND_READ_UPS_AMB_TEMP			0x1431
#define COMMAND_READ_UPS_REMAIN_CAPBILITY					0X1433
#define COMMAND_READ_UPS_REMAIN_TIME					0X1434


/* Exported types ------------------------------------------------------------*/
typedef union 
{
  struct
  {
		uint8_t RxMsgType[1];
		uint8_t	RxErrType[1];
		uint8_t RxValueType[2];
		uint8_t RxValue[4];
  } u8;                                  
  uint8_t all[8];                            
} CAN_RxMessageTypeDef;

typedef struct
{
	CAN_RxHeaderTypeDef RxHeader;
  CAN_RxMessageTypeDef	RxMessage;
}CAN_RxPacketTypeDef;

typedef union 
{
  struct
  {
		uint8_t TxMsgType[1];
		uint8_t	TxErrType[1];
		uint8_t TxValueType[2];
		uint8_t TxValue[4];
  } u8;                                  
  uint8_t all[8];                            
} CAN_TxMessageTypeDef;

typedef struct 
{
	CAN_TxHeaderTypeDef TxHeader;
	CAN_TxMessageTypeDef TxMessage;
}CAN_TxPacketTypeDef;

typedef struct
{
	uint8_t u8TransmitPFC;
	uint8_t u8TransmitDCAC;
	uint8_t u8TransmitUPS;
}CAN_TxEnableTypeDef;

typedef union 
{
  struct
  {
		uint8_t u8MidVolNotBal:1;
		uint8_t u8unused1:1;
		uint8_t u8CBCOcp:1;
		uint8_t u8PFCBusVolPosOVP:1;
		uint8_t u8PFCBusVolNegOVP:1;
		uint8_t u8BusSoftStarFault:1;
		uint8_t u8OcpForZero:1;
		uint8_t u8unused2:1;
		uint8_t ACFreAbnor:1;
		uint8_t ACVolAbnor:1;
		uint8_t u8unused3:1;
		uint8_t u8LossPhase:1;
		uint8_t u8unused4:1;
		uint8_t OTP:1;
		uint8_t u8unused5:1;
		uint8_t AmbOTP:1;
  } b;                                  
  uint16_t all;                            
} CAN_PFCFaultTypeDef;

typedef union 
{
  struct
  {
		uint8_t u8MidVolNotBal:1;
		uint8_t u8OTP:1;
		uint8_t u8AmbOTP:1;
		uint8_t u8unused1:1;
		uint8_t u8ThreePhaseNotBal:1;
		uint16_t u16unused2:11;
  } b;                                  
  uint16_t all;                            
} CAN_PFCWarnTypeDef;

typedef union 
{
  struct
  {
		uint8_t u8TurnOn:1;
		uint8_t u8TestForOpenLoop:1;
		uint8_t u8Unused:6;
  } b;                                  
  uint8_t all;                            
} CAN_DCAC1ComTypeDef;

typedef enum 
{
	DCAC_Init = 0,
	DCAC_Standby = 1,
	DCAC_SoftStart = 2,
	DCAC_Normal = 3,
	DCAC_Fault = 4,
	DCAC_Update = 5
}CAN_DCAC1StatusTypeDef;

typedef union 
{
  struct
  {
		uint8_t u8OpRMSOVP:1;
		uint8_t u8OpRMSUVP:1;
		uint8_t u8OpRMSOCP:1;
		uint8_t u8OpInsOVP:1;
		uint8_t u8OpInsOCP:1;
		uint8_t u8DCACBusOVP:1;
		uint8_t u8DCACBusUVP:1;
		uint8_t u8DCACOTP:1;
		uint8_t u8DCDCOTP:1;
		uint8_t u8AmbOTP:1;
		uint8_t u8HWOcp:1;
		uint8_t u8CanErr:1;
		uint8_t u8EEPErr:1;
		uint8_t u8FanErr:1;
		uint8_t u8CycleByCycleError:1;
		uint8_t u8Unused1:1;
		uint8_t u8OpOffsetErr:1;
		uint16_t u8Unused2:15;
  } b;                                  
  uint32_t all;                            
} CAN_DCAC1FaultTypeDef;

typedef union 
{
  struct
  {
		uint8_t u8TurnOn:1;
		uint8_t u8TestForOpenLoop:1;
		uint16_t u8Unused:14;
  } b;                                  
  uint16_t all;                            
} CAN_UPSComTypeDef;

typedef enum 
{
	UPS_Init = 0,
	UPS_SoftStartForChg = 1,
	UPS_SoftStartForDischg = 2,
	UPS_Chg = 3,
	UPS_Dischg = 4,
	UPS_Fault = 5,
	UPS_Update = 6
}CAN_UPSStatusTypeDef;

typedef union 
{
  struct
  {
		uint8_t u8SWOCPForChg:1;
		uint8_t u8SWOCPForDischg:1;
		uint8_t u8MidBusOVP:1;
		uint8_t u8OpBusOVP:1;
		uint8_t u8BatOVP:1;
		uint8_t u8LCOCP:1;
		uint8_t u8HWOCPForChg:1;
		uint8_t u8HWOCPForDischg:1;
		uint8_t u8HWOCPForOpBus:1;
		uint8_t u8HWOCPForMidBus:1;
		uint8_t u8AmbOTP:1;
		uint8_t u8OTP:1;
		uint8_t u8EEPErr:1;
		uint8_t u8FanErr:1;
		uint8_t u8BatLowVol:1;
		uint8_t u8OpBusLowVol:1;
		uint16_t u16Unused;
  } b;                                  
  uint32_t all;                            
} CAN_UPSFaultTypeDef;

typedef struct
{
	uint16_t u16IpUab;
	uint16_t u16IpUbc;
	uint16_t u16IpUca;
	uint16_t u16IpIa;
	uint16_t u16IpIb;
	uint16_t u16IpIc;
	uint16_t u16BusVolVp;
	uint16_t u16BusVolVn;
	uint16_t u16BusVolVpn;
	uint16_t u16Temp;
	uint16_t u16AmbTemp;
	uint16_t u16ACFre;
	uint16_t u16OpDCDCVol;
	uint32_t u32SWVer;
	uint32_t u32HWVer;
	uint16_t u16WorkStatus;
	uint16_t u16FaultId;
	uint16_t u16WarnId;
}CAN_PFCDataTypeDef;

typedef struct
{
	uint16_t u16CtrCom;
	uint16_t u16SetOpFre;
	uint16_t u16CalACVolKVal;
	uint16_t u16CalACCurKVal;
	uint16_t u16CalInductCurKVal;
	uint16_t u16CalBusVolKVal;
	uint16_t u16CalBusVolOffsetVal;
	uint16_t u16CalDCVolOffsetVal;
	uint16_t u16CalDCCurOffsetVal;
	uint16_t u16CalInductCurOffsetVal;
	uint8_t  u8WorkStatus;
	uint32_t u32FaultId;
	uint16_t u16Unused1;
	uint16_t u16Unused2;
	uint32_t u16SWVer;
	uint32_t u16HWVer;
	uint16_t u16OpVolRMS;
	uint16_t u16OpCurRMS;
	uint16_t u16OpPower;
	uint16_t u16OpFre;
	uint16_t u16ACBusVol;
	int16_t i16DCDCTemp;
	int16_t i16AmbTemp;
	int16_t i16DCACTemp;
	uint16_t u16InductCurRMS;
}CAN_DCACDataTypeDef;

typedef struct
{
	uint16_t u16CtrCom;
	uint16_t u16SetChgMod;
	uint16_t u16SetChgVol;
	uint16_t u16SetChgCur;
	uint16_t u16SetCtrMod;
	uint8_t u8WorkStatus;
	uint32_t u32FaultId;
	uint16_t u16Unused1;
	uint16_t u16Unused2;
	uint32_t u32SWVer;
	uint32_t u32HWVer;
	uint16_t u16BatVol;
	uint16_t u16MidVol;
	uint16_t u16BusVol;
	uint16_t u16BatCur;
	uint16_t u16LCCur;
	uint16_t u16ChgCur;
	uint16_t u16DisChgCurI;
	uint16_t u16DisChgCurII;
	uint16_t u16BusCur;
	int16_t i16Temp;
	int16_t i16AmbTemp;
	int16_t i16RemaCap;
	int16_t i16RemaTim;
}CAN_UPSDataTypeDef;

typedef struct
{
	uint16_t u16Trap1Enable;
	uint16_t u16Trap3Enable;
	uint16_t u16Trap4;
}CAN_TrapDataTypeDef;

typedef struct
{
  uint8_t u8Timer10msFlag;    
	
  uint8_t u8Timer100msFlag;

  uint8_t u8Timer1sFlag;

} Timer_CanTransmitTypeDef;

/* Exported constants --------------------------------------------------------*/
extern uint8_t u8CanReceiveFlag;
extern CAN_RxPacketTypeDef packet;
extern Timer_CanTransmitTypeDef Timer_CanTransmitFlag;
extern CAN_TxPacketTypeDef 	Tx_packet;
extern CAN_UPSDataTypeDef	UPSData;
/* Exported functions --------------------------------------------------------*/
extern void App_CAN(void);
extern void Task_ResolveCanMessage(CAN_RxPacketTypeDef packet);
extern void CAN_Rx_Msg(uint8_t u8fifox,
												uint16_t*id,
												uint8_t *ide,
												uint8_t *rtr,
												uint8_t *len,
												uint8_t dat[8],
												CAN_RxPacketTypeDef packet);

extern unsigned char CAN1_Tx_Msg(uint16_t*id,
								uint8_t *rtr,
								uint8_t *len,
								uint8_t dat[8],
								CAN_TxPacketTypeDef Tx_packet);

extern unsigned char CAN2_Tx_Msg(uint16_t*id,
								uint8_t *rtr,
								uint8_t *len,
								uint8_t dat[8],
								CAN_TxPacketTypeDef Tx_packet);

#endif /* __APP_CAN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
