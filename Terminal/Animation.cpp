#include "Animation.h"
#include "Logger.h"

using namespace Pastel;

namespace Exolix::Terminal
{

Thread *Animation::RenderingThreadInstance;
AnimationSettings *Animation::Settings;
int Animation::CurrentFrame = 0;
bool Animation::Running = false;
std::wstring Animation::Label = L"";
std::wstring Animation::LastOutput = L"";

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public static void Start(string label, AnimationSettings? settings = null)
	void Animation::Start(const std::wstring &label, AnimationSettings *settings)
	{
		if (settings == nullptr)
		{
			settings = new AnimationSettings();
		}

		Settings = settings;
		Running = true;
		CurrentFrame = 0;
		Label = label;

		if (RenderingThreadInstance == nullptr)
		{
			Thread *renderingThread = new Thread([&] () {FrameRenderingThread();});
			RenderingThreadInstance = renderingThread;
	//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
	//ORIGINAL LINE: renderingThread!.Start();
			renderingThread->Start();

//C# TO C++ CONVERTER TODO TASK: A 'delete renderingThread' statement was not added since renderingThread was assigned to a field. Handle memory management manually.
		}
		else if (!RenderingThreadInstance->IsAlive)
		{
			Thread *renderingThread = new Thread([&] () {FrameRenderingThread();});
			RenderingThreadInstance = renderingThread;
	//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
	//ORIGINAL LINE: renderingThread!.Start();
			renderingThread->Start();

//C# TO C++ CONVERTER TODO TASK: A 'delete renderingThread' statement was not added since renderingThread was assigned to a field. Handle memory management manually.
		}
	}

	bool Animation::GetRunningState()
	{
		return Running;
	}

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public static void Stop(string? label = null, string newState = "success")
	void Animation::Stop(const std::wstring &label, const std::wstring &newState)
	{
		if (newState == L"success" || newState == L"processing" || newState == L"error" || newState == L"warning")
		{
			if (Running)
			{
				if (label != L"")
				{
					Label = label;
				}

				Running = false;
				std::wstring prefixHex = L"60CDFF";

				if (newState == L"success")
				{
					prefixHex = L"50FFAB";
				}
				else if (newState == L"error")
				{
					prefixHex = L"FF0055";
				}
				else if (newState == L"warning")
				{
					prefixHex = L"FFA500";
				}

				RenderCurrentFrame(L"·", prefixHex);
				Logger::PrintDynamic(L"\n");
				return;
			}

			std::runtime_error("Animation is not running");
			return;
		}

		std::runtime_error("Invalid state type, the following states are supported [ \"processing\", \"success\", \"warning\", \"error\" ] ");
	}

	void Animation::FrameRenderingThread()
	{
		do
		{
			CurrentFrame++;
	//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
	//ORIGINAL LINE: if (CurrentFrame > Settings!.Frames.Length - 1)
			if (CurrentFrame > Settings->Frames.size() - 1)
			{
				CurrentFrame = 0;
			}

			RenderCurrentFrame();
	//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
	//ORIGINAL LINE: Thread.Sleep((int)Settings!.Interval);
			delay(static_cast<int>(Settings->Interval));
		} while (Running);
	}

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public static void RenderCurrentFrame(string? prefixIcon = null, string? prefixHex = null)
	void Animation::RenderCurrentFrame(const std::wstring &prefixIcon, const std::wstring &prefixHex)
	{
		std::wstring suffixSpacing;
		std::wstring outputLabel = Label;
		std::wstring renderPrefixIcon;

		int consoleWidth = Console::WindowWidth;
		int suffixLength;

		if (prefixIcon == L"")
		{
	//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
	//ORIGINAL LINE: prefixIcon = Settings!.Frames[CurrentFrame];
			prefixIcon = Settings->Frames[CurrentFrame];
		}

		if (consoleWidth - LastOutput.length() >= 0)
		{
			// TODO: Cut off label
		}

		suffixLength = consoleWidth - outputLabel.length() - prefixIcon.length() - 2;
		if (suffixLength < 0)
		{
			suffixLength = 0;
		}

		suffixSpacing = std::wstring(L' ', suffixLength);

		if (prefixHex == L"")
		{
	//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
	//ORIGINAL LINE: renderPrefixIcon = prefixIcon.Pastel("#" + Settings!.FrameHexColor);
			renderPrefixIcon = prefixIcon.Pastel(L"#" + Settings->FrameHexColor);
		}
		else
		{
			renderPrefixIcon = prefixIcon.Pastel(L"#" + prefixHex);
		}

		Logger::PrintDynamic(StringHelper::formatSimple(L"\r {0} {1}{2}", renderPrefixIcon, outputLabel, suffixSpacing));
		LastOutput = StringHelper::formatSimple(L"{0} {1}", prefixIcon, outputLabel);
	}
}
