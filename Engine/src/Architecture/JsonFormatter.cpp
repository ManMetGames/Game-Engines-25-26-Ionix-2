#include "JsonFormatter.h"
#include <iostream>
#include <fstream>
#include <string_view>

namespace IonixEngine
{
	JsonFormatter::JsonFormatter(const std::string& filepath) :
		filepath(filepath)
	{
		OpenFile();
		if (!fileIsValid)
		{
			std::cout << "[JSON Formatter] Failed to find file!\nRun OpenFile() with a valid filepath." << std::endl;
		}

		charOverwrites.insert({'o',"a"});
	}

	bool JsonFormatter::OpenFile()
	{
		std::ifstream jsonFile;
		jsonFile.open(filepath);
		if (jsonFile.is_open())
		{
			fileContents << jsonFile.rdbuf();
			fileIsValid = true;
		}
		else
		{
			fileIsValid = false;
		}
		return fileIsValid;
	}

	void JsonFormatter::ProcessFile()
	{
		if (!fileIsValid)
		{
			std::cout << "[JSON Formatter] Given file is invalid, run again with a valid filepath." << std::endl;
			return;
		}
		std::string fileContentsCopy = fileContents.str();
		for (char character : fileContentsCopy)
		{
			//aka if charOverwrites has a key-value pair matching character
			if (charOverwrites.find(character) != charOverwrites.end())
			{
				std::cout << "[JSON Formatter] Char overwrite value found!" << std::endl;
			}
			else
			{
				continue;
			}
		}
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
		std::string fileContentsCopy = fileContents.str();
		for (char character : fileContentsCopy)
		{
			std::cout << character;
		}
		std::cout << std::endl;
	}



}