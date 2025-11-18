#pragma once
#include <string>
#include <stack>
#include <vector>
#include <iostream>
#include <fstream>

namespace IonixEngine
{
	enum JsonIndent
	{
		Object,
		ObjectArray,
		MultiArray,

		Array
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
		void beginArray(std::string arrayName, JsonIndent indentType = Array);
		void finishArray();
		std::string getNewField(std::string fieldName);
		std::string getIndent();

	public:
		JsonSerializer(std::string filepath, bool tabIndent = true, int indentSize = 4);
		std::string GetFilePath();
		void SetFilePath(const std::string& newFilepath);
		
		void AddStringField(std::string fieldName, std::string fieldData);
		void AddBoolField(std::string fieldName, bool fieldData);
		void AddIntField(std::string fieldName, int fieldData);
		void AddFloatField(std::string fieldName, float fieldData);
		void AddDoubleField(std::string fieldName, double fieldData);

		void AddObject(std::string objectName);
		void EndObject();

		void AddStringArray(std::string arrayName, std::vector<std::string> stringVector);
		void AddBoolArray(std::string arrayName, std::vector<bool> boolVector);
		void AddIntArray(std::string arrayName, std::vector<int> intVector);
		void AddFloatArray(std::string arrayName, std::vector<float> floatVector);
		void AddDoubleArray(std::string arrayName, std::vector<double> doubleVector);
		
		void AddObjectArray(std::string arrayName);
		void NextObjectInArray();
		void EndObjectArray();

		//void AddMultiArray(std::string arrayName);
		//void NextArrayInMulti();
		//void EndMultiArray();

		void FinalizeJsonFile();
		void IoTest();
	};
}
