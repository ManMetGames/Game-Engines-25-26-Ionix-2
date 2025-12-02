#include "JsonFormatter.h"
#include <iostream>
#include <fstream>
#include <string_view>

namespace IonixEngine
{
	std::string JsonFormatter::processFile()
	{
		if (!fileIsValid)
		{
			std::cout << "[JSON Formatter] Given file is invalid, run again with a valid filepath." << std::endl;
			return "";
		}
		std::string fileContentsCopy = fileContents.str();
		int index = 0;
		int lastOverwrite = 0;
		std::stringstream output;
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

				output << unreplacedSection;
				output << replacement;

				lastOverwrite = index + 1;
			}
			index++;
			continue;
		}
		char* charPosition = &fileContentsCopy.at(lastOverwrite);
		size_t viewSize = index - lastOverwrite;
		std::string_view finalSection{ charPosition,viewSize };
		reconstructedFile.push_back(finalSection);
		output << finalSection;

		return output.str();
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
		charOverwrites.insert({'a',"o"});
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

	void JsonFormatter::WriteToFile()
	{
		processFile();
		std::ofstream jsonFile;
		jsonFile.open(filepath);
		jsonFile << processFile();
		jsonFile.close();
		return;
	}

	std::string JsonFormatter::ExportString()
	{
		std::cout << processFile() << std::endl;
		return processFile();
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