#include "JsonSerializer.h"

namespace IonixEngine
{
	//appends a new line to the output json string, parameter content is the content of the new line
	//adds indentation and newline commands automatically
	void JsonSerializer::addline(std::string content,std::string lineEnd)
	{
		std::string indent;
		if (tabIndent)
		{
			indent = std::string(indents.size(), '	');
		}
		else
		{
			indent = std::string(indent.size() * indentSize, ' ');
		}
		finalJsonFile.append(indent);
		finalJsonFile.append(content);
		finalJsonFile.append(lineEnd);
		finalJsonFile.append("\n");
	}

	//appends a field name string to the output json string
	void JsonSerializer::newField(std::string fieldName)
	{
		finalJsonFile.append("\"");
		finalJsonFile.append(fieldName);
		finalJsonFile.append("\": ");
	}

	//returns a field name string which can be appended to the output json string
	std::string JsonSerializer::getNewField(std::string fieldName)
	{
		std::string output;
		output.append("\"");
		output.append(fieldName);
		output.append("\": ");
		return output;
	}

	JsonSerializer::JsonSerializer(std::string filepath, bool tabIndent, int indentSize) :
		tabIndent(tabIndent),
		filePath(filepath),
		indentSize(indentSize)
	{
		indents.push(JsonIndent::None);
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
		//newLine.append(std::to_string(fieldData));
		char buf[32];
		sprintf(buf, "%s", fieldData ? "true" : "false");
		newLine.append(buf);
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

	void JsonSerializer::AddObject(std::string objectName)
	{
		std::string newLine;
		std::string indent;
		switch (indents.top())
		{
		case None:
		case Object:
		case ObjectArray:
			newLine = getNewField(objectName);
			//newLine.append("\n");
			addline(newLine,"");
			indent = std::string(indents.size(), '	');
			finalJsonFile.append(indent);
			//addline("{");
			finalJsonFile.append("{\n");
			indents.push(JsonIndent::Object);
			break;
		default:
			std::cout << "Invalid object creation" << std::endl;
			break;
		}

	}

	void JsonSerializer::EndObject()
	{
		switch (indents.top())
		{
		case Object:
			indents.pop();
			finalJsonFile.erase(finalJsonFile.end() - 2);
			addline("}");
			break;
		default:
			std::cout << "Invalid call to end object" << std::endl;
			break;
		}
	}

	void JsonSerializer::FinalizeJsonFile()
	{
		while (indents.size() > 1)
		{
			switch (indents.top())
			{
			case Object:
				EndObject();
				break;
			default:
				addline(":D", "]");
				indents.pop();
				break;
			}
		}
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