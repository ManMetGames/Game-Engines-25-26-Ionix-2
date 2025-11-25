#include "JsonFormatter.h"
#include <iostream>
#include <fstream>
#include <string_view>

namespace IonixEngine
{
	JsonFormatter::JsonFormatter(const std::string& filepath)
	{
		this->filepath = filepath;
		if (!OpenFile())
		{
			std::cout << "[JSON Formatter] Failed to find file!\nRun OpenFile() with a valid filepath." << std::endl;
		}
	}

	bool JsonFormatter::OpenFile()
	{
		std::ifstream jsonFile;
		jsonFile.open(filepath);
		if (jsonFile.is_open())
		{
			char character;
			while (jsonFile.get(character))
			{
				fileContents.push_back(character);
			}
			return true;
		}
		else { return false; }
	}

	std::string JsonFormatter::GetFilepath()
	{
		return filepath;
	}

	void JsonFormatter::SetFilepath(const std::string& newFilepath)
	{
		filepath = newFilepath;
		OpenFile();
	}

	void JsonFormatter::DebugLogFileContents()
	{
		std::cout << "[JSON Formatter] Logging file contents:" << std::endl;
		for (char character : fileContents)
		{
			std::cout << character;
		}
		std::cout << std::endl;
	}



}