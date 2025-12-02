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
		std::stringstream inputString;
		std::map<char, std::string> charOverwrites;
		bool inputValid;

		std::string processFile();

	public:
		JsonFormatter();
		bool OpenFile(std::string filepath);
		bool ImportString(std::string input);

		bool AddOverwrite(char input, std::string output);
		bool RemoveOverwrite(char key);

		void WriteToFile(std::string filepath);
		std::string ExportString();

		void DebugLogFileContents();
	};
}