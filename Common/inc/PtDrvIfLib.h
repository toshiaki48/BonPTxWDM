#pragma once

#include <windows.h>

#define	PT_CH_NAME_SIZE		(16)

#define	PT_DEV_BT_NAME		L"Ec%dBt%d"
#define	PT_DEV_BS_NAME		L"Ec%dBs%d"

typedef	enum	{
	BS_LNB_POWER_OFF,
	BS_LNB_POWER_11V,
	BS_LNB_POWER_15V,
	BS_LNB_POWER_MAX
} BS_LNB_POWER;

typedef	enum	{
	LAYER_BS_LOW = 0,
	LAYER_BS_HI,
	LAYER_BS_MAX,
	LAYER_BT_A = 0,
	LAYER_BT_B,
	LAYER_BT_C,
	LAYER_BT_MAX
} LAYER_INDEX;

typedef	struct	{
	UINT	Numerator;
	UINT	Denominator;
} TS_ERR_RATE;

typedef	struct{
	union{
		struct	{
			BYTE	system;
			BYTE	indicator;
			BYTE	emergency;
			BYTE	partial;
			BYTE	mode[3];
			BYTE	rate[3];
			BYTE	interleave[3];
			BYTE	segment[3];
			BYTE	phase;
		}bt;
		struct	{
			bool	tmcerr;
			BYTE	indicator;
			WORD	tsId[8];
		}bs;
	}u;
}TMCC_STATUS;

typedef	struct	{
	bool	null_h,null_l;
	BYTE	rateh,ratel;
	BYTE	sloth,slotl;
}BS_LAYER_STATUS;

typedef	struct	{
	union{
		struct	{
			bool	a;
			bool	b;
			bool	c;
		}bt;
		struct	{
			bool	h;
		}bs;
		DWORD	all;
	}u;
} LAYER_MASK;

typedef	struct	{
	bool	retryov;
	bool	alarm;
	bool	tmunvld;
	bool	mdunvld;
	bool	fulock;
	bool	vulock;
	bool	rulock;
	bool	rseorf;
}BT_TUNER_STATUS;

typedef	struct	{
	bool	overFlow;
	bool	empty;
	bool	busy;
}DMA_INFOMATION;

extern HANDLE __cdecl PtDrvOpen(const wchar_t *pDevName);
extern BOOL __cdecl PtDrvClose(HANDLE hPt);
extern int __cdecl PtDrvGetDevCount(void);
extern NTSTATUS __cdecl PtDrvGetVersion(HANDLE hPt,UINT *version);
extern NTSTATUS __cdecl PtDrvGetPciClockCounter(HANDLE hPt,UINT *counter);
extern NTSTATUS __cdecl PtDrvSetPciLatencyTimer(HANDLE hPt,BYTE latencyTimer);
extern NTSTATUS __cdecl PtDrvGetPciLatencyTimer(HANDLE hPt,BYTE *latencyTimer);
extern NTSTATUS __cdecl PtDrvSetLnbPower(HANDLE hPt,BS_LNB_POWER lnbPower);
extern NTSTATUS __cdecl PtDrvGetLnbPower(HANDLE hPt,BS_LNB_POWER *lnbPower);
extern NTSTATUS __cdecl PtDrvSetTunerSleep(HANDLE hPt,bool sleep);
extern NTSTATUS __cdecl PtDrvGetTunerSleep(HANDLE hPt,bool *sleep);
extern NTSTATUS __cdecl PtDrvGetChannelMax(HANDLE hPt,int *pChannelMax);
extern NTSTATUS __cdecl PtDrvGetChannelInfo(HANDLE hPt,UINT channel,int *pFrequency,char const **ppChName);
extern NTSTATUS __cdecl PtDrvSetFrequency(HANDLE hPt,UINT channel);
extern NTSTATUS __cdecl PtDrvGetFrequency(HANDLE hPt,UINT *channel);
extern NTSTATUS __cdecl PtDrvGetFrequencyStatus(HANDLE hPt,int *clock,int *carrier);
extern NTSTATUS __cdecl PtDrvGetCnAgc(HANDLE hPt,UINT *cn100,UINT *curAgc,UINT *maxAgc);
extern NTSTATUS __cdecl PtDrvSetIdS(HANDLE hPt,UINT id);
extern NTSTATUS __cdecl PtDrvGetIdS(HANDLE hPt,UINT *id);
extern NTSTATUS __cdecl PtDrvGetCorrectedErrorRate(HANDLE hPt,LAYER_INDEX layIndex,TS_ERR_RATE *errRate);
extern NTSTATUS __cdecl PtDrvResetCorrectedErrorCount(HANDLE hPt);
extern NTSTATUS __cdecl PtDrvGetErrorCount(HANDLE hPt,UINT *count);
extern NTSTATUS __cdecl PtDrvGetTunerStatus(HANDLE hPt,BT_TUNER_STATUS *status);
extern NTSTATUS __cdecl PtDrvGetTmcc(HANDLE hPt,TMCC_STATUS *tmcc);
extern NTSTATUS __cdecl PtDrvGetLayerS(HANDLE hPt,BS_LAYER_STATUS *layerS);
extern NTSTATUS __cdecl PtDrvGetLockedT(HANDLE hPt,bool locked[3]);
extern NTSTATUS __cdecl PtDrvSetLayerEnable(HANDLE hPt,LAYER_MASK *layerMask);
extern NTSTATUS __cdecl PtDrvGetLayerEnable(HANDLE hPt,LAYER_MASK *layerMask);
extern NTSTATUS __cdecl PtDrvSetStreamEnable(HANDLE hPt,bool enable);
extern NTSTATUS __cdecl PtDrvGetStreamEnable(HANDLE hPt,bool *enable);
extern NTSTATUS __cdecl PtDrvGetTransferInfo(HANDLE hPt,DMA_INFOMATION *pInfo);
extern NTSTATUS __cdecl PtDrvGetTransferPktErr(HANDLE hPt,bool *pDmaPktErr);
extern NTSTATUS __cdecl PtDrvGetStreamSize(HANDLE hPt,UINT *tsSize);
extern NTSTATUS __cdecl PtDrvGetStreamData(HANDLE hPt,void *ts,UINT size,UINT *outSize);
extern NTSTATUS __cdecl PtDrvGetStreamFlush(HANDLE hPt);

extern void __cdecl DrvCnvMjd2Ymd(UINT mjd,UINT &yy,UCHAR &mm,UCHAR &dd);
extern UINT __cdecl DrvCnvYmd2Mjd(UINT yy,UCHAR mm,UCHAR dd);
extern bool __cdecl DrvChkSection(void const *pSection,UINT size);
extern UINT __cdecl DrvCnvEuni2uni(void const *pEights,wchar_t *pUnicodes,int outSize);

