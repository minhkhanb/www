#include "pch.h"
#ifndef H_ACCOUNT_DATA
#define H_ACCOUNT_DATA
#include "../SourceCPP/StreamBuffer.h"
namespace PhoneDirect3DXamlAppComponent
{
	[Windows::Foundation::Metadata::WebHostHiddenAttribute]
	public ref class AccountData sealed
	{
	public:
		AccountData(){};
		AccountData(StreamBuffer^ buffer);
		property uint64 AccountID;
		property int HighScore;
		property int TotalCoin;
		property int16 Location;
		property Platform::String^ UserName;
		property Platform::String^ AvatarUrl;
		property Platform::Array<uint8>^ Data;
		int calcBinSize()
		{
			int size = 0;
			size += 8; // account ID
			size += 4; // high score
			size += 4; // Total Coin
			size += 2;//Location
			//User Name
			if (UserName != nullptr){
				size += 2 + 2 * UserName->Length();
			}
			else{
				size += 2;
			}
			//Avatar Url
			if (AvatarUrl != nullptr){
				size += 2 + 2 * AvatarUrl->Length();
			}
			else{
				size += 2;
			}
			// Data
			if (Data != nullptr){
				size += 4 + Data->Length;
			}
			else{
				size += 4;
			}
			return size;
		};
		StreamBuffer^ getBinary();
		void SetHighScore(int score);
		void SetAccountID(uint64 accid);
		void SetTotalCoin(int totalCoin);
		void SetLocation(int16 location);
		void SetAvatarURL(Platform::String^ url);
		void SetUserName(Platform::String^ username);
		void SetData(const Platform::Array<uint8>^ data);
	};
}

#endif