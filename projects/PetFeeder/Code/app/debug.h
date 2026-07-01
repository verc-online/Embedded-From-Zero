/*
* debug.h
*
* Created: 13.06.2026 12:00:39
*  Author: HP
*/


#ifndef DEBUG_H_
#define DEBUG_H_

#include "../config/config.h"

#define DEBUG_ENABLED 1

#if DEBUG_ENABLED
void Debug_Log(const char *message);

#define  DEBUG_LOG(msg) Debug_Log(msg)

#else 
#define DEBUG_LOG(msg)

#endif // DEBUG_ENABLED


#endif /* DEBUG_H_ */