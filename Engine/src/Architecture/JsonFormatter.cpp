#include "JsonFormatter.h"
#include <iostream>
#include <fstream>
#include <string_view>

namespace IonixEngine
{
	void JsonFormatter::writeToFile()
	{
		std::ofstream jsonFile;
		jsonFile.open(filepath);
		for (auto strView : reconstructedFile)
		{
			jsonFile << strView;
		}
		jsonFile.close();
	}

	JsonFormatter::JsonFormatter(const std::string& filepath) :
		filepath(filepath)
	{
		OpenFile();
		if (!fileIsValid)
		{
			std::cout << "[JSON Formatter] Failed to find file!\nRun OpenFile() with a valid filepath." << std::endl;
		}

		//find better way to allow for character overwriting, maybe structs?
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
			jsonFile.close();
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
		int index = 0;
		int lastOverwrite = 0;
		for (char character : fileContentsCopy)
		{
			//aka if charOverwrites has a key-value pair matching character
			if (charOverwrites.find(character) != charOverwrites.end())
			{
				std::cout << "[JSON Formatter] Char overwrite value found!" << std::endl;
				char* charPos = &fileContentsCopy.at(lastOverwrite);
				size_t viewSize = index - lastOverwrite;
				std::string_view unreplacedSection{ charPos,viewSize };
				std::string_view replacement{ charOverwrites[character] };

				reconstructedFile.push_back(unreplacedSection);
				reconstructedFile.push_back(replacement);

				lastOverwrite = index + 1;
			}
			index++;
			continue;
		}
		char* charPosition = &fileContentsCopy.at(lastOverwrite);
		size_t viewSize = index - lastOverwrite;
		std::string_view finalSection{ charPosition,viewSize };
		reconstructedFile.push_back(finalSection);

		//debug purposes
		for (auto stringview : reconstructedFile)
		{
			std::cout << stringview;
		}
		std::cout << std::endl;

		writeToFile();
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