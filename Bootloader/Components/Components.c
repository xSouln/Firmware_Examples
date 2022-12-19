//==============================================================================
#include "Components.h"
//==============================================================================
/**
 * @brief common handler
 */
inline void ComponentsHandler()
{
	HabibandBootloaderHandler();
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
inline void ComponentsTimeSynchronization()
{
	HabibandBootloaderTimeSynchronization();
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the component
 * @param parent binding to the parent object
 * @return int 
 */
int ComponentsInit(void* parent)
{

	return 0;
}
//==============================================================================
