#pragma once
#include <string>
#include <vector>
#include <map>
#include <fstream>
#include <sstream>

namespace IonixEngine
{

	class JsonFormatter
	{
	private:
		std::vector<char> fileContentsVector;
		std::stringstream fileContents;
		std::string filepath;
		std::map<char, std::string> charOverwrites;
		bool fileIsValid;
	public:
		JsonFormatter(const std::string& filepath);
		bool OpenFile();

		void ProcessFile();

		std::string GetFilepath();
		void SetFilepath(const std::string& newFilepath);

		void DebugLogFileContents();
	};
}