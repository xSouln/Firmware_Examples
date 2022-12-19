//==============================================================================
#include "Components.h"
//==============================================================================
/**
 * @brief common handler
 */
inline void ComponentsHandler()
{
	HabibandComponentHandler();
}
//------------------------------------------------------------------------------
/**
 * @brief time synchronization of time-dependent processes
 */
inline void ComponentsTimeSynchronization()
{
	HabibandComponentTimeSynchronization();
}
//------------------------------------------------------------------------------
/**
 * @brief initializing the component
 * @param parent binding to the parent object
 * @return int 
 */
int ComponentsInit(void* parent)
{
	HabibandComponentInit(parent);

	return 0;
}
//==============================================================================
