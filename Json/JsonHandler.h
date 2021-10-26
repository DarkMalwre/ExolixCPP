#pragma once

#include <string>

using namespace Newtonsoft::Json;

namespace Exolix::Json
{
	class JsonHandler
	{
	public:
		template<typename DocumentType>
		static DocumentType Parse(const std::wstring &jsonString)
		{
//C# TO C++ CONVERTER TODO TASK: There is no C++ equivalent to the C# 'null-forgiving operator':
//ORIGINAL LINE: return JsonConvert.DeserializeObject<DocumentType>(jsonString, new JsonSerializerSettings() { TypeNameHandling = TypeNameHandling.Auto })!;
			JsonSerializerSettings *tempVar = new JsonSerializerSettings();
			tempVar->TypeNameHandling = TypeNameHandling::Auto;

			delete tempVar;
			return JsonConvert::DeserializeObject<DocumentType>(jsonString, tempVar);
		}

		template<typename DocumentType>
		static std::wstring Stringify(DocumentType json)
		{
			return JsonConvert::SerializeObject(json);
		}
	};
}
