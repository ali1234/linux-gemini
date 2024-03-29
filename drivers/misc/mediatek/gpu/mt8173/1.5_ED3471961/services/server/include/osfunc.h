/**************************************************************************/ /*!
@File
@Title          OS functions header
@Copyright      Copyright (c) Imagination Technologies Ltd. All Rights Reserved
@Description    OS specific API definitions
@License        Dual MIT/GPLv2

The contents of this file are subject to the MIT license as set out below.

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in
all copies or substantial portions of the Software.

Alternatively, the contents of this file may be used under the terms of
the GNU General Public License Version 2 ("GPL") in which case the provisions
of GPL are applicable instead of those above.

If you wish to allow use of your version of this file only under the terms of
GPL, and not to allow others to use your version of this file under the terms
of the MIT license, indicate your decision by deleting the provisions above
and replace them with the notice and other provisions required by GPL as set
out in the file called "GPL-COPYING" included in this distribution. If you do
not delete the provisions above, a recipient may use your version of this file
under the terms of either the MIT license or GPL.

This License is also included in this distribution in the file called
"MIT-COPYING".

EXCEPT AS OTHERWISE STATED IN A NEGOTIATED AGREEMENT: (A) THE SOFTWARE IS
PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING
BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR
PURPOSE AND NONINFRINGEMENT; AND (B) IN NO EVENT SHALL THE AUTHORS OR
COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/ /***************************************************************************/

#ifdef DEBUG_RELEASE_BUILD
#pragma optimize( "", off )
#define DEBUG		1
#endif

#ifndef __OSFUNC_H__
#define __OSFUNC_H__


#if defined(__KERNEL__) && defined(ANDROID) && !defined(__GENKSYMS__)
#define __pvrsrv_defined_struct_enum__
#include <services_kernel_client.h>
#endif

#include "img_types.h"
#include "pvrsrv_device.h"
#include "device.h"

/******************************************************************************
 * Static defines
 *****************************************************************************/
#define KERNEL_ID			0xffffffffL
#define ISR_ID				0xfffffffdL

IMG_UINT64 OSClockns64(IMG_VOID);
IMG_UINT64 OSClockus64(IMG_VOID);
IMG_UINT32 OSClockus(IMG_VOID);
IMG_UINT32 OSClockms(IMG_VOID);

IMG_SIZE_T OSGetPageSize(IMG_VOID);
IMG_SIZE_T OSGetPageShift(IMG_VOID);
IMG_SIZE_T OSGetPageMask(IMG_VOID);

typedef IMG_VOID (*PFN_THREAD)(IMG_PVOID pvData);

PVRSRV_ERROR OSInstallDeviceLISR(PVRSRV_DEVICE_CONFIG *psDevConfig,
								 IMG_HANDLE *hLISRData,
								 PFN_LISR pfnLISR,
								 IMG_VOID *hData);
PVRSRV_ERROR OSUninstallDeviceLISR(IMG_HANDLE hLISRData);

PVRSRV_ERROR OSInstallMISR(IMG_HANDLE *hMISRData,
						   PFN_MISR pfnMISR,
						   IMG_VOID *hData);
PVRSRV_ERROR OSUninstallMISR(IMG_HANDLE hMISRData);
PVRSRV_ERROR OSScheduleMISR(IMG_HANDLE hMISRData);




/*************************************************************************/ /*!
@Function       OSThreadCreate
@Description    Creates a kernel thread and starts it running. The caller
                is responsible for informing the thread that it must finish
                and return from the pfnThread function. It is not possible
                to kill or terminate it.The new thread runs with the default
                priority provided by the Operating System.
@Output         phThread       Returned handle to the thread.
@Input          pszThreadName  Name to assign to the thread.
@Input          pfnThread      Thread entry point function.
@Input          hData          Thread specific data pointer for pfnThread().
@Return         Standard PVRSRV_ERROR error code.
*/ /**************************************************************************/

PVRSRV_ERROR OSThreadCreate(IMG_HANDLE *phThread,
							IMG_CHAR *pszThreadName,
							PFN_THREAD pfnThread,
							IMG_VOID *hData);

/*! Available priority levels for the creation of a new Kernel Thread. */
typedef enum priority_levels
{
	HIGHEST_PRIORITY = 0,
	HIGH_PRIRIOTY,
	NORMAL_PRIORITY,
	LOW_PRIORITY,
	LOWEST_PRIORITY,
	NOSET_PRIORITY,   /* With this option the priority level is is the default for the given OS */
	LAST_PRIORITY     /* This must be always the last entry */
} OS_THREAD_LEVEL;

/*************************************************************************/ /*!
@Function       OSThreadCreatePriority
@Description    As OSThreadCreate, this function creates a kernel thread and
                starts it running. The difference is that with this function
                is possible to specify the priority used to schedule the new
                thread.

@Output         phThread        Returned handle to the thread.
@Input          pszThreadName   Name to assign to the thread.
@Input          pfnThread       Thread entry point function.
@Input          hData           Thread specific data pointer for pfnThread().
@Input          eThreadPriority Priority level to assign to the new thread.
@Return         Standard PVRSRV_ERROR error code.
*/ /**************************************************************************/
PVRSRV_ERROR OSThreadCreatePriority(IMG_HANDLE *phThread,
                                    IMG_CHAR *pszThreadName,
                                    PFN_THREAD pfnThread,
                                    IMG_VOID *hData,
                                    OS_THREAD_LEVEL eThreadPriority);

/*************************************************************************/ /*!
@Function       OSThreadDestroy
@Description    Waits for the thread to end and then destroys the thread
                handle memory. This function will block and wait for the
                thread to finish successfully, thereby providing a sync point
                for the thread completing its work. No attempt is made to kill
                or otherwise terminate the thread.
@Input          phThread  The thread handle returned by OSThreadCreate().
@Return         Standard PVRSRV_ERROR error code.
*/ /**************************************************************************/
PVRSRV_ERROR OSThreadDestroy(IMG_HANDLE hThread);

IMG_VOID OSMemCopy(IMG_VOID *pvDst, const IMG_VOID *pvSrc, IMG_SIZE_T ui32Size);
IMG_VOID OSMemCopyMTK(IMG_VOID *pvDst, const IMG_VOID *pvSrc, IMG_SIZE_T ui32Size);
IMG_VOID *OSMapPhysToLin(IMG_CPU_PHYADDR BasePAddr, IMG_SIZE_T ui32Bytes, IMG_UINT32 ui32Flags);
IMG_BOOL OSUnMapPhysToLin(IMG_VOID *pvLinAddr, IMG_SIZE_T ui32Bytes, IMG_UINT32 ui32Flags);


IMG_VOID OSCPUOperation(PVRSRV_CACHE_OP eCacheOp);

IMG_VOID OSFlushCPUCacheRangeKM(IMG_PVOID pvVirtStart,
								IMG_PVOID pvVirtEnd,
								IMG_CPU_PHYADDR sCPUPhysStart,
								IMG_CPU_PHYADDR sCPUPhysEnd);


IMG_VOID OSCleanCPUCacheRangeKM(IMG_PVOID pvVirtStart,
								IMG_PVOID pvVirtEnd,
								IMG_CPU_PHYADDR sCPUPhysStart,
								IMG_CPU_PHYADDR sCPUPhysEnd);

IMG_VOID OSInvalidateCPUCacheRangeKM(IMG_PVOID pvVirtStart,
									 IMG_PVOID pvVirtEnd,
									 IMG_CPU_PHYADDR sCPUPhysStart,
									 IMG_CPU_PHYADDR sCPUPhysEnd);


IMG_PID OSGetCurrentProcessIDKM(IMG_VOID);
IMG_CHAR *OSGetCurrentProcessNameKM(IMG_VOID);
IMG_UINTPTR_T OSGetCurrentThreadIDKM(IMG_VOID);
IMG_VOID OSMemSet(IMG_VOID *pvDest, IMG_UINT8 ui8Value, IMG_SIZE_T ui32Size);
IMG_INT OSMemCmp(IMG_VOID *pvBufA, IMG_VOID *pvBufB, IMG_SIZE_T uiLen);

PVRSRV_ERROR OSMMUPxAlloc(PVRSRV_DEVICE_NODE *psDevNode, IMG_SIZE_T uiSize,
							Px_HANDLE *psMemHandle, IMG_DEV_PHYADDR *psDevPAddr);

IMG_VOID OSMMUPxFree(PVRSRV_DEVICE_NODE *psDevNode, Px_HANDLE *psMemHandle);

PVRSRV_ERROR OSMMUPxMap(PVRSRV_DEVICE_NODE *psDevNode, Px_HANDLE *psMemHandle,
						IMG_SIZE_T uiSize, IMG_DEV_PHYADDR *psDevPAddr,
						IMG_VOID **pvPtr);

IMG_VOID OSMMUPxUnmap(PVRSRV_DEVICE_NODE *psDevNode, Px_HANDLE *psMemHandle, IMG_VOID *pvPtr);


PVRSRV_ERROR OSInitEnvData(IMG_VOID);
IMG_VOID OSDeInitEnvData(IMG_VOID);

IMG_CHAR* OSStringNCopy(IMG_CHAR *pszDest, const IMG_CHAR *pszSrc, IMG_SIZE_T uSize);
IMG_INT32 OSSNPrintf(IMG_CHAR *pStr, IMG_SIZE_T ui32Size, const IMG_CHAR *pszFormat, ...) IMG_FORMAT_PRINTF(3, 4);
IMG_SIZE_T OSStringLength(const IMG_CHAR *pStr);
IMG_SIZE_T OSStringNLength(const IMG_CHAR *pStr, IMG_SIZE_T uiCount);
IMG_INT32 OSStringCompare(const IMG_CHAR *pStr1, const IMG_CHAR *pStr2);

PVRSRV_ERROR OSEventObjectCreate(const IMG_CHAR *pszName,
								 IMG_HANDLE *EventObject);
PVRSRV_ERROR OSEventObjectDestroy(IMG_HANDLE hEventObject);
PVRSRV_ERROR OSEventObjectSignal(IMG_HANDLE hEventObject);
PVRSRV_ERROR OSEventObjectWait(IMG_HANDLE hOSEventKM);
PVRSRV_ERROR OSEventObjectWaitTimeout(IMG_HANDLE hOSEventKM, IMG_UINT32 uiTimeoutMs);
PVRSRV_ERROR OSEventObjectWaitAndHoldBridgeLock(IMG_HANDLE hOSEventKM);
PVRSRV_ERROR OSEventObjectWaitTimeoutAndHoldBridgeLock(IMG_HANDLE hOSEventKM, IMG_UINT32 uiTimeoutMs);
PVRSRV_ERROR OSEventObjectOpen(IMG_HANDLE hEventObject,
											IMG_HANDLE *phOSEvent);
PVRSRV_ERROR OSEventObjectClose(IMG_HANDLE hOSEventKM);

/* Avoid macros so we don't evaluate pszSrc twice */
static INLINE IMG_CHAR *OSStringCopy(IMG_CHAR *pszDest, const IMG_CHAR *pszSrc)
{
	IMG_CHAR  *result; /* Helps with klocwork, will be optimised away */

	result = OSStringNCopy(pszDest, pszSrc, OSStringLength(pszSrc)+1);
#if defined(__KLOCWORK__)
	pszDest[OSStringLength(pszSrc)] = '\0';
#endif
	return result;
}

/*!
******************************************************************************

 @Function OSWaitus
 
 @Description 
    This function implements a busy wait of the specified microseconds
    This function does NOT release thread quanta
 
 @Input ui32Timeus - (us)

 @Return IMG_VOID

******************************************************************************/ 
IMG_VOID OSWaitus(IMG_UINT32 ui32Timeus);


/*!
******************************************************************************

 @Function OSSleepms
 
 @Description 
    This function implements a sleep of the specified milliseconds
    This function may allow pre-emption if implemented
 
 @Input ui32Timems - (ms)

 @Return IMG_VOID

******************************************************************************/ 
IMG_VOID OSSleepms(IMG_UINT32 ui32Timems);

IMG_VOID OSReleaseThreadQuanta(IMG_VOID);

IMG_UINT8 OSReadHWReg8(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset);
IMG_UINT16 OSReadHWReg16(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset);
IMG_UINT32 OSReadHWReg32(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset);
IMG_UINT64 OSReadHWReg64(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset);
IMG_UINT64 OSReadHWRegBank(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset, IMG_UINT8 *pui8DstBuf, IMG_UINT64 ui64DstBufLen);

IMG_VOID OSWriteHWReg8(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset, IMG_UINT8 ui32Value);
IMG_VOID OSWriteHWReg16(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset, IMG_UINT16 ui32Value);
IMG_VOID OSWriteHWReg32(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset, IMG_UINT32 ui32Value);
IMG_VOID OSWriteHWReg64(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset, IMG_UINT64 ui64Value);
IMG_UINT64 OSWriteHWRegBank(IMG_PVOID pvLinRegBaseAddr, IMG_UINT32 ui32Offset, IMG_UINT8 *pui8SrcBuf, IMG_UINT64 ui64rcBufLen);

typedef IMG_VOID (*PFN_TIMER_FUNC)(IMG_VOID*);
IMG_HANDLE OSAddTimer(PFN_TIMER_FUNC pfnTimerFunc, IMG_VOID *pvData, IMG_UINT32 ui32MsTimeout);
PVRSRV_ERROR OSRemoveTimer(IMG_HANDLE hTimer);
PVRSRV_ERROR OSEnableTimer(IMG_HANDLE hTimer);
PVRSRV_ERROR OSDisableTimer(IMG_HANDLE hTimer);


/******************************************************************************

 @Function		OSPanic

 @Description	Take action in response to an unrecoverable driver error

 @Input    IMG_VOID

 @Return   IMG_VOID

******************************************************************************/
IMG_VOID OSPanic(IMG_VOID);

IMG_BOOL OSProcHasPrivSrvInit(IMG_VOID);

typedef enum _img_verify_test
{
	PVR_VERIFY_WRITE = 0,
	PVR_VERIFY_READ
} IMG_VERIFY_TEST;

IMG_BOOL OSAccessOK(IMG_VERIFY_TEST eVerification, IMG_VOID *pvUserPtr, IMG_SIZE_T ui32Bytes);

PVRSRV_ERROR OSCopyToUser(IMG_PVOID pvProcess, IMG_VOID *pvDest, const IMG_VOID *pvSrc, IMG_SIZE_T ui32Bytes);
PVRSRV_ERROR OSCopyFromUser(IMG_PVOID pvProcess, IMG_VOID *pvDest, const IMG_VOID *pvSrc, IMG_SIZE_T ui32Bytes);

#if defined (__linux__)
#define OSBridgeCopyFromUser OSCopyFromUser
#define OSBridgeCopyToUser OSCopyToUser
#else
PVRSRV_ERROR OSBridgeCopyFromUser (IMG_PVOID pvProcess,
						IMG_PVOID pvDest,
						const IMG_PVOID pvSrc,
						IMG_SIZE_T ui32Bytes);
PVRSRV_ERROR OSBridgeCopyToUser (IMG_PVOID pvProcess,
						IMG_VOID *pvDest,
						const IMG_VOID *pvSrc,
						IMG_SIZE_T ui32Bytes);
#endif

PVRSRV_ERROR OSGetGlobalBridgeBuffers (IMG_VOID **ppvBridgeInBuffer,
							IMG_UINT32 *pui32BridgeInBufferSize,
							IMG_VOID **ppvBridgeOutBuffer,
							IMG_UINT32 *pui32BridgeOutBufferSize);


IMG_VOID OSWriteMemoryBarrier(IMG_VOID);
IMG_VOID OSMemoryBarrier(IMG_VOID);

#if defined(LINUX) && defined(__KERNEL__)

/* Provide LockDep friendly definitions for Services RW locks */
#include <linux/mutex.h>
#include <linux/slab.h>
#include "allocmem.h"

typedef struct rw_semaphore *POSWR_LOCK;

#define OSWRLockCreate(ppsLock) ({ \
	PVRSRV_ERROR e = PVRSRV_ERROR_OUT_OF_MEMORY; \
	*(ppsLock) = OSAllocMem(sizeof(struct rw_semaphore)); \
	if (*(ppsLock)) { init_rwsem(*(ppsLock)); e = PVRSRV_OK; }; \
	e;})
#define OSWRLockDestroy(psLock) ({OSFreeMem(psLock); PVRSRV_OK;})

#define OSWRLockAcquireRead(psLock) ({down_read(psLock); PVRSRV_OK;})
#define OSWRLockReleaseRead(psLock) ({up_read(psLock); PVRSRV_OK;})
#define OSWRLockAcquireWrite(psLock) ({down_write(psLock); PVRSRV_OK;})
#define OSWRLockReleaseWrite(psLock) ({up_write(psLock); PVRSRV_OK;})

#elif defined(LINUX) || defined(__QNXNTO__)
/* User-mode unit tests use these definitions on Linux */

typedef struct _OSWR_LOCK_ *POSWR_LOCK;

PVRSRV_ERROR OSWRLockCreate(POSWR_LOCK *ppsLock);
IMG_VOID OSWRLockDestroy(POSWR_LOCK psLock);
IMG_VOID OSWRLockAcquireRead(POSWR_LOCK psLock);
IMG_VOID OSWRLockReleaseRead(POSWR_LOCK psLock);
IMG_VOID OSWRLockAcquireWrite(POSWR_LOCK psLock);
IMG_VOID OSWRLockReleaseWrite(POSWR_LOCK psLock);

#else
struct _OSWR_LOCK_ {
	IMG_UINT32 ui32Dummy;
};
static INLINE PVRSRV_ERROR OSWRLockCreate(POSWR_LOCK *ppsLock)
{
	PVR_UNREFERENCED_PARAMETER(ppsLock);
	return PVRSRV_OK;
}

static INLINE IMG_VOID OSWRLockDestroy(POSWR_LOCK psLock)
{
	PVR_UNREFERENCED_PARAMETER(psLock);
}

static INLINE IMG_VOID OSWRLockAcquireRead(POSWR_LOCK psLock)
{
	PVR_UNREFERENCED_PARAMETER(psLock);
}

static INLINE IMG_VOID OSWRLockReleaseRead(POSWR_LOCK psLock)
{
	PVR_UNREFERENCED_PARAMETER(psLock);
}

static INLINE IMG_VOID OSWRLockAcquireWrite(POSWR_LOCK psLock)
{
	PVR_UNREFERENCED_PARAMETER(psLock);
}

static INLINE IMG_VOID OSWRLockReleaseWrite(POSWR_LOCK psLock)
{
	PVR_UNREFERENCED_PARAMETER(psLock);
}
#endif

IMG_UINT64 OSDivide64r64(IMG_UINT64 ui64Divident, IMG_UINT32 ui32Divisor, IMG_UINT32 *pui32Remainder);
IMG_UINT32 OSDivide64(IMG_UINT64 ui64Divident, IMG_UINT32 ui32Divisor, IMG_UINT32 *pui32Remainder);

IMG_VOID OSDumpStack(IMG_VOID);

IMG_VOID OSAcquireBridgeLock(IMG_VOID);
IMG_VOID OSReleaseBridgeLock(IMG_VOID);


/*
 *  Functions for providing support for PID statistics.
 */
typedef void (OS_STATS_PRINTF_FUNC)(const IMG_CHAR *pszFormat, ...);
 
typedef void (OS_STATS_PRINT_FUNC)(IMG_PVOID pvStatPtr,
                                   OS_STATS_PRINTF_FUNC* pfnOSGetStatsPrintf);

IMG_PVOID OSCreateStatisticEntry(IMG_CHAR* pszName, IMG_PVOID pvFolder,
                                 OS_STATS_PRINT_FUNC* pfnStatsPrint,
                                 IMG_PVOID pvData);
IMG_VOID OSRemoveStatisticEntry(IMG_PVOID pvEntry);
IMG_PVOID OSCreateStatisticFolder(IMG_CHAR *pszName, IMG_PVOID pvFolder);
IMG_VOID OSRemoveStatisticFolder(IMG_PVOID pvFolder);


#endif /* __OSFUNC_H__ */

/******************************************************************************
 End of file (osfunc.h)
******************************************************************************/

