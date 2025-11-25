#include <string>
#include <vector>
#include <map>
#pragma once

namespace IonixEngine
{

	class JsonFormatter
	{
	private:
		std::vector<char> fileContents;
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