#include "Logger.h"
#include "../Developer/States.h"

using namespace Exolix::Developer;
using namespace Pastel;

namespace Exolix::Terminal
{

bool Logger::KeepAliveState = false;
Thread *Logger::KeepAliveThreadInstance;
int Logger::KeepAliveRequests = 0;

	void Logger::Info(const std::wstring &message)
	{
		PrintLineDynamic((std::wstring(L" · [ Info ]")).Pastel(L"#60cdff") + L" " + message);
	}

	void Logger::PrintDynamic(const std::wstring &stdOutRaw)
	{
		if (States::GetDebugMode())
		{
			Debug::Write(stdOutRaw);
			return;
		}

		std::wcout << stdOutRaw;
	}

	void Logger::Success(const std::wstring &message)
	{
		PrintLineDynamic((std::wstring(L" · [ Success ]")).Pastel(L"#50ffab") + L" " + message);
	}

	void Logger::Error(const std::wstring &message)
	{
		PrintLineDynamic((std::wstring(L" · [ Error ]")).Pastel(L"#ff5555") + L" " + message);
	}

	void Logger::ErrorException(std::runtime_error error)
	{
		Error(error.what());
		StackTrace tempVar(1, true);
		std::vector<StackFrame*> stFrames = (&tempVar)->GetFrames();

		for (auto stack : stFrames)
		{
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
			Error(L"    " + stack->ToString().TrimEnd(L'\n'));
		}
	}

	void Logger::Warning(const std::wstring &message)
	{
		PrintLineDynamic((std::wstring(L" · [ Warning ]")).Pastel(L"#ffaa55") + L" " + message);
	}

	void Logger::PrintLineDynamic(const std::wstring &stdOutText)
	{
		PrintDynamic(stdOutText + L"\n");
	}

	void Logger::AppDone(const std::wstring &toAction)
	{
		Logger::PrintLineDynamic(StringHelper::formatSimple(L" {0} {1} {2}", (std::wstring(L"Press")).Pastel(L"#999999"), (std::wstring(L"ENTER")).Pastel(L"#ffffff"), StringHelper::formatSimple(L"to {0}", toAction).Pastel(L"#999999")));
		while (Console::ReadKey().Key != ConsoleKey::Enter)
		{
		}
	}

	void Logger::HideCursor()
	{
		Console::CursorVisible = false;
	}

	void Logger::ShowCursor()
	{
		Console::CursorVisible = true;
	}

	void Logger::KeepAliveThread()
	{
		do
		{
			delay(100);
		} while (KeepAliveState);
	}

	void Logger::KeepAlive(bool enabled)
	{
		if (enabled)
		{
			KeepAliveRequests++;

			if (KeepAliveThreadInstance == nullptr || !KeepAliveThreadInstance->IsAlive)
			{
				KeepAliveThreadInstance = new Thread([&] () {KeepAliveThread();});
				KeepAliveThreadInstance->Start();
			}
		}
		else
		{
			KeepAliveRequests--;
		}

		if (KeepAliveRequests > 0)
		{
			KeepAliveState = true;
			return;
		}

		KeepAliveState = false;
	}
}
