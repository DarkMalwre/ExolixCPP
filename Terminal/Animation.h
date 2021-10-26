#pragma once

#include <string>
#include <vector>
#include <stdexcept>
#include "../stringhelper.h"

using namespace Pastel;

namespace Exolix::Terminal
{
	class AnimationSettings
	{
	public:
		std::wstring Prefix = L"";

		std::vector<std::wstring> Frames = {L"   ", L".  ", L".. ", L"...", L" ..", L"  ."};

		int Interval = 100;

		std::wstring FrameHexColor = L"60cdff";
	};

//C# TO C++ CONVERTER NOTE: The following .NET attribute has no direct equivalent in C++:
//ORIGINAL LINE: [Obsolete("Disabled because of timing issues, CLI animations arnt always the best for fast paced code", true)] public class Animation
	class Animation
	{
	private:
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: private static Thread? RenderingThreadInstance;
		static Thread *RenderingThreadInstance;
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: private static AnimationSettings? Settings;
		static AnimationSettings *Settings;
		static int CurrentFrame;
		static bool Running;
		static std::wstring Label;
		static std::wstring LastOutput;

	public:
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public static void Start(string label, AnimationSettings? settings = null)
		static void Start(const std::wstring &label, AnimationSettings *settings = nullptr);

		static bool GetRunningState();

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public static void Stop(string? label = null, string newState = "success")
		static void Stop(const std::wstring &label = L"", const std::wstring &newState = L"success");

	private:
		static void FrameRenderingThread();

	public:
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public static void RenderCurrentFrame(string? prefixIcon = null, string? prefixHex = null)
		static void RenderCurrentFrame(const std::wstring &prefixIcon = L"", const std::wstring &prefixHex = L"");
	};
}
