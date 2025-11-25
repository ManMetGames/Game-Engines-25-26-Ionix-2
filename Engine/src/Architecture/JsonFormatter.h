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

public:
	JsonFormatter(const std::string& filepath);
	bool OpenFile();

	std::string GetFilepath();
	void SetFilepath(const std::string& newFilepath);

	void DebugLogFileContents();
};
}