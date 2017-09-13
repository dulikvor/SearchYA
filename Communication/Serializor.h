#pragma once

#include <sstream>
#include <string>
#include <list>
#include <vector>

class GeneralParams;

class Serializor
{
public:
	Serializor();
	Serializor(const char* data, int size);
	~Serializor(){}
	static void Serialize(Serializor& context, int type, const bool& value);
	static void Serialize(Serializor& context, int type, const short& value);
	static void Serialize(Serializor& context, int type, const int& value);
	static void Serialize(Serializor& context, int type, const long& value);
	static void Serialize(Serializor& context, int type, const float& value);
	static void Serialize(Serializor& context, int type, const double& value);
	static void Serialize(Serializor& context, int type, const std::string& value);
	static void Serialize(Serializor& context, int type, const std::list<std::string>& value);
	static void Serialize(Serializor& context, const bool& value);
	static void Serialize(Serializor& context, const short& value);
	static void Serialize(Serializor& context, const int& value);
	static void Serialize(Serializor& context, const long& value);
	static void Serialize(Serializor& context, const float& value);
	static void Serialize(Serializor& context, const double& value);
	static void Serialize(Serializor& context, const std::string& value);
	static void Serialize(Serializor& context, const std::vector<std::string>& value);
	static int DeserializeInt(Serializor& context);
	static short DeserializeShort(Serializor& context);
	static long DeserializeLong(Serializor& context);
	static bool DeserializeBool(Serializor& context);
	static float DeserializeFloat(Serializor& context);
	static double DeserializeDouble(Serializor& context);
	static std::string DeserializeString(Serializor& context);
	static std::list<std::string> DeserializeListString(Serializor& context);
	static std::vector<std::string> DeserializeVectorString(Serializor& context);

	void Write(const char* data, int size);
	void Read(char* buffer, int size);
	void Clean();
	//Accessors
	std::string GetBuffer(){return m_buffer.str();}

private:
	std::stringstream m_buffer;
};
