#include "States.h"
#include "../Terminal/Logger.h"

using namespace Exolix::Terminal;

namespace Exolix::Developer
{

bool States::isDebugMode = false;

	void States::SetDebugMode(bool enabled)
	{
		isDebugMode = enabled;
	}

	bool States::GetDebugMode()
	{
		return isDebugMode;
	}

	void States::End()
	{
		if (isDebugMode)
		{
			Logger::PrintDynamic(L"\n Exolix End | Debug Mode \n --- \n");
		}
	}
}
