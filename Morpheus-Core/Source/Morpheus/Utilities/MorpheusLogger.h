#pragma once

#include "MultithreadedLog.h"
#include "Morpheus/Utilities/Instrumentor.h"

namespace Morpheus {

	//COLORS LIST
	//1: Blue
	//2: Green
	//3: Cyan
	//4: Red
	//5: Purple
	//6: Yellow (Dark)
	//7: Default white
	//8: Gray/Grey
	//9: Bright blue
	//10: Brigth green
	//11: Bright cyan
	//12: Bright red
	//13: Pink/Magenta
	//14: Yellow
	//15: Bright white
	//Numbers after 15 include background colors

	class MorpheusLogger
	{
	public:
		static void LogError(const std::string& _Message)
		{
			s_SystemLogger << "�12" << _Message << std::endl;
		}

		static void LogWarn(const std::string& _Message)
		{
			s_SystemLogger << "�14" << "[MORPHEUS] " << _Message << std::endl;
		}

		static void LogInfo(const std::string& _Message)
		{
			s_SystemLogger << "�15" << "[MORPHEUS] " << _Message << std::endl;
		}

		static void LogTrace(const std::string& _Message)
		{
			s_SystemLogger << "�09" << "[MORPHEUS] " << _Message << std::endl;
		}

		static void LogSpecial(const std::string& _Message)
		{
			s_SystemLogger << "�13" << "[MORPHEUS] " << _Message << std::endl;
		}

		static void LogSpecialOverride(const std::string& _Message)
		{
			s_SystemLogger << "�02" << "[MORPHEUS] " << _Message << std::endl;
		}

		static void LogNormal(const std::string& _Message)
		{
			s_SystemLogger << "�07" << "[MORPHEUS] " << _Message << std::endl;
		}

	private:
		static Log s_SystemLogger;
	};

}

#ifdef MORP_LOGGING
	#define MORP_CORE_TRACE(...)		::Morpheus::MorpheusLogger::LogTrace(__VA_ARGS__)
	#define MORP_CORE_INFO(...)			::Morpheus::MorpheusLogger::LogInfo(__VA_ARGS__)
	#define MORP_CORE_WARN(...)			::Morpheus::MorpheusLogger::LogWarn(__VA_ARGS__)
	#define MORP_CORE_ERROR(...)		::Morpheus::MorpheusLogger::LogError(__VA_ARGS__)
	#define MORP_CORE_SPECIAL(...)		::Morpheus::MorpheusLogger::LogSpecial(__VA_ARGS__)
	#define MORP_CORE_SPECIAL_2(...)	::Morpheus::MorpheusLogger::LogSpecialOverride(__VA_ARGS__)
#else
	#define MORP_CORE_TRACE(...)		
	#define MORP_CORE_INFO(...)			
	#define MORP_CORE_WARN(...)		
	#define MORP_CORE_ERROR(...)	
	#define MORP_CORE_SPECIAL(...)	
	#define MORP_CORE_SPECIAL_2(...)
#endif