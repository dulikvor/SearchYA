#pragma once

#include <atomic>
#include <string>

namespace core
{
	class Enviorment
	{
	public:
		static Enviorment& Instance();
		void Init();
		//Accessors
		int GetCoreCount() const {return m_coreCount;}
		std::string GetWorkingDir() const {return m_workingDir;}

	private:
		void ReadWorkingDir();
	
	private:
		const int MAX_WORKING_DIR_SIZE = 500;
		int m_coreCount;
		std::string m_workingDir;
		std::atomic_bool m_initiated;

	};
}
