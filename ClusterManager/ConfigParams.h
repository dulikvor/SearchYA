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
	int GetRecoveryTime() const { return m_recoveryTime; }
	const std::string& GetHostAddress() const { return m_hostAddress; }
	const std::string& GetMesosMasterAddress() const { return m_mesosMasterAddress; }
	std::string GetMesosMasterAddress() { return const_cast<const ConfigParams&>(*this).GetMesosMasterAddress(); }
	const std::string&  GetExecDir() const { return m_executableDir; }
	std::string GetExecDir() { return const_cast<const ConfigParams&>(*this).GetExecDir(); }

private:
	ConfigParams(){}

private:
	NodePUMap m_processingUnitsMap;
	int m_recoveryTime;
	std::string m_hostAddress;
	std::string m_mesosMasterAddress;
	int m_mesosMasterPort;
	std::string m_executableDir;

};
