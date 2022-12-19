//==============================================================================
#include "Bootloader_Component.h"
#include "Bootloader/Adapters/Bootloader_Adapter.h"
//==============================================================================
/**
 * @brief common handler
 */
inline void BootloaderComponentHandler()
{
	BootloaderHandler();
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
inline void BootloaderComponentTimeSynchronization()
{
	BootloaderTimeSynchronization();
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the component
 * @param parent binding to the parent object
 * @return int 
 */
int BootloaderComponentInit(void* parent)
{
	xResult result = xResultAccept;
	result |= BootloaderAdapterInit(&Bootloader, parent) != xResultAccept;
	result |= BootloaderInit(parent) != xResultAccept;

	return result;
}
//==============================================================================
