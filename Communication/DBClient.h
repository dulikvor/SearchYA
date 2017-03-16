#pragma once

#include <string>
#include <vector>
#include <tuple>

struct redisContext;

class DBClient
{
public:
	DBClient():m_redisContext(nullptr){};
	~DBClient();
	//Attempts to establish a connection to redis server designated by a received host address.
	void Connect(const std::string& hostAddress);
	
	bool SetHashFields(const std::string& key, 
			const std::vector<std::tuple<const std::string&, char*, int>>& fieldsValues);
	bool DelHashFields(const std::string& key, const std::vector<std::string>& fields);
	//std::vector<std::vector<char>> GetHashValues(const std::string& key, 
	//		const std::vector<std::string>& fields);

private:
	redisContext* m_redisContext;
};
