#include "JsonFormatter.h"
#include <iostream>
#include <fstream>
#include <string_view>

namespace IonixEngine
{
	std::string JsonFormatter::processFile()
	{
		if (!inputValid)
		{
			std::cout << "[JSON Formatter] Given file is invalid, run again with a valid filepath." << std::endl;
			return "";
		}
		std::string fileContentsCopy = inputString.str();
		size_t index = 0;
		size_t lastOverwrite = 0;
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

				output << unreplacedSection;
				output << replacement;

				lastOverwrite = index + 1;
			}
			index++;
			continue;
		}
		if (lastOverwrite < index)
		{
			char* charPosition = &fileContentsCopy.at(lastOverwrite);
			size_t viewSize = index - lastOverwrite;
			std::string_view finalSection{ charPosition,viewSize };
			output << finalSection;
		}

		return output.str();
	}

	JsonFormatter::JsonFormatter()
	{
		inputValid = false;
		//if (!inputValid)
		//{
		//	std::cout << "[JSON Formatter] Failed to find file!\nRun OpenFile() with a valid filepath." << std::endl;
		//}

		//find better way to allow for character overwriting, maybe structs?
		//charOverwrites.insert({'o',"a"});
	}

	bool JsonFormatter::OpenFile(std::string filepath)
	{
		std::ifstream jsonFile;
		jsonFile.open(filepath);
		if (jsonFile.is_open())
		{
			inputString << jsonFile.rdbuf();
			inputValid = true;
			jsonFile.close();
		}
		else
		{
			inputValid = false;
		}
		return inputValid;
	}

	bool JsonFormatter::ImportString(std::string input)
	{
		//do some foolproofing here
		inputString << input;
		inputValid = true;
		return true;
	}

	bool JsonFormatter::AddOverwrite(char input, std::string output)
	{
		if (charOverwrites.find(input) == charOverwrites.end())
		{
			charOverwrites.insert({ input, output });
			return true;
		}
		else
		{
			return false;
		}
	}

	bool JsonFormatter::RemoveOverwrite(char key)
	{
		if (charOverwrites.find(key) != charOverwrites.end())
		{
			charOverwrites.erase(key);
			return true;
		}
		else
		{
			return false;
		}
	}

	void JsonFormatter::WriteToFile(std::string filepath)
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
		//std::cout << processFile() << std::endl;
		return processFile();
	}

	void JsonFormatter::DebugLogFileContents()
	{
		std::cout << "[JSON Formatter] Logging file contents:" << std::endl;
		std::string fileContentsCopy = inputString.str();
		for (char character : fileContentsCopy)
		{
			std::cout << character;
		}
		std::cout << std::endl;
	}
}