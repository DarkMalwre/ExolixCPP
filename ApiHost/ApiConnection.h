#pragma once

#include "../Json/JsonHandler.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <tuple>

using namespace Exolix::Json;
using namespace Fleck;

namespace Exolix::ApiHost
{
	class ApiConnection
	{
	public:
		std::wstring RemoteAddress = L"";
		std::wstring Identifier = L"";
	private:
		IWebSocketConnection *RealConnection;
		std::vector<std::function<void(ApiConnection*)>*> OnCloseEvents = std::vector<std::function<void(ApiConnection*)>*>();
		std::vector<std::tuple<std::wstring, std::function<void(const std::wstring&)>*>> OnMessageEvents = std::vector<std::tuple<std::wstring, std::function<void(const std::wstring&)>*>>();
		std::vector<std::function<void(const std::wstring&, const std::wstring&)>*> OnMessageGlobalEvents = std::vector<std::function<void(const std::wstring&, const std::wstring&)>*>();
	public:
		bool Alive = false;

		virtual ~ApiConnection()
		{
			delete RealConnection;
		}

		ApiConnection(IWebSocketConnection *connection);

		template<typename MessageType>
		void Send(const std::wstring &channel, MessageType message)
		{
			CheckAliveState();

			try
			{
				auto stringMessageData = JsonHandler::Stringify<MessageType>(message);
				class AnonymousType
				{
				public:
					std::wstring Channel;
					std::wstring Data;

					AnonymousType(std::wstring Channel, std::wstring Data) : Channel(Channel), Data(Data) {}
				};
				auto fullMessageString = JsonHandler::Stringify(AnonymousType(channel, stringMessageData));

				RealConnection->Send(fullMessageString);
			}
			catch (const std::runtime_error &e1)
			{
			}
		}

		void Close();

		void OnClose(std::function<void(ApiConnection*)> action);

		void CheckAliveState();

		/// <summary>
		/// Trigger all on close events
		/// </summary>
		void TriggerOnClose();

		void OnMessage(const std::wstring &channel, std::function<void(const std::wstring&)> action);

		void OnMessageGlobal(std::function<void(const std::wstring&, const std::wstring&)> action);

		/// <summary>
		/// Trigger all on message events
		/// </summary>
		/// <param name="message">Message to add to event</param>
		void TriggerOnMessage(const std::wstring &channel, const std::wstring &message);

		void TriggerOnMessageGlobal(const std::wstring &channel, const std::wstring &message);
	};
}
