#include "ApiConnection.h"

using namespace Exolix::Json;
using namespace Fleck;

namespace Exolix::ApiHost
{

	ApiConnection::ApiConnection(IWebSocketConnection *connection)
	{
		Alive = connection->IsAvailable;
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to 'ToString':
		Identifier = connection->ConnectionInfo.Id->ToString();
		RemoteAddress = connection->ConnectionInfo.ClientIpAddress;
		RealConnection = connection;
	}

	void ApiConnection::Close()
	{
		bool alreadyClosed = false;

		try
		{
			RealConnection->Close();
		}
		catch (const std::runtime_error &e1)
		{
			alreadyClosed = true;
		}
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the exception 'finally' clause:
		finally
		{
			Alive = false;

			if (!alreadyClosed)
			{
				TriggerOnClose();
			}
		}
	}

	void ApiConnection::OnClose(std::function<void(ApiConnection*)> action)
	{
		OnCloseEvents.push_back(action);
	}

	void ApiConnection::CheckAliveState()
	{
		Alive = RealConnection->IsAvailable;
	}

	void ApiConnection::TriggerOnClose()
	{
		CheckAliveState();
		ApiConnection *instance = static_cast<ApiConnection*>(this->MemberwiseClone());

		for (auto action : OnCloseEvents)
		{
			Thread tempVar(new ThreadStart([&] ()
			{
				action(instance);
			}));
			(&tempVar)->Start();
		}
	}

	void ApiConnection::OnMessage(const std::wstring &channel, std::function<void(const std::wstring&)> action)
	{
		OnMessageEvents.push_back(std::make_tuple(channel, action));
	}

	void ApiConnection::OnMessageGlobal(std::function<void(const std::wstring&, const std::wstring&)> action)
	{
		OnMessageGlobalEvents.push_back(action);
	}

	void ApiConnection::TriggerOnMessage(const std::wstring &channel, const std::wstring &message)
	{
		for (auto action : OnMessageEvents)
		{
			if (action.Item1 == channel)
			{
				Thread tempVar(new ThreadStart([&] ()
				{
					action.Item2(message);
				}));
				(&tempVar)->Start();
			}
		}
	}

	void ApiConnection::TriggerOnMessageGlobal(const std::wstring &channel, const std::wstring &message)
	{
		for (auto action : OnMessageGlobalEvents)
		{
			action(channel, message);
		}
	}
}
