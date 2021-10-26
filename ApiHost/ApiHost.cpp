#include "ApiHost.h"
#include "../Terminal/Logger.h"

using namespace Exolix::Json;
using namespace Exolix::Terminal;
using namespace Fleck;

namespace Exolix::ApiHost
{

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiHost(ApiHostSettings? settings)
	ApiHost::ApiHost(ApiHostSettings *settings)
	{
		if (settings == nullptr)
		{
			Settings = new ApiHostSettings();
			return;
		}

		Settings = settings;
	}

	std::wstring ApiHost::BuildClusterConnectUrl(const std::wstring &host, std::optional<int> &port, bool secure)
	{
		std::wstring protocol = L"ws://";
		if (secure)
		{
			protocol = L"wss://";
		}

		std::wstring portSuffix = L"";
		if (port)
		{
			portSuffix = L":" + port;
		}

		return protocol + host + portSuffix;
	}

	std::wstring ApiHost::BuildConnectUrl()
	{
		std::wstring protocol = L"ws://";
		if (Settings->Certificate != nullptr)
		{
			protocol = L"wss://";
		}

		std::wstring portSuffix = L"";
		if (Settings->Port)
		{
			portSuffix = L":" + Settings->Port;
		}

		return protocol + Settings->Host + portSuffix;
	}

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiConnection? GetConnection(string Identifier)
	ApiConnection *ApiHost::GetConnection(const std::wstring &Identifier)
	{
		for (auto connection : ApiConnections)
		{
			if (connection->Identifier == Identifier)
			{
				return connection;
			}
		}

		return nullptr;
	}

	std::vector<ApiConnection*> ApiHost::GetAllConnections()
	{
		return ApiConnections;
	}

	void ApiHost::CheckAliveConnections()
	{
		try
		{
			auto connections = GetAllConnections();
			for (auto connection : connections)
			{
				if (connection != nullptr && !connection->Alive)
				{
					ApiConnections.Remove(connection);
					ConnectedClients = ApiConnections.size();
				}
			}
		}
		catch (const std::runtime_error &e1)
		{
			CheckAliveConnections();
		}
	}

	void ApiHost::Run()
	{
		Logger::KeepAlive(true);
		ListeningAddress = BuildConnectUrl();

		auto server = new WebSocketServer(ListeningAddress);
		FleckLog->LogAction = [&] (level, message, ex)
		{
			if (message == L"Server started at " + ListeningAddress + L" (actual port " + Settings->Port + L")")
			{
				TriggerOnReady();
			}
		};

		server->Start([&] (socket)
		{
			Thread tempVar(new ThreadStart([&] ()
			{
					auto apiConnection = new ApiConnection(socket);

					socket->OnOpen = [&] ()
					{
						ApiConnections.push_back(apiConnection);
						ConnectedClients = ApiConnections.size();
						TriggerOnOpen(apiConnection);

						CheckAliveConnections();
					};

					socket->OnClose = [&] ()
					{
						RemoveConnection(apiConnection->Identifier);
						ConnectedClients = ApiConnections.size();
						apiConnection->TriggerOnClose();

						CheckAliveConnections();
					};

					socket->OnMessage = [&] (message)
					{
						try
						{
							auto parsedMessageContainer = JsonHandler::Parse<ApiMessageContainer*>(message);
							std::wstring channel = parsedMessageContainer->Channel;
							std::wstring data = parsedMessageContainer->Data;

							if (channel != L"" && dynamic_cast<std::wstring>(channel) != nullptr && data != L"" && dynamic_cast<std::wstring>(data) != nullptr)
							{
								apiConnection->TriggerOnMessage(channel, data);
								apiConnection->TriggerOnMessageGlobal(channel, data);
							}
						}
						catch (const std::runtime_error &e1)
						{
						}
					};

//C# TO C++ CONVERTER TODO TASK: A 'delete apiConnection' statement was not added since apiConnection was passed to a method or constructor. Handle memory management manually.
			}));
			(&tempVar)->Start();
		});

		delete server;
	}

	void ApiHost::OnReady(std::function<void()> action)
	{
		OnReadyEvents.push_back(action);
	}

	void ApiHost::TriggerOnReady()
	{
		for (auto action : OnReadyEvents)
		{
			Thread tempVar(new ThreadStart(action));
			(&tempVar)->Start();
		}
	}

	void ApiHost::RemoveConnection(const std::wstring &Identifer)
	{
		for (auto connection : ApiConnections)
		{
			if (connection->Identifier == Identifer)
			{
				ApiConnections.Remove(connection);
				break;
			}
		}
	}

	void ApiHost::OnOpen(std::function<void(ApiConnection*)> action)
	{
		OnOpenEvents.push_back(action);
	}

	void ApiHost::TriggerOnOpen(ApiConnection *connection)
	{
		for (auto action : OnOpenEvents)
		{
			Thread tempVar(new ThreadStart([&] ()
			{
				action(connection);
			}));
			(&tempVar)->Start();
		}
	}
}
