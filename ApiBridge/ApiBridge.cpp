#include "ApiBridge.h"

namespace Exolix::ApiBridge
{

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiBridge(ApiBridgeSettings? settings)
	ApiBridge::ApiBridge(ApiBridgeSettings *settings)
	{
		if (settings == nullptr)
		{
			Settings = new ApiBridgeSettings();
			return;
		}

		Settings = settings;
	}

	void ApiBridge::Run()
	{

	}
}
