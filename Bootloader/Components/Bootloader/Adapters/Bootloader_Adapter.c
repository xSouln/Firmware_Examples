//==============================================================================
#include "Bootloader_Adapter.h"
#include "Bootloader/Bootloader.h"
#include "stm32wbxx_hal_flash_ex.h"
#include "wwdg.h"
#include "shci.h"
//==============================================================================
extern void BLEAppResetDevice();
//==============================================================================
static xResult PrivateBootloaderRequestListener(BootloaderT* device, BootloaderRequestSelector event, uint32_t args, uint32_t count)
{
	switch((uint8_t)event)
	{
		case BootloaderRequestUnLockMemory:
			if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) != 0U)
			{
				return HAL_FLASH_Unlock() == HAL_OK ? xResultAccept : xResultError;
			}
			return xResultAccept;

		case BootloaderRequestLockMemory:
			if (READ_BIT(FLASH->CR, FLASH_CR_LOCK) == 0U)
			{
				return HAL_FLASH_Lock() == HAL_OK ? xResultAccept : xResultError;
			}
			return xResultAccept;

		case BootloaderRequestSystemReset:
			MX_WWDG_Init();
			while (true) { }
			NVIC_SystemReset();
			break;

		case BootloaderRequestMoveIRQVector:
			__set_PRIMASK(1);
			SCB->VTOR = (args);
			__set_PRIMASK(0);
			break;

		case BootloaderRequestEnableIRQ:
			__enable_irq();
			break;

		case BootloaderRequestDisableIRQ:
			__disable_irq();
			break;

		case BootloaderRequestDelay:
			HAL_Delay(args);
			break;

		case BootloaderRequestDisposeSystem:
			break;

		case BootloaderRequestSetJumpAddress:
			break;

		default: return xResultNotSupported;
	}
	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateBootloaderGetValueAction(BootloaderT* device, BootloaderValueSelector event, uint32_t* value)
{
	if (!value) { return xResultInvalidParameter; }

	switch((uint8_t)event)
	{

		default: return xResultNotSupported;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateBootloaderSetValueAction(BootloaderT* device, BootloaderValueSelector event, uint32_t value)
{
	switch((uint8_t)event)
	{

		default: return xResultNotSupported;
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateBootloaderReadAction(BootloaderT* device, uint32_t start_address, uint8_t* data, uint32_t size, uint32_t mode)
{
	volatile uint8_t* mem = (volatile uint8_t*)start_address;

	for (uint32_t i = 0; i < size; i++)
	{
		data[i] = mem[i];
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateBootloaderWriteAction(BootloaderT* device, uint32_t address, uint8_t* data, uint32_t size, uint32_t mode)
{
	typedef union
	{
		uint8_t Data[sizeof(uint64_t)];
		uint64_t Value;

	} DWordT;

	DWordT dword;

	uint32_t i = 0;
	uint8_t byte_number;
	while (i < size)
	{
		dword.Value = 0xffffffffffffffff;
		byte_number = 0;

		while (i < size && byte_number < sizeof(DWordT))
		{
			dword.Data[byte_number++] = data[i];
			i++;
		}

		FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

		/* Set PG bit */
		SET_BIT(FLASH->CR, FLASH_CR_PG);

		/* Program first word */
		*(uint32_t *)address = (uint32_t)dword.Value;

		/* Barrier to ensure programming is performed in 2 steps, in right order
		(independently of compiler optimization behavior) */
		__ISB();

		/* Program second word */
		*(uint32_t *)(address + 4U) = (uint32_t)(dword.Value >> 32U);

		//*(volatile uint64_t*)address = dword.Value;

		FLASH_WaitForLastOperation(FLASH_TIMEOUT_VALUE);

		CLEAR_BIT(FLASH->CR, FLASH_TYPEPROGRAM_DOUBLEWORD);

		//HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, dword.Value);

		address += byte_number;
		//HAL_FLASH_Program(FLASH_TYPEPROGRAM_DOUBLEWORD, address, dword.Value);
	}

	return xResultAccept;
}
//------------------------------------------------------------------------------
static xResult PrivateBootloaderEraseAction(BootloaderT* device, uint32_t start_address, uint32_t end_address, uint32_t mode)
{
	uint32_t page_error;
	uint32_t size;

	device->TimeOut = device->Options.TimeOut;

	if (start_address >= end_address)
	{
		return xResultInvalidParameter;
	}

	if (start_address < BOOTLOADER_FLASH_MEMORY_START_ADDRESS
	|| end_address > BOOTLOADER_FLASH_MEMORY_END_ADDRESS)
	{
		return xResultOutOfRange;
	}

	start_address -= BOOTLOADER_FLASH_MEMORY_START_ADDRESS;
	end_address -= BOOTLOADER_FLASH_MEMORY_START_ADDRESS;

	size = end_address - start_address;

	if (size > BOOTLOADER_FLASH_MEMORY_SIZE)
	{
		return xResultOutOfRange;
	}

	FLASH_EraseInitTypeDef erase_request =
	{
		.Page = (start_address) / FLASH_PAGE_SIZE,
		.NbPages = (size / FLASH_PAGE_SIZE) + 1,
		.TypeErase = FLASH_TYPEERASE_PAGES
	};

	if (Bootloader.SystemIsInit) { SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_ON); }
	xResult result = HAL_FLASHEx_Erase(&erase_request, &page_error);
	if (Bootloader.SystemIsInit) { SHCI_C2_FLASH_EraseActivity(ERASE_ACTIVITY_OFF); }

	switch ((uint8_t)result)
	{
		case HAL_OK: return xResultAccept;
		case HAL_BUSY: return xResultBusy;
		case HAL_TIMEOUT: return xResultTimeOut;
	}

	return xResultError;
}
//==============================================================================
/**
 * @brief adaptation of the abstract control layer for low-level functions and registers
 * @param device 
 * @param adapter adapter data
 * @return xResult 
 */
xResult BootloaderAdapterInit(BootloaderT* device, BootloaderAdapterT* adapter)
{
	if (device && adapter)
	{
		device->Adapter.Description = "BootloaderAdapterT";
		device->Adapter.Parent = device;
		device->Adapter.Child = adapter;

		device->Adapter.RequestListener = (BootloaderRequestListenerT)PrivateBootloaderRequestListener;
		device->Adapter.GetValue = (BootloaderGetValueActionT)PrivateBootloaderGetValueAction;
		device->Adapter.SetValue = (BootloaderSetValueActionT)PrivateBootloaderSetValueAction;

		device->Adapter.Read = (BootloaderReadActionT)PrivateBootloaderReadAction;
		device->Adapter.Write = (BootloaderWriteActionT)PrivateBootloaderWriteAction;
		device->Adapter.Erase = (BootloaderEraseActionT)PrivateBootloaderEraseAction;

		return xResultAccept;
	}
	return xResultLinkError;
}
//==============================================================================
