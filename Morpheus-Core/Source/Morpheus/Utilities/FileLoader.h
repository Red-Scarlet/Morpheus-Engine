#pragma once

#include "Morpheus/Core/Common.h"

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
	};

}