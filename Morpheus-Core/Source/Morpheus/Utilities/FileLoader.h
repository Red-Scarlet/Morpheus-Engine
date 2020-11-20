#pragma once

#include "Morpheus/Core/Common.h"

#include <fstream>

namespace Morpheus {

	struct FileLoader
	{
	public:
		static String ReadFile(const char* filepath)
		{
			FILE* file = fopen(filepath, "r");
			fseek(file, 0, SEEK_END);
			unsigned long length = ftell(file);
			char* data = new char[length + 1];
			memset(data, 0, length + 1);
			fseek(file, 0, SEEK_SET);
			fread(data, 1, length, file);
			fclose(file);

			String result(data);
			delete[] data;
			return result;
		}


		static Vector<float8> ReadFile(const String& _Filepath)
		{
			std::ifstream file(_Filepath, std::ios::ate | std::ios::binary);

			if (!file.is_open())
				MORP_CORE_ASSERT(MORP_ERROR, "[FILE UTILS] Failed to open File: " + _Filepath + "!");

			uint32 fileSize = (uint32)file.tellg();
			Vector<float8> buffer(fileSize);
			file.seekg(0);
			file.read(buffer.data(), fileSize);
			file.close();

			return buffer;
		}


	};

}