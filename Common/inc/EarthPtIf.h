// ========================================================================================
//	EarthPtIf.h
// ========================================================================================

#ifndef _EARTH_PT_IF_H_
#define _EARTH_PT_IF_H_
#include <windows.h>

namespace EARTH{
	typedef signed char			sbyte;
	typedef unsigned char		byte;
	typedef unsigned short		ushort;
	typedef unsigned long		uint;
	typedef signed long long	slong;
	typedef unsigned long long	ulong;
	typedef signed long			status;

namespace PT{
	class	Device;
	class	Device2;

	enum Status{
		STATUS_OK = (0x00000000),

		STATUS_GENERAL_ERROR = (0xe0008100),
		STATUS_NOT_IMPLIMENTED,
		STATUS_INVALID_PARAM_ERROR,
		STATUS_OUT_OF_MEMORY_ERROR,
		STATUS_INTERNAL_ERROR,

		STATUS_WDAPI_LOAD_ERROR = (0xe0008200),
		STATUS_ALL_DEVICES_MUST_BE_DELETED_ERROR,

		STATUS_PCI_BUS_ERROR = (0xe0008300),
		STATUS_CONFIG_REVISION_ERROR,
		STATUS_FPGA_VERSION_ERROR,
		STATUS_PCI_BASE_ADDRESS_ERROR,
		STATUS_FLASH_MEMORY_ERROR,
		STATUS_DCM_LOCK_TIMEOUT_ERROR,
		STATUS_DCM_SHIFT_TIMEOUT_ERROR,
		STATUS_POWER_RESET_ERROR,
		STATUS_I2C_ERROR,
		STATUS_TUNER_IS_SLEEP_ERROR,
		STATUS_PLL_OUT_OF_RANGE_ERROR,
		STATUS_PLL_LOCK_TIMEOUT_ERROR,
		STATUS_VIRTUAL_ALLOC_ERROR,
		STATUS_DMA_ADDRESS_ERROR,
		STATUS_BUFFER_ALREADY_ALLOCATED_ERROR,
		STATUS_DEVICE_IS_ALREADY_OPEN_ERROR,
		STATUS_DEVICE_IS_NOT_OPEN_ERROR,
		STATUS_BUFFER_IS_IN_USE_ERROR,
		STATUS_BUFFER_IS_NOT_ALLOCATED_ERROR,
		STATUS_DEVICE_MUST_BE_CLOSED_ERROR,

		STATUS_WD_DriverName_ERROR = (0xe0008400),
		STATUS_WD_Open_ERROR = (0xe0008401),
		STATUS_WD_Close_ERROR,
		STATUS_WD_Version_ERROR,
		STATUS_WD_License_ERROR,
		STATUS_WD_PciScanCards_ERROR,
		STATUS_WD_PciConfigDump_ERROR,
		STATUS_WD_PciGetCardInfo_ERROR,
		STATUS_WD_PciGetCardInfo_Bus_ERROR,
		STATUS_WD_PciGetCardInfo_Memory_ERROR,
		STATUS_WD_CardRegister_ERROR,
		STATUS_WD_CardUnregister_ERROR,
		STATUS_WD_CardCleanupSetup_ERROR,
		STATUS_WD_DMALock_ERROR,
		STATUS_WD_DMAUnlock_ERROR,
		STATUS_WD_DMASyncCpu_ERROR,
		STATUS_WD_DMASyncIo_ERROR
	};

	class Bus{
	public:
		typedef status (*NewBusFunction)(Bus **bus);
		struct DeviceInfo {
			wchar_t	DevName[4][8];
		};

		status Delete(){return STATUS_OK;};
		status GetVersion(uint *version);
		status Scan(DeviceInfo *deviceInfoPtr,uint *deviceInfoCount,uint maxBadBitCount = 0);
		status NewDevice(const DeviceInfo *deviceInfoPtr,Device **device,Device2 **device2 = NULL);
	};

	class Device{
	public:
		enum LnbPower {
			LNB_POWER_OFF,
			LNB_POWER_15V,
			LNB_POWER_11V
		};
		enum TunerPowerReset {
			TUNER_POWER_OFF,
			TUNER_POWER_ON_RESET_ENABLE,
			TUNER_POWER_ON_RESET_DISABLE
		};
		enum ISDB {
			ISDB_S,
			ISDB_T,
			ISDB_COUNT
		};
		enum LayerIndex {
			LAYER_INDEX_L = 0,
			LAYER_INDEX_H,
			LAYER_INDEX_A = 0,
			LAYER_INDEX_B,
			LAYER_INDEX_C
		};
		enum LayerCount {
			LAYER_COUNT_S = LAYER_INDEX_H + 1,
			LAYER_COUNT_T = LAYER_INDEX_C + 1
		};
		struct ErrorRate {
			uint	Numerator,Denominator;
		};
		struct TmccS {
			uint	Indicator;
			uint	Mode[4];
			uint	Slot[4];
			uint	Id[8];
			uint	Emergency;
			uint	UpLink;
			uint	ExtFlag;
			uint	ExtData[2];
		};
		struct LayerS {
			uint	Mode[LAYER_COUNT_S];
			uint	Count[LAYER_COUNT_S];
		};
		struct TmccT {
			uint	System;
			uint	Indicator;
			uint	Emergency;
			uint	Partial;
			uint	Mode[LAYER_COUNT_T];
			uint	Rate[LAYER_COUNT_T];
			uint	Interleave[LAYER_COUNT_T];
			uint	Segment[LAYER_COUNT_T];
			uint	Phase;
			uint	Reserved;
		};
		enum LayerMask {
			LAYER_MASK_NONE,
			LAYER_MASK_L = 1 << LAYER_INDEX_L,
			LAYER_MASK_H = 1 << LAYER_INDEX_H,
			LAYER_MASK_A = 1 << LAYER_INDEX_A,
			LAYER_MASK_B = 1 << LAYER_INDEX_B,
			LAYER_MASK_C = 1 << LAYER_INDEX_C
		};
		enum {
			BUFFER_PAGE_COUNT = 511
		};
		struct BufferInfo {
			uint	VirtualSize;
			uint	VirtualCount;
			uint	LockSize;
		};
		struct TransferInfo {
			bool	TransferCounter0;
			bool	TransferCounter1;
			bool	BufferOverflow;
		};
		enum {
			CH_NAME_SIZE = 16
		};

		status Delete();
		status Open(const PT::Bus::DeviceInfo *deviceInfo);
		status Close();
		status GetPciClockCounter(uint *counter);
		status SetPciLatencyTimer(byte latencyTimer);
		status GetPciLatencyTimer(byte *latencyTimer);
		status SetLnbPower(LnbPower lnbPower);
		status GetLnbPower(LnbPower *lnbPower);
		status SetLnbPowerWhenClose(LnbPower lnbPower);
		status GetLnbPowerWhenClose(LnbPower *lnbPower);
		status SetTunerPowerReset(TunerPowerReset tunerPowerReset);
		status GetTunerPowerReset(TunerPowerReset *tunerPowerReset);
		status InitTuner(uint tuner);
		status SetTunerSleep(uint tuner,ISDB isdb,bool sleep);
		status GetTunerSleep(uint tuner,ISDB isdb,bool *sleep);
		status GetCannelMax(uint tunerIndex,PT::Device::ISDB isdb,int *channelMax);
		status GetChannelInfo(uint tunerIndex,PT::Device::ISDB isdb,uint channel,int *pFrequency,char const **ppChName);
		status SetFrequency(uint tuner,ISDB isdb,uint  channel,int offset = 0);
		status GetFrequency(uint tuner,ISDB isdb,uint *channel,int *offset = NULL);
		status GetFrequencyOffset(uint tuner,ISDB isdb,int *clock,int *carrier);
		status GetCnAgc(uint tuner,ISDB isdb,uint *cn100,uint *currentAgc,uint *maxAgc);
		status SetIdS(uint tuner,uint id);
		status GetIdS(uint tuner,uint *id);
		status GetCorrectedErrorRate(uint tuner,ISDB isdb,LayerIndex layerIndex,ErrorRate *errorRate);
		status ResetCorrectedErrorCount(uint tuner,ISDB isdb);
		status GetErrorCount(uint tuner,ISDB isdb,uint *count);
		status GetTmccS(uint tuner,TmccS *tmcc);
		status GetLayerS(uint tuner,LayerS *layerS);
		status GetTmccT(uint tuner,TmccT *tmcc);
		status GetLockedT(uint tuner,bool locked[LAYER_COUNT_T]);
		status SetLayerEnable(uint tuner,ISDB isdb,LayerMask layerMask);
		status GetLayerEnable(uint tuner,ISDB isdb,LayerMask *layerMask);
		status SetBufferInfo(const BufferInfo *bufferInfo);
		status GetBufferInfo(BufferInfo *bufferInfo);
		status GetBufferPtr(uint index,void **ptr);
		status ResetTransferCounter();
		status IncrementTransferCounter();
		status SetStreamEnable(uint tuner,ISDB isdb,bool enable);
		status GetStreamEnable(uint tuner,ISDB isdb,bool *enable);
		status SetStreamGray(uint tuner,ISDB isdb,uint gray);
		status GetStreamGray(uint tuner,ISDB isdb,uint *gray);
		status SetTransferEnable(bool enable);
		status GetTransferEnable(bool *enable);
		status GetTransferInfo(TransferInfo *);

		status GetStreamData(uint tuner,ISDB isdb,void *ts,uint size,uint *outSize);
	};

	class Device2{};
}
}
#endif
