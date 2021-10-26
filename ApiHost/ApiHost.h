#pragma once

#include "ApiConnection.h"
#include <string>
#include <vector>
#include <stdexcept>
#include <functional>
#include <optional>

using namespace Exolix::Json;
using namespace Exolix::Terminal;
using namespace Fleck;

namespace Exolix::ApiHost
{
	class ApiHostCertificate
	{
	public:
		std::wstring Certificate = L"";
		std::wstring Key = L"";
	};

	class ApiClusterAuth
	{
	public:
		std::wstring Key1 = L"";
		std::wstring Key2 = L"";
	};

	class ApiPeerNode
	{
	public:
		std::wstring Key1 = L"";
		std::wstring Key2 = L"";
		std::wstring Host = L"0.0.0.0";
		std::optional<int> Port;
	};

	class ApiHostSettings
	{
	public:
		std::wstring Host = L"0.0.0.0";
		std::optional<int> Port;
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiHostCertificate? Certificate = null;
		ApiHostCertificate *Certificate = nullptr;
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiClusterAuth? ClusterAuth = null;
		ApiClusterAuth *ClusterAuth = nullptr;
		virtual ~ApiHostSettings()
		{
			delete Certificate;
			delete ClusterAuth;
		}

	};

	class ApiMessageContainer
	{
	public:
		std::wstring Channel = L"Main";
		std::wstring Data = L"{ \"No\": \"Value\" }";
	};

	class ApiHost
	{
		/// <summary>
		/// Settings for the API endpoint server
		/// </summary>
	private:
		ApiHostSettings *Settings;

		/// <summary>
		/// All server connections
		/// </summary>
		std::vector<ApiConnection*> ApiConnections = std::vector<ApiConnection*>();

	public:
		int ConnectedClients = 0;

		/// <summary>
		/// Address were server has been opened at
		/// </summary>
		std::wstring ListeningAddress = L"Server Not Connected";

		/// <summary>
		/// List of all on ready event actions
		/// </summary>
	private:
		std::vector<std::function<void()>> OnReadyEvents = std::vector<std::function<void()>>();

		/// <summary>
		/// On open events list
		/// </summary>
		std::vector<std::function<void(ApiConnection*)>*> OnOpenEvents = std::vector<std::function<void(ApiConnection*)>*>();

		/// <summary>
		/// Create a new API endpoint server
		/// </summary>
		/// <param name="settings">Settings for the server</param>
	public:
		virtual ~ApiHost()
		{
			delete Settings;
		}

//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiHost(ApiHostSettings? settings)
		ApiHost(ApiHostSettings *settings);

		/// <summary>
		/// Build an address to connect to a cluster connection
		/// </summary>
		/// <param name="host">Peer node host</param>
		/// <param name="port">Peer node port</param>
		/// <param name="secure">Is the peer node using a secure protocol</param>
		/// <returns>Connect address</returns>
	private:
		std::wstring BuildClusterConnectUrl(const std::wstring &host, std::optional<int> &port, bool secure = false);

		/// <summary>
		/// Build an address from settings for hosting the server
		/// </summary>
		/// <returns>Connection address</returns>
		std::wstring BuildConnectUrl();

		/// <summary>
		/// Get a connection from its identifier
		/// </summary>
		/// <param name="Identifier">Connection identifier</param>
		/// <returns>Returns the connection object, otherwise null if it does not exist</returns>
	public:
//C# TO C++ CONVERTER WARNING: Nullable reference types have no equivalent in C++:
//ORIGINAL LINE: public ApiConnection? GetConnection(string Identifier)
		ApiConnection *GetConnection(const std::wstring &Identifier);

		/// <summary>
		/// Get a list currently connected connections
		/// </summary>
		/// <returns>List of all connections</returns>
		std::vector<ApiConnection*> GetAllConnections();

		/// <summary>
		/// Send a message to all currently connected clients
		/// </summary>
		/// <typeparam name="MessageType">Data type for message</typeparam>
		/// <param name="channel">Receiver channel</param>
		/// <param name="message">Message object</param>
		template<typename MessageType>
		void Emit(const std::wstring &channel, MessageType message)
		{
			auto allCons = std::vector<ApiConnection*>(GetAllConnections());
			CheckAliveConnections();

			for (auto connection : allCons)
			{
				try
				{
					if (connection != nullptr)
					{
						connection->Send<MessageType>(channel, message);
					}
					CheckAliveConnections();
				}
				catch (const std::runtime_error &e1)
				{
					ApiConnections.Remove(connection);
					if (connection != nullptr)
					{
						connection->Close();
					}
					CheckAliveConnections();
				}
			}
		}

	private:
		void CheckAliveConnections();

		/// <summary>
		/// Start listening for API commands
		/// </summary>
	public:
		void Run();

		/// <summary>
		/// Trigger a callback when the server is ready for responding to client commands
		/// </summary>
		/// <param name="action">Callback</param>
		void OnReady(std::function<void()> action);

		/// <summary>
		/// Trigger all on ready events
		/// </summary>
	private:
		void TriggerOnReady();

	public:
		void RemoveConnection(const std::wstring &Identifer);

		void OnOpen(std::function<void(ApiConnection*)> action);

		/// <summary>
		/// Trigger all on open events
		/// </summary>
	private:
		void TriggerOnOpen(ApiConnection *connection);
	};
}
