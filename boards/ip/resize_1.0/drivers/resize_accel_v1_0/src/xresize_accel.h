// ==============================================================
// Vivado(TM) HLS - High-Level Synthesis from C, C++ and SystemC v2019.2 (64-bit)
// Copyright 1986-2019 Xilinx, Inc. All Rights Reserved.
// ==============================================================
#ifndef XRESIZE_ACCEL_H
#define XRESIZE_ACCEL_H

#ifdef __cplusplus
extern "C" {
#endif

/***************************** Include Files *********************************/
#ifndef __linux__
#include "xil_types.h"
#include "xil_assert.h"
#include "xstatus.h"
#include "xil_io.h"
#else
#include <stdint.h>
#include <assert.h>
#include <dirent.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/mman.h>
#include <unistd.h>
#include <stddef.h>
#endif
#include "xresize_accel_hw.h"

/**************************** Type Definitions ******************************/
#ifdef __linux__
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;
#else
typedef struct {
    u16 DeviceId;
    u32 Axilites_BaseAddress;
} XResize_accel_Config;
#endif

typedef struct {
    u32 Axilites_BaseAddress;
    u32 IsReady;
} XResize_accel;

/***************** Macros (Inline Functions) Definitions *********************/
#ifndef __linux__
#define XResize_accel_WriteReg(BaseAddress, RegOffset, Data) \
    Xil_Out32((BaseAddress) + (RegOffset), (u32)(Data))
#define XResize_accel_ReadReg(BaseAddress, RegOffset) \
    Xil_In32((BaseAddress) + (RegOffset))
#else
#define XResize_accel_WriteReg(BaseAddress, RegOffset, Data) \
    *(volatile u32*)((BaseAddress) + (RegOffset)) = (u32)(Data)
#define XResize_accel_ReadReg(BaseAddress, RegOffset) \
    *(volatile u32*)((BaseAddress) + (RegOffset))

#define Xil_AssertVoid(expr)    assert(expr)
#define Xil_AssertNonvoid(expr) assert(expr)

#define XST_SUCCESS             0
#define XST_DEVICE_NOT_FOUND    2
#define XST_OPEN_DEVICE_FAILED  3
#define XIL_COMPONENT_IS_READY  1
#endif

/************************** Function Prototypes *****************************/
#ifndef __linux__
int XResize_accel_Initialize(XResize_accel *InstancePtr, u16 DeviceId);
XResize_accel_Config* XResize_accel_LookupConfig(u16 DeviceId);
int XResize_accel_CfgInitialize(XResize_accel *InstancePtr, XResize_accel_Config *ConfigPtr);
#else
int XResize_accel_Initialize(XResize_accel *InstancePtr, const char* InstanceName);
int XResize_accel_Release(XResize_accel *InstancePtr);
#endif

void XResize_accel_Start(XResize_accel *InstancePtr);
u32 XResize_accel_IsDone(XResize_accel *InstancePtr);
u32 XResize_accel_IsIdle(XResize_accel *InstancePtr);
u32 XResize_accel_IsReady(XResize_accel *InstancePtr);
void XResize_accel_EnableAutoRestart(XResize_accel *InstancePtr);
void XResize_accel_DisableAutoRestart(XResize_accel *InstancePtr);

void XResize_accel_Set_src_rows(XResize_accel *InstancePtr, u32 Data);
u32 XResize_accel_Get_src_rows(XResize_accel *InstancePtr);
void XResize_accel_Set_src_cols(XResize_accel *InstancePtr, u32 Data);
u32 XResize_accel_Get_src_cols(XResize_accel *InstancePtr);
void XResize_accel_Set_dst_rows(XResize_accel *InstancePtr, u32 Data);
u32 XResize_accel_Get_dst_rows(XResize_accel *InstancePtr);
void XResize_accel_Set_dst_cols(XResize_accel *InstancePtr, u32 Data);
u32 XResize_accel_Get_dst_cols(XResize_accel *InstancePtr);

void XResize_accel_InterruptGlobalEnable(XResize_accel *InstancePtr);
void XResize_accel_InterruptGlobalDisable(XResize_accel *InstancePtr);
void XResize_accel_InterruptEnable(XResize_accel *InstancePtr, u32 Mask);
void XResize_accel_InterruptDisable(XResize_accel *InstancePtr, u32 Mask);
void XResize_accel_InterruptClear(XResize_accel *InstancePtr, u32 Mask);
u32 XResize_accel_InterruptGetEnabled(XResize_accel *InstancePtr);
u32 XResize_accel_InterruptGetStatus(XResize_accel *InstancePtr);

#ifdef __cplusplus
}
#endif

#endif
