//==============================================================================
#ifndef _HABIBAND_COMPONENT_H
#define _HABIBAND_COMPONENT_H
//------------------------------------------------------------------------------
#ifdef __cplusplus
 extern "C" {
#endif 
//==============================================================================
//includes:

#include "Habiband_Control.h"
//==============================================================================
//defines:

extern HabibandT Habiband;
//==============================================================================
//functions:

int HabibandComponentInit(void* parent);

extern inline void HabibandComponentTimeSynchronization();
extern inline void HabibandComponentHandler();
//==============================================================================
#ifdef __cplusplus
}
#endif
//------------------------------------------------------------------------------
#endif

