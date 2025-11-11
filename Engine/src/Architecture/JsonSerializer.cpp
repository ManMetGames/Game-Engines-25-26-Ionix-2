#include "JsonSerializer.h"

namespace IonixEngine
{
	void JsonSerializer::addline(std::string content)
	{
		//std::string indent = std::string(indentSize, '	');
		finalJsonFile.append("	");
		finalJsonFile.append(content);
		finalJsonFile.append(",\n");
	}

	void JsonSerializer::newField(std::string fieldName)
	{
		finalJsonFile.append("\"");
		finalJsonFile.append(fieldName);
		finalJsonFile.append("\": ");
	}

	std::string JsonSerializer::getNewField(std::string fieldName)
	{
		std::string output;
		output.append("\"");
		output.append(fieldName);
		output.append("\": ");
		return output;
	}

	JsonSerializer::JsonSerializer(std::string filepath, int indentSize) :
		filePath(filepath),
		indentSize(indentSize),
		indentLevel(0)
	{
		finalJsonFile = "{\n";
	}

	std::string JsonSerializer::GetFilePath()
	{
		return filePath;
	}

	void JsonSerializer::SetFilePath(const std::string& newFilepath)
	{
		filePath = newFilepath;
	}

	void JsonSerializer::AddStringField(std::string fieldName, std::string fieldData)
	{
		std::string newLine = getNewField(fieldName);
		newLine.append("\"");
		newLine.append(fieldData);
		newLine.append("\"");
		addline(newLine);
	}

	void JsonSerializer::AddBoolField(std::string fieldName, bool fieldData)
	{
		std::string newLine = getNewField(fieldName);
		newLine.append(std::to_string(fieldData));
		addline(newLine);
	}

	void JsonSerializer::AddIntField(std::string fieldName, int fieldData)
	{
		std::string newLine = getNewField(fieldName);
		//newLine.append("\"");
		newLine.append(std::to_string(fieldData));
		//newLine.append("\"");
		addline(newLine);
	}

	void JsonSerializer::AddFloatField(std::string fieldName, float fieldData)
	{
		std::string newLine = getNewField(fieldName);
		newLine.append(std::to_string(fieldData));
		addline(newLine);
	}

	void JsonSerializer::AddDoubleField(std::string fieldName, double fieldData)
	{
		std::string newLine = getNewField(fieldName);
		char buf[32];
		sprintf(buf, "%.16lf", fieldData);
		newLine.append(buf);
		addline(newLine);
	}

	void JsonSerializer::FinalizeJsonFile()
	{
		finalJsonFile.erase(finalJsonFile.end() - 2);
		finalJsonFile.append("}");
		std::ofstream fileWriter;
		fileWriter.open(filePath);
		fileWriter << finalJsonFile;
		fileWriter.close();
	}

	void JsonSerializer::IoTest()
	{
		addline("test");
		addline("test 2");
		FinalizeJsonFile();
	}
}