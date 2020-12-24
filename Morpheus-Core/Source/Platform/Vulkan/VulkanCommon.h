#pragma once

#include <vulkan/vulkan.h>
#include "Morpheus/Mathematics/Mathematics.h"

#define VULKAN_CORE_LOGGING
#define VULKAN_CORE_LOG_EXT

//#define VULKAN_CORE_LOG_POINTERS
//#define VULKAN_CORE_LOG_COMMANDS
//#define VULKAN_CORE_LOG_BINDINGS


#ifdef VULKAN_CORE_LOGGING
	#define VULKAN_CORE_TRACE(...)					MORP_CORE_TRACE(__VA_ARGS__)
	#define VULKAN_CORE_INFO(...)					MORP_CORE_INFO(__VA_ARGS__)
	#define VULKAN_CORE_WARN(...)					MORP_CORE_WARN(__VA_ARGS__)
	#define VULKAN_CORE_ERROR(...)					MORP_CORE_ERROR(__VA_ARGS__)
	#define VULKAN_CORE_SPECIAL(...)				MORP_CORE_SPECIAL(__VA_ARGS__)
	#define VULKAN_CORE_SPECIAL_2(...)				MORP_CORE_SPECIAL_2(__VA_ARGS__)

	#ifdef VULKAN_CORE_LOG_POINTERS
		#define VULKAN_CORE_DEBUG_PTR(...)			VULKAN_CORE_SPECIAL("[VULKAN] Memory Address: " + Morpheus::ReadAddress(__VA_ARGS__))
	#else
		#define VULKAN_CORE_DEBUG_PTR(...)
	#endif

	#ifdef VULKAN_CORE_LOG_COMMANDS
		#define VULKAN_CORE_DEBUG_CMD(VAR, VAR_2)	VULKAN_CORE_SPECIAL("[VULKAN] Command Call: " + Morpheus::GetString(VAR) + " " + Morpheus::GetString(VAR_2))
	#else
		#define VULKAN_CORE_DEBUG_CMD(VAR, VAR_2)
	#endif

	#ifdef VULKAN_CORE_LOG_BINDINGS
		#define VULKAN_CORE_DEBUG_BOUND(VAR)		VULKAN_CORE_SPECIAL("[VULKAN] Bounded ID: " + Morpheus::GetString(VAR))
	#else
		#define VULKAN_CORE_DEBUG_BOUND(VAR)
	#endif
	
#else
	#define VULKAN_CORE_TRACE(...)
	#define VULKAN_CORE_INFO(...)
	#define VULKAN_CORE_WARN(...)
	#define VULKAN_CORE_ERROR(...)
	#define VULKAN_CORE_SPECIAL(...)
	#define VULKAN_CORE_SPECIAL_2(...)
	#define VULKAN_CORE_DEBUG_PTR(...)
	#define VULKAN_CORE_DEBUG_CMD(VAR, VAR_2)
	#define VULKAN_CORE_DEBUG_BOUND(VAR)
#endif

