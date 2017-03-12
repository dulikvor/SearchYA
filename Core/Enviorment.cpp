#include "Enviorment.h"
#include <unistd.h>
#include "Assert.h"

namespace core
{
	Enviorment& Enviorment::Instance()
	{
		static Enviorment instance;
		return instance;
	}

	void Enviorment::Init()
	{
		ASSERT(m_initiated.exchange(true) == false);
		LINUX_VERIFY((m_coreCount = sysconf(_SC_NPROCESSORS_ONLN)) != -1);
	}
}
