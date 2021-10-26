#pragma once

#include <string>
#include <optional>

namespace Exolix::ApiBridge
{
	class ApiBridgeSettings
	{
	public:
		std::wstring Host = L"localhost";
		std::optional<int> Port;
	};

	class ApiBridge
	{
	public:
		ApiBridgeSettings *Settings;

		virtual ~ApiBridge()
		{
			delete Settings;
		}

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiBridge(ApiBridgeSettings? settings)
		ApiBridge(ApiBridgeSettings *settings);

		void Run();
	};
}
