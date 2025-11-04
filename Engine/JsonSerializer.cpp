#include "JsonSerializer.h"


namespace IonixEngine
{
	JsonSerializer::JsonSerializer(std::string filepath, std::string filename, int indentSize) :
		filePath(filepath),
		fileName(filename),
		indentSize(indentSize),
		indentLevel(0),

	{
	}
}