# pragma once

#include <string>
#include <unordered_map>
#include <list>

class Params;

class ConfigParams
{
public:
	using NodeAddress = std::string;
	using NodePUMap = std::unordered_map<NodeAddress, std::list<int>>;

public:
	static ConfigParams& Instance();
	~ConfigParams(){}

	void Load(const Params& configParams);
	//Accessors
	const NodePUMap& GetProcessingUnitsMap() const { return m_processingUnitsMap; }

private:
	ConfigParams(){}

private:
	NodePUMap m_processingUnitsMap;

}
