#include "pch.h"
#include "AccountData.h"
using namespace PhoneDirect3DXamlAppComponent;
AccountData::AccountData(StreamBuffer^ buf){
	AccountID = buf->readUInt64();
	HighScore = buf->readInt32();
	TotalCoin = buf->readInt32();
	Location = buf->readInt16();
	UserName = buf->readSStr();
	AvatarUrl = buf->readSStr();
	Data = buf->readIBin();
}
StreamBuffer^ AccountData::getBinary(){
	StreamBuffer^ rs = ref new StreamBuffer(calcBinSize());
	rs->writeUInt64(AccountID);
	rs->writeInt32(HighScore);
	rs->writeInt32(TotalCoin);
	rs->writeInt16(Location);
	rs->writeSStr(UserName);
	rs->writeSStr(AvatarUrl);
	rs->writeIBin(Data);
	return rs;
}

void AccountData::SetAvatarURL(Platform::String^ url)
{
	AvatarUrl = url;
}
void AccountData::SetUserName(Platform::String^ username)
{
	UserName = username;
}
void AccountData::SetTotalCoin(int totalCoin)
{
	TotalCoin = totalCoin;
}
void AccountData::SetAccountID(UINT64 accid)
{
	AccountID = accid;
}
void AccountData::SetHighScore(int score)
{
	HighScore = score;
}
void AccountData::SetLocation(int16 location)
{
	Location = location;
}
void AccountData::SetData(const Platform::Array<uint8>^ data)
{
	Data = ref new Platform::Array<uint8>(10000);
	for (int k = 0; k < 10000; k++)
	{
		Data[k] = data[k];
	}
}