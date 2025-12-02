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
		std::vector<std::string_view> reconstructedFile;
		std::stringstream fileContents;
		std::string filepath;
		std::map<char, std::string> charOverwrites;
		bool fileIsValid;

		std::string processFile();

	public:
		JsonFormatter(const std::string& filepath);
		bool OpenFile();

		void WriteToFile();
		std::string ExportString();

		std::string GetFilepath();
		void SetFilepath(const std::string& newFilepath);

		void DebugLogFileContents();
	};
}