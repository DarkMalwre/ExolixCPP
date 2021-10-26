#pragma once

#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>
#include "../stringhelper.h"

using namespace Exolix::Developer;
using namespace Pastel;

namespace Exolix::Terminal
{
	class Logger
	{
	private:
		static bool KeepAliveState;
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: private static Thread? KeepAliveThreadInstance;
		static Thread *KeepAliveThreadInstance;
		static int KeepAliveRequests;

	public:
		static void Info(const std::wstring &message);

		static void PrintDynamic(const std::wstring &stdOutRaw);

		static void Success(const std::wstring &message);

		static void Error(const std::wstring &message);

		static void ErrorException(std::runtime_error error);

		static void Warning(const std::wstring &message);

		static void PrintLineDynamic(const std::wstring &stdOutText);

		static void AppDone(const std::wstring &toAction = L"exit");

		static void HideCursor();

		static void ShowCursor();

	private:
		static void KeepAliveThread();

	public:
		static void KeepAlive(bool enabled);
	};
}
