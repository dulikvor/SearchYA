#include "Serializor.h"
#include "Param.h"
#include "GeneralParams.h"
#include "Core/Assert.h"

using namespace std;

Serializor::Serializor(){}

Serializor::Serializor(const char* data, int size)
{
	m_buffer.write(data, size);
}

void Serializor::Serialize(Serializor& context, int type, const bool& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	context.Write(reinterpret_cast<const char*>(&value), sizeof(bool));
}

void Serializor::Serialize(Serializor& context, int type, const int& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	context.Write(reinterpret_cast<const char*>(&value), sizeof(int));
}

void Serializor::Serialize(Serializor& context, int type, const short& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	context.Write(reinterpret_cast<const char*>(&value), sizeof(short));
}

void Serializor::Serialize(Serializor& context, int type, const long& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	context.Write(reinterpret_cast<const char*>(&value), sizeof(long));
}

void Serializor::Serialize(Serializor& context, int type, const float& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	context.Write(reinterpret_cast<const char*>(&value), sizeof(float));
}

void Serializor::Serialize(Serializor& context, int type, const double& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	context.Write(reinterpret_cast<const char*>(&value), sizeof(double));
}

void Serializor::Serialize(Serializor& context, int type, const string& value)
{
	context.Write(reinterpret_cast<const char*>(&type), sizeof(int));
	int stringSize = value.size();
	context.Write(reinterpret_cast<const char*>(&stringSize), sizeof(int));
	context.Write(value.data(), stringSize);
}

void Serializor::Serialize(Serializor& context, int type, const list<string>& value)
{
	VERIFY(false, "Invalid type, not supported by Serializor");
}


void Serializor::Serialize(Serializor& context, const bool& value)
{
	context.Write(reinterpret_cast<const char*>(&value), sizeof(bool));
}

void Serializor::Serialize(Serializor& context, const short& value)
{
	context.Write(reinterpret_cast<const char*>(&value), sizeof(short));
}

void Serializor::Serialize(Serializor& context, const int& value)
{
	context.Write(reinterpret_cast<const char*>(&value), sizeof(int));
}

void Serializor::Serialize(Serializor& context, const long& value)
{
	context.Write(reinterpret_cast<const char*>(&value), sizeof(long));
}

void Serializor::Serialize(Serializor& context, const float& value)
{
	context.Write(reinterpret_cast<const char*>(&value), sizeof(float));
}

void Serializor::Serialize(Serializor& context, const double& value)
{
	context.Write(reinterpret_cast<const char*>(&value), sizeof(double));
}

void Serializor::Serialize(Serializor& context, const string& value)
{
	int stringSize = value.size();
	context.Write(reinterpret_cast<const char*>(&stringSize), sizeof(int));
	context.Write(value.data(), stringSize);
}

int Serializor::DeserializeInt(Serializor& context)
{
	int val;
	context.Read(reinterpret_cast<char*>(&val), sizeof(int));
	return val;
}

short Serializor::DeserializeShort(Serializor& context)
{
	short val;
	context.Read(reinterpret_cast<char*>(&val), sizeof(short));
	return val;
}

long Serializor::DeserializeLong(Serializor& context)
{
	long val;
	context.Read(reinterpret_cast<char*>(&val), sizeof(long));
	return val;
}

float Serializor::DeserializeFloat(Serializor& context)
{
	float val;
	context.Read(reinterpret_cast<char*>(&val), sizeof(float));
	return val;
}

double Serializor::DeserializeDouble(Serializor& context)
{
	double val;
	context.Read(reinterpret_cast<char*>(&val), sizeof(double));
	return val;
}

bool Serializor::DeserializeBool(Serializor& context)
{
	bool val;
	context.Read(reinterpret_cast<char*>(&val), sizeof(bool));
	return val;
}

string Serializor::DeserializeString(Serializor& context)
{
	int stringSize;
	context.Read(reinterpret_cast<char*>(&stringSize), sizeof(int));
	string val(stringSize, 0);
	context.Read(const_cast<char*>(val.data()), stringSize);
	return val;
}

list<string> Serializor::DeserializeListString(Serializor& context)
{
	VERIFY(false, "Invalid type, not supported by Serializor");
	return list<string>();
}


void Serializor::Write(const char* data, int size)
{
	m_buffer.write(data, size);
}

void Serializor::Read(char* data, int size)
{
	m_buffer.read(data, size);
//	VERIFY(size == ,"Ran out of characters to read");
}

