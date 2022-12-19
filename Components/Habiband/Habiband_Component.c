//==============================================================================
#include "Habiband_Component.h"
#include "Habiband_BLEAdapter.h"
//==============================================================================
HabibandT Habiband;
//==============================================================================
/**
 * @brief main handler
 */
inline void HabibandComponentHandler()
{
	HabibandBootloaderHandler();
	HabibandControlHandler(&Habiband);
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
inline void HabibandComponentTimeSynchronization()
{
	HabibandBootloaderTimeSynchronization();
	HabibandTimeSynchronization(&Habiband);
}
//==============================================================================
/**
 * @brief initializing the component
 * @param parent binding to the parent object
 * @return int
 */
int HabibandComponentInit(void* parent)
{
	int result = Habiband_BLEAdapterInit(&Habiband);
	result |= HabibandInit(&Habiband, parent);

	return result;
}
//==============================================================================
