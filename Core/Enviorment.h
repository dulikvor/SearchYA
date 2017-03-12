#pragma once

#include <atomic>

namespace core
{
	class Enviorment
	{
	public:
		static Enviorment& Instance();
		void Init();
		//Accessors
		int GetCoreCount() const {return m_coreCount;}
	
	private:
		int m_coreCount;
		std::atomic_bool m_initiated;

	};
}
