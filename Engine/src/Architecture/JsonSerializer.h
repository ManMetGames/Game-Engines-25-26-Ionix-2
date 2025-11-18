#pragma once
#include <string>
#include <stack>
#include <iostream>
#include <fstream>

namespace IonixEngine
{
	enum JsonIndent
	{
		None,
		Object,
		ObjectArray,
		MetaArray,

		StringArray,
		BoolArray,
		IntArray,
		FloatArray,
		DoubleArray
	};

	class JsonSerializer
	{
	private:
		int indentSize;
		bool tabIndent;
		std::stack<JsonIndent> indents;
		std::string finalJsonFile;
		std::string filePath;

		void addline(std::string content, std::string lineEnd = ",");
		void newField(std::string fieldName);
		std::string getNewField(std::string fieldName);

	public:
		JsonSerializer(std::string filepath, bool tabIndent = true, int indentSize = 4);
		std::string GetFilePath();
		void SetFilePath(const std::string& newFilepath);
		
		void AddStringField(std::string fieldName, std::string fieldData);
		void AddBoolField(std::string fieldName, bool fieldData);
		void AddIntField(std::string fieldName, int fieldData);
		void AddFloatField(std::string fieldName, float fieldData);
		void AddDoubleField(std::string fieldName, double fieldData);

		void NewArray(std::string arrayName);
		void NewObject(std::string objectName);
		void AddObject(std::string objectName);
		void EndObject();

		void FinalizeJsonFile();
		void IoTest();
	};
}
