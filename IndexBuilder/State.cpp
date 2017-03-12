#include "State.h"
#include "IndexBuilder.h"

void State::HandleTerminate()
{
	IndexBuilder::Instance().Terminate();
}
