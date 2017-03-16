# pragma once

#include <string>
#include <unordered_map>
#include <list>

class GeneralParams;

class ConfigParams
{
public:
	static ConfigParams& Instance();
	~ConfigParams(){}
	//Will fill all internal members using a received general param instance.
	//each member equivilant node name with in the general param is known and
	//hard written. the class is imutable passed this point.
	void Load(const GeneralParams& configParams);
	//Accessors
	const std::string& GetRedisHostAddress() const { return m_redisHostAddress; }

private:
	ConfigParams(){}

private:
	std::string m_redisHostAddress;
};
