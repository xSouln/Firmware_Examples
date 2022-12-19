//==============================================================================
#include "Bootloader.h"
//==============================================================================
BootloaderT Bootloader;
//==============================================================================
/**
 * @brief common handler
 */
void BootloaderHandler()
{

}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
void BootloaderTimeSynchronization()
{
	if (Bootloader.TimeOut)
	{
		Bootloader.TimeOut--;
	}
}
//------------------------------------------------------------------------------
/**
 * @brief unlocks memory for write and erase
 * @return xResult 
 */
xResult BootloaderUnLockMemory()
{
	Bootloader.Status.Operation = BootloaderOperationUnLockMemory;
	Bootloader.Status.Result = Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestUnLockMemory, 0, 0);

	return Bootloader.Status.Result;
}
//------------------------------------------------------------------------------
/**
 * @brief locks memory for write and erase
 * @return xResult 
 */
xResult BootloaderLockMemory()
{
	Bootloader.Status.Operation = BootloaderOperationLockMemory;
	Bootloader.Status.Result = Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestLockMemory, 0, 0);

	return Bootloader.Status.Result;
}
//------------------------------------------------------------------------------
/**
 * @brief erases memory
 * @param start_address must be in the range of
 * BOOTLOADER_FLASH_MEMORY_START_ADDRESS <= starting address < BOOTLOADER_FLASH_MEMORY_END_ADDRESS
 * @param end_address must be in the range of end_address < BOOTLOADER_FLASH_MEMORY_END_ADDRESS
 * @return xResult 
 */
xResult BootloaderErase(uint32_t start_address, uint32_t end_address)
{
	Bootloader.Status.Operation = BootloaderOperationErase;
	Bootloader.Status.Result = Bootloader.Adapter.Erase(&Bootloader, start_address, end_address, 0);

	return Bootloader.Status.Result;
}
//------------------------------------------------------------------------------
/**
 * @brief reads data from flash
 * @param start_address must be in the range of
 * BOOTLOADER_FLASH_MEMORY_START_ADDRESS <= starting address < BOOTLOADER_FLASH_MEMORY_END_ADDRESS
 * @param memory 
 * @param size 
 * @return xResult 
 */
xResult BootloaderRead(uint32_t start_address, uint8_t* memory, uint32_t size)
{
	Bootloader.Status.Operation = BootloaderOperationRead;
	Bootloader.Status.Result = Bootloader.Adapter.Read(&Bootloader, start_address, memory, size, 0);

	return Bootloader.Status.Result;
}
//------------------------------------------------------------------------------
/**
 * @brief writing data to the flash
 * @param start_address must be in the range of
 * BOOTLOADER_FLASH_MEMORY_START_ADDRESS <= starting address < BOOTLOADER_FLASH_MEMORY_END_ADDRESS
 * @param memory 
 * @param size 
 * @return xResult 
 */
xResult BootloaderWrite(uint32_t start_address, uint8_t* memory, uint32_t size)
{
	Bootloader.Status.Operation = BootloaderOperationWrite;
	Bootloader.Status.Result = Bootloader.Adapter.Write(&Bootloader, start_address, memory, size, 0);

	return Bootloader.Status.Result;
}
//------------------------------------------------------------------------------
/**
 * @brief transfer of the interrupt vector
 * @param address 
 * @return xResult 
 */
xResult BootloaderMoveIRQVector(uint32_t address)
{
	return Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestMoveIRQVector, address, 0);
}
//------------------------------------------------------------------------------
/**
 * @brief disable interrupts
 * @return xResult 
 */
xResult BootloaderDisableIRQ()
{
	return Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestDisableIRQ, 0, 0);
}
//------------------------------------------------------------------------------
/**
 * @brief enable interrupts
 * @return xResult 
 */
xResult BootloaderEnableIRQ()
{
	return Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestEnableIRQ, 0, 0);
}
//------------------------------------------------------------------------------
/**
 * @brief system reset
 */
void BootloaderSystemReset()
{
	Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestSystemReset, 0, 0);
}
//------------------------------------------------------------------------------
/**
 * @brief saving a remark in memory
 * @param remark 
 * @return xResult 
 */
static xResult BootloaderSaveRemark(BootloaderRemarkT* remark)
{
	BootloaderUnLockMemory();
	remark->Key = BOOTLOADER_REMARK_KEY;
	BootloaderErase(BOOTLOADER_REMARK_ADDRESS, BOOTLOADER_REMARK_ADDRESS + sizeof(BootloaderRemarkT));
	BootloaderWrite(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)remark, sizeof(BootloaderRemarkT));
	BootloaderLockMemory();

	BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateBootloaderStartApp()
{
	if (Bootloader.InBoot)
	{
		uint32_t main_address = (*(uint32_t*)(BOOTLOADER_APP_START_ADDRESS + 4));
		void (*main)() = (void (*)())main_address;

		if (main_address != -1 && main_address)
		{
			BootloaderDisableIRQ();
			Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestDisposeSystem, BOOTLOADER_APP_START_ADDRESS, 0);
			Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestSetJumpAddress, BOOTLOADER_APP_START_ADDRESS, 0);
			main();

			BootloaderSystemReset();

			return xResultError;
		}

		return xResultInvalidParameter;
	}

	return xResultOperationNotAvailable;
}
//------------------------------------------------------------------------------
/**
 * @brief attempt to navigate to the application at BOOTLOADER_APP_START_ADDRESS
 * @return xResult 
 */
xResult BootloaderRequestStartApp()
{
	if (Bootloader.FirmwareLoadingInit)
	{
		return xResultBusy;
	}

	if (Bootloader.InBoot)
	{

		BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));

		Bootloader.Remark.Error.Value = 0;
		Bootloader.Remark.State.Value = 0;

		if (BootloaderSaveRemark(&Bootloader.Remark) == xResultAccept)
		{
			BootloaderSystemReset();
		}

		return xResultError;
	}

	return xResultOperationNotAvailable;
}
//------------------------------------------------------------------------------
/**
 * @brief reset saved errors
 * @return xResult
 */
xResult BootloaderResetFirmwareError()
{
	if (!Bootloader.FirmwareLoadingInit)
	{
		BootloaderRemarkT remark;

		BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&remark, sizeof(remark));
		remark.Error.Value = 0;

		BootloaderSaveRemark(&remark);

		return xResultAccept;
	}

	return xResultBusy;
}
//------------------------------------------------------------------------------
/**
 * @brief attempt to switch to bootloader
 * @return xResult 
 */
xResult BootloaderStartBoot()
{
	if (Bootloader.FirmwareLoadingInit)
	{
		return xResultBusy;
	}

	if (!Bootloader.InBoot)
	{
		BootloaderRemarkT remark;

		BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&remark, sizeof(remark));
		remark.State.BootIsEnabled = true;

		BootloaderSaveRemark(&remark);

		BootloaderDisableIRQ();
		Bootloader.Adapter.RequestListener(&Bootloader, BootloaderRequestDisposeSystem, BOOTLOADER_APP_START_ADDRESS, 0);
		BootloaderSystemReset();

		return xResultError;
	}

	return xResultOperationNotAvailable;
}
//------------------------------------------------------------------------------
/**
 * @brief initializes the firmware download
 * @param size image size
 * @param crc CRC of the image
 * @return xResult 
 */
xResult BootloaderInitLoadFirmware(uint32_t size, uint32_t crc)
{
	xResult result = xResultError;

	if (!Bootloader.InBoot)
	{
		return xResultOperationNotAvailable;
	}

	if (Bootloader.FirmwareLoadingInit)
	{
		return xResultBusy;
	}

	if (size == 0 || crc == 0)
	{
		return xResultInvalidParameter;
	}

	Bootloader.FirmwareLoadingInit = true;

	BootloaderUnLockMemory();

	//Bootloader.FirmwareStartAddress = BOOTLOADER_APP_START_ADDRESS;
	//Bootloader.FirmwareStopAddress = BOOTLOADER_APP_START_ADDRESS + size;
	Bootloader.FirmwareSize = size;
	Bootloader.FirmwareCrc = crc;
	Bootloader.FirmwareLoadedSize = 0;
	Bootloader.Crc = 0;

	//BootloaderDisableIRQ();
	result = BootloaderErase(BOOTLOADER_APP_START_ADDRESS, BOOTLOADER_APP_START_ADDRESS + size);
	//BootloaderEnableIRQ();

	return result;
}
//------------------------------------------------------------------------------
/**
 * @brief canceling the firmware download
 * @return xResult 
 */
xResult BootloaderCancelLoadFirmware()
{
	if (!Bootloader.InBoot)
	{
		return xResultOperationNotAvailable;
	}

	if (Bootloader.FirmwareLoadingInit)
	{
		BootloaderLockMemory();

		Bootloader.FirmwareLoadingInit = false;
	}
	return xResultAccept;
}
//------------------------------------------------------------------------------
/**
 * @brief writes part of the image to the flash
 * @param memory
 * @param size
 * @return xResult 
 */
xResult BootloaderLoad(uint8_t* memory, uint32_t size)
{
	if (!Bootloader.FirmwareLoadingInit)
	{
		return xResultError;
	}

	for (int i = 0; i < size; i++)
	{
		Bootloader.Crc += memory[i];
	}

	Bootloader.Status.Operation = BootloaderOperationLoad;
	Bootloader.Status.Result = Bootloader.Adapter.Write(&Bootloader,
			BOOTLOADER_APP_START_ADDRESS + Bootloader.FirmwareLoadedSize,
			memory,
			size,
			0);

	Bootloader.FirmwareLoadedSize += size;

	return Bootloader.Status.Result;
}
//------------------------------------------------------------------------------
/**
 * @brief called at the end of the firmware download
 * @return xResult 
 */
xResult BootloaderConfirmLoadFirmware()
{
	xResult result = xResultError;

	if (!Bootloader.InBoot)
	{
		return xResultOperationNotAvailable;
	}

	if (Bootloader.FirmwareLoadingInit)
	{
		if (Bootloader.FirmwareSize != 0
		&& Bootloader.FirmwareLoadedSize == Bootloader.FirmwareSize
		&& Bootloader.Crc == Bootloader.FirmwareCrc)
		{
			//uint32_t firmware_size = Bootloader.FirmwareStopAddress - Bootloader.FirmwareStartAddress;
			Bootloader.AppSectionCrc = BootloaderCalculateMemoryCrc(BOOTLOADER_APP_START_ADDRESS, Bootloader.FirmwareSize);

			if (Bootloader.AppSectionCrc == Bootloader.Crc)
			{
				BootloaderRemarkT remark;

				BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&remark, sizeof(remark));

				Bootloader.Remark.Crc = Bootloader.Crc;
				Bootloader.Remark.Key = BOOTLOADER_REMARK_CONFIRMATION_KEY;
				Bootloader.Remark.FirmwareSize = Bootloader.FirmwareSize;
				Bootloader.Remark.State.Value = 0;
				Bootloader.Remark.Error.Value = 0;

				BootloaderErase(BOOTLOADER_REMARK_ADDRESS, BOOTLOADER_REMARK_ADDRESS + sizeof(BootloaderRemarkT));
				BootloaderWrite(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));
				BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));
				//BootloaderSaveRemark(&remark);

				result = xResultAccept;
			}
		}

		BootloaderLockMemory();

		Bootloader.FirmwareLoadingInit = false;
	}

	return result;
}
//------------------------------------------------------------------------------
/**
 * @brief reads data from memory and calculates the CRC of the read data
 * @param start_address must be in the range of
 * BOOTLOADER_FLASH_MEMORY_START_ADDRESS <= starting address < BOOTLOADER_FLASH_MEMORY_END_ADDRESS
 * @param memory_size 
 * @return uint32_t 
 */
uint32_t BootloaderCalculateMemoryCrc(uint32_t start_address, uint32_t memory_size)
{
	uint32_t crc = 0;

	if (start_address < BOOTLOADER_FLASH_MEMORY_START_ADDRESS
	|| start_address > BOOTLOADER_FLASH_MEMORY_END_ADDRESS
	|| memory_size > BOOTLOADER_FLASH_MEMORY_SIZE
	|| ((BOOTLOADER_FLASH_MEMORY_END_ADDRESS - start_address) < memory_size))
	{
		return -1;
	}

	volatile uint8_t* mem = (volatile uint8_t*)start_address;

	for (uint32_t i = 0; i < memory_size; i++)
	{
		crc += mem[i];
	}

	return crc;
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the input from the application
 */
void BootloaderInitAppInput()
{
	Bootloader.InBoot = false;

	BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));
	Bootloader.AppSectionCrc = BootloaderCalculateMemoryCrc(BOOTLOADER_APP_START_ADDRESS, Bootloader.Remark.FirmwareSize);

	BootloaderMoveIRQVector(BOOTLOADER_APP_START_ADDRESS);
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the input from the bootloader
 */
void BootloaderInitBootInput()
{
	uint8_t start_app;

	Bootloader.InBoot = true;
	BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));

	Bootloader.AppSectionCrc = BootloaderCalculateMemoryCrc(BOOTLOADER_APP_START_ADDRESS, Bootloader.Remark.FirmwareSize);
	Bootloader.AppCrcError = Bootloader.AppSectionCrc == -1 || Bootloader.Remark.Crc != Bootloader.AppSectionCrc;
	Bootloader.RemarkKeyError = Bootloader.Remark.Key != BOOTLOADER_REMARK_KEY;

	start_app = !Bootloader.Remark.State.BootIsEnabled
				&& !Bootloader.RemarkKeyError
				&& !Bootloader.AppCrcError
				&& !(Bootloader.Remark.Error.Value & Bootloader.Remark.ErrorMask.Value);

	if (Bootloader.Remark.State.BootIsEnabled)
	{
		Bootloader.Remark.State.BootIsEnabled = false;
		BootloaderSaveRemark(&Bootloader.Remark);
	}

	if (start_app)
	{
		PrivateBootloaderStartApp();
	}
}
//------------------------------------------------------------------------------
/**
 * @brief called at HardFaultError
 */
void BootloaderHardFaultError()
{
	BootloaderRead(BOOTLOADER_REMARK_ADDRESS, (uint8_t*)&Bootloader.Remark, sizeof(Bootloader.Remark));
	Bootloader.Remark.Error.AppStartingHardFaultError = true;

	BootloaderSaveRemark(&Bootloader.Remark);
	BootloaderSystemReset();
}
//------------------------------------------------------------------------------
/**
 * @brief called upon completion of initialization of the microcontroller
 */
void BootloaderInitSystemEnd()
{
	Bootloader.SystemIsInit = true;
}
//==============================================================================
/**
 * @brief initializes bootloader controls
 * initially, it is recommended to call the initialization of the adapter
 * @param parent binding to the parent object
 * @return xResult 
 */
xResult BootloaderInit(void* parent)
{
	if (!Bootloader.IsInit)
	{
		Bootloader.IsInit = true;

		Bootloader.Description = "BootloaderT";
		Bootloader.Parent = parent;
		Bootloader.Options.TimeOut = BOOTLOADER_OPERATION_TIMEOUT;

		Bootloader.Info.AppStartAddress = BOOTLOADER_APP_START_ADDRESS;
		Bootloader.Info.AppEndAddress = BOOTLOADER_FLASH_MEMORY_END_ADDRESS;

		Bootloader.Info.BootloaderStartAddress = BOOTLOADER_BOOT_START_ADDRESS;
		Bootloader.Info.BootloaderEndAddres = BOOTLOADER_REMARK_ADDRESS;
	}

	return xResultAccept;
}
//==============================================================================
