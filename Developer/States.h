#pragma once

using namespace Exolix::Terminal;

namespace Exolix::Developer
{
	class States
	{
	private:
		static bool isDebugMode;

	public:
		static void SetDebugMode(bool enabled);

		static bool GetDebugMode();

		static void End();
	};
}
