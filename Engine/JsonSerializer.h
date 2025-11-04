#pragma once
#include <string>
#include <stack>
#include <iostream>
#include <fstream>

namespace IonixEngine
{
	class JsonSerializer
	{
	private:
		int indentLevel;
		int indentSize;
		std::stack<std::string> indents;
		char* finalJsonFile;
		std::string filePath;
		std::string fileName;

	public:
		JsonSerializer(std::string filepath, std::string filename, int indentSize = 4);
		std::string GetFilePath();
		void SetFilePath(string newFilepath);
	};
}
