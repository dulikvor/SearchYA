# pragma once

#include <string>
#include <unordered_map>
#include <list>

class GeneralParams;

class ConfigParams
{
public:
	using NodeAddress = std::string;

public:
	static ConfigParams& Instance();
	~ConfigParams(){}
	//Will fill all internal members using a received general param instance.
	//each member equivilant node name with in the general param is known and
	//hard written. the class is imutable passed this point.
	void Load(const GeneralParams& configParams);
	//Accessors
	const std::list<NodeAddress>& GetClusterNodes() const { return m_clusterNodesAddresses; }
	int GetRecoveryTime() const { return m_recoveryTime; }
	const std::string& GetHostAddress() const { return m_hostAddress; }
	const std::string& GetMesosMasterAddress() const { return m_mesosMasterAddress; }
	std::string GetMesosMasterAddress() { return const_cast<const ConfigParams&>(*this).GetMesosMasterAddress(); }
	const std::string&  GetExecDir() const { return m_executableDir; }
	std::string GetExecDir() { return const_cast<const ConfigParams&>(*this).GetExecDir(); }
	const std::string& GetRole() const { return m_role; }
	const std::string& GetRedisServerAddress() const { return m_redisServerAddress; }
	const std::string& GetZipkinHostAddress() const { return m_zipkinHostAddress; }

private:
	ConfigParams(){}

private:
	std::list<NodeAddress> m_clusterNodesAddresses;
	int m_recoveryTime;
	std::string m_hostAddress;
	std::string m_mesosMasterAddress;
	std::string m_redisServerAddress;
	int m_mesosMasterPort;
	std::string m_zipkinHostAddress;
	std::string m_executableDir;
	std::string m_role;
};
