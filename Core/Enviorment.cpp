#include "Enviorment.h"
#include <unistd.h>
#include "Assert.h"
#include "Directory.h"

using namespace std;

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
		ReadWorkingDir();
	}

	void Enviorment::ReadWorkingDir()
	{
		char buffer[MAX_WORKING_DIR_SIZE];
		ssize_t byteCount = readlink("/proc/self/exe", buffer, MAX_WORKING_DIR_SIZE);
		m_workingDir = Directory::GetDir(string(buffer, byteCount > 0 ? byteCount : 0));
	}
}
