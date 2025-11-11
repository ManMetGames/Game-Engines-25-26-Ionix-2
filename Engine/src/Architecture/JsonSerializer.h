#pragma once
#include <string>
#include <stack>
#include <iostream>
#include <fstream>

namespace IonixEngine
{
	class JsonSerializer
	{
	private:
		int indentLevel;
		int indentSize;
		std::stack<int> indents;
		std::string finalJsonFile;
		std::string filePath;

		void addline(std::string content);
		void newField(std::string fieldName);
		std::string getNewField(std::string fieldName);

	public:
		JsonSerializer(std::string filepath, int indentSize = 4);
		std::string GetFilePath();
		void SetFilePath(const std::string& newFilepath);
		
		void AddStringField(std::string fieldName, std::string fieldData);
		void AddIntField(std::string fieldName, int fieldData);

		void FinalizeJsonFile();
		void IoTest();
	};
}
