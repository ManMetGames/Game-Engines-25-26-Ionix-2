#include "JsonSerializer.h"

namespace IonixEngine
{
	//appends a new line to the output json string, parameter content is the content of the new line
	//adds indentation and newline commands automatically
	void JsonSerializer::addline(std::string content,std::string lineEnd)
	{
		finalJsonFile.append(getIndent());
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

	void JsonSerializer::beginArray(std::string arrayName, JsonIndent indentType)
	{
		switch (indents.top())
		{
		case Object:
			addline(getNewField(arrayName), "");
		case MultiArray:
			finalJsonFile.append(getIndent());
			finalJsonFile.append("[\n");
			indents.push(indentType);
			break;
		}

	}

	void JsonSerializer::finishArray()
	{
		indents.pop();
		std::string indent = getIndent();
		finalJsonFile.erase(finalJsonFile.end() - 2);
		finalJsonFile.append(indent);
		finalJsonFile.append("],\n");
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

	//returns a string with correct indent style and length
	std::string JsonSerializer::getIndent()
	{
		std::string output;
		if (tabIndent)
		{
			output = std::string(indents.size(), '	');
		}
		else
		{
			output = std::string(indents.size() * indentSize, ' ');
		}
		return output;
	}

	JsonSerializer::JsonSerializer(std::string filepath, bool tabIndent, int indentSize) :
		tabIndent(tabIndent),
		filePath(filepath),
		indentSize(indentSize)
	{
		indents.push(Object);
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
		if (indents.top() != Object)
		{
			std::cout << "Cannot add a string field, add an object first" << std::endl;
		}
		else
		{
			std::string newLine;
			newLine = getNewField(fieldName);
			newLine += '"';
			newLine.append(fieldData);
			newLine += '"';
			addline(newLine);
		}
	}

	void JsonSerializer::AddBoolField(std::string fieldName, bool fieldData)
	{
		if (indents.top() != Object)
		{
			std::cout << "Cannot add a boolean field, add an object first" << std::endl;
		}
		else
		{
			std::string newLine;
			newLine = getNewField(fieldName);
			char buf[32];
			sprintf(buf, "%s", fieldData ? "true" : "false");
			newLine.append(buf);
			addline(newLine);
		}
	}

	void JsonSerializer::AddIntField(std::string fieldName, int fieldData)
	{
		if (indents.top() != Object)
		{
			std::cout << "Cannot add a integer field, add an object first" << std::endl;
		}
		else
		{
			std::string newLine = getNewField(fieldName);
			//newLine.append("\"");
			newLine.append(std::to_string(fieldData));
			//newLine.append("\"");
			addline(newLine);
		}
	}

	void JsonSerializer::AddFloatField(std::string fieldName, float fieldData)
	{
		if (indents.top() != Object)
		{
			std::cout << "Cannot add a float field, add an object first" << std::endl;
		}
		else
		{
			std::string newLine = getNewField(fieldName);
			newLine.append(std::to_string(fieldData));
			addline(newLine);
		}
	}

	void JsonSerializer::AddDoubleField(std::string fieldName, double fieldData)
	{
		if (indents.top() != Object)
		{
			std::cout << "Cannot add a string field, add an object first" << std::endl;
		}
		else
		{
			std::string newLine = getNewField(fieldName);
			char buf[32];
			sprintf(buf, "%.16lf", fieldData);
			newLine.append(buf);
			addline(newLine);
		}
	}

	void JsonSerializer::AddObject(std::string objectName)
	{
		std::string newLine;
		std::string indent;
		switch (indents.top())
		{
		case Object:
		case ObjectArray:
			newLine = getNewField(objectName);
			addline(newLine,"");
			indent = getIndent();
			finalJsonFile.append(indent);
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
		if (indents.size() == 1)
		{
			std::cout << "Invalid call to end object" << std::endl;
		}
		switch (indents.top())
		{
		case Object:
			std::cout << "Ending object" << std::endl;
			indents.pop();
			finalJsonFile.erase(finalJsonFile.end() - 2);
			addline("}");
			break;
		default:
			std::cout << "Invalid call to end object" << std::endl;
			break;
		}
	}

	void JsonSerializer::AddStringArray(std::string arrayName, std::vector<std::string> stringVector)
	{
		beginArray(arrayName);
		for (std::string stringElement : stringVector)
		{
			std::string newline;
			newline += '"';
			newline.append(stringElement);
			newline += '"';
			addline(newline);
		}
		finishArray();
	}

	void JsonSerializer::AddBoolArray(std::string arrayName, std::vector<bool> boolVector)
	{
		beginArray(arrayName);
		for (bool boolElement:boolVector)
		{
			std::string newline;
			char buf[32];
			sprintf(buf, "%s", boolElement ? "true" : "false");
			newline.append(buf);
			addline(newline);
		}
		finishArray();
	}

	void JsonSerializer::AddIntArray(std::string arrayName, std::vector<int> intVector)
	{
		beginArray(arrayName);
		for (int num : intVector)
		{
			addline(std::to_string(num));
		}
		finishArray();
	}

	void JsonSerializer::AddFloatArray(std::string arrayName, std::vector<float> floatVector)
	{
		beginArray(arrayName);
		for (float num : floatVector)
		{
			addline(std::to_string(num));
		}
		finishArray();
	}

	void JsonSerializer::AddDoubleArray(std::string arrayName, std::vector<double> doubleVector)
	{
		beginArray(arrayName);
		for (double num : doubleVector)
		{
			std::string newline;
			char buf[32];
			sprintf(buf, "%.16lf", num);
			newline.append(buf);
			addline(newline);
		}
		finishArray();
	}

	//
	void JsonSerializer::AddObjectArray(std::string arrayName)
	{
		beginArray(arrayName, ObjectArray);
		addline("{","");
		indents.push(Object);
	}

	void JsonSerializer::NextObjectInArray()
	{
		finalJsonFile.erase(finalJsonFile.end() - 2);
		indents.pop();
		addline("},","");
		addline("{", "");
		indents.push(Object);
	}

	void JsonSerializer::EndObjectArray()
	{
		finalJsonFile.erase(finalJsonFile.end() - 2);
		indents.pop();
		addline("}");
		finalJsonFile.erase(finalJsonFile.end() - 2);
		indents.pop();
		addline("]");
	}

	//void JsonSerializer::AddMultiArray(std::string arrayName)
	//{
	//	beginArray(arrayName,MultiArray);
	//	//indents.push(MultiArray);
	//}

	//void JsonSerializer::NextArrayInMulti()
	//{
	//}

	void JsonSerializer::FinalizeJsonFile()
	{
		while (indents.size() > 1)
		{
			std::cout << indents.top() << std::endl;
			switch (indents.top())
			{
			case Object:
				EndObject();
				break;
			case ObjectArray:
				EndObjectArray();
				break;
			default:
				indents.pop();
				addline("AAAAA");
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