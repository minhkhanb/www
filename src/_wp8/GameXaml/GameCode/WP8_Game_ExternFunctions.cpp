#include "pch.h"
#include "../../../../game/GameCore/CGame.h"
#include "../../../../game/GameCore/Os_Functions.h"
#include "../SourceCPP/ServerStream/ServerStreamManager.h"
#include "../SourceCPP/StreamBuffer.h"
#include "../_wp8/GameXaml/GameCode/AccountData.h"
using namespace PhoneDirect3DXamlAppComponent;

#define REQUEST_CODE_CHECK_SERVER					1001   
#define REQUEST_CODE_CHECK_ACCOUNT					1002   
#define REQUEST_CODE_CREATE_USER					1003  
#define REQUEST_CODE_SYNC_TO_SERVER					1004
#define REQUEST_CODE_SYNC_FROM_SERVER				1005
#define REQUEST_CODE_GET_HIGH_SCORE_GLOBAL			1006
#define REQUEST_CODE_GET_HIGH_SCORE_FRIEND			1007
#define REQUEST_CODE_GET_RANK_CLOBAL				1008
#define REQUEST_CODE_LOAD_UPDATE					1009


extern "C" void AsynCheckServerRealIsAble(PineServiceHandle* handle)
{
	if (handle->isFree())
	{
		/*request check server*/
		handle->Handle = ServerStreamManager::Request(REQUEST_CODE_CHECK_SERVER, nullptr);
		handle->Status = PineServiceResult::WaitingResponse;
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			int errorcode = stream->ErrorCode;
			/*0 = use serverReal
			1 = use serverDev
			2 = not network
			*/
			switch (errorcode)
			{
			case 2:
				handle->Status = PineServiceResult::Responsed;// SVRESULT::SV_DATA_ERROR_UNKNOW;
				handle->ResponseStatus = 2;
				break;
			default:
				handle->Status = PineServiceResult::Responsed;// SVRESULT::SV_DATA_SUCCESS;
				handle->ResponseStatus = 0;
				break;
			}
			ServerStreamManager::freeStream(handle->Handle);
		}
		else
		{
			handle->Status = PineServiceResult::WaitingResponse;//SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
}
extern "C" int AsynCheckDataFromPineServer(PineServiceHandle* handle, UINT64 account_id)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	int ret = CHECKING_STATUS_NORMAL;
	if (handle->isFree())
	{
		StreamBuffer^ buffer = ref new StreamBuffer(8);
		buffer->writeUInt64(account_id);
		handle->Handle = ServerStreamManager::Request(REQUEST_CODE_CHECK_ACCOUNT, buffer->Data);
		handle->Status =PineServiceResult::WaitingResponse;
	}
	else if (handle->Handle > -1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
			int errorcode = stream->ErrorCode;
			/*
			* error code = 0 : use serverReal
			* error code = 1 : use serverDev
			* error code = 2 : not network
			*/
			switch (errorcode)
			{
			case 1:
				
				handle->ResponseStatus = CHECKING_STATUS_NO_ACCOUNT;
				handle->Status = PineServiceResult::Responsed;
				break;
			case 2:
				
				handle->ResponseStatus = CHECKING_STATUS_EXCEPTION;
				handle->Status = PineServiceResult::Responsed; //SVRESULT::SV_DATA_ERROR_UNKNOW;
				break;
			case 0:
			{
					  AccountData^ acctmp = ref new AccountData(ref new StreamBuffer(stream->ResponseData));
					  char buffer[MAX_SAVE_BUFFER_CACHE];
					  for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
					  {
						  buffer[i] = acctmp->Data[i];
					  }
					  INT_TIME time_play = GAME()->GetInt64At(buffer, SAVE_FORMAT_TIMEPLAY);
					  INT_TIME owner_save = GAME()->GetInt64At(buffer, SAVE_FORMAT_OWNNER);
					  //PDEBUG("\n _owerSave = %lld --- time_play = %lld", GET_MY_CHAR()._owerSave, GET_MY_CHAR()._timePlay);
					  handle->ResponseStatus = CHECKING_STATUS_NORMAL;
					  if (owner_save != game->_user._ower_save )
					  {
						  if (game->_user._ower_save == 0)
						  {
							  handle->ResponseStatus = CHECKING_STATUS_OLD_DATA;
						  }
						  else
						  {
							  handle->ResponseStatus = CHECKING_STATUS_SWAP_ACCOUNT;
						  }
						  
					  }
					  else if (time_play > game->_user._timePlay)
					  {
						  
					  }
					  handle->Status = PineServiceResult::Responsed;//SVRESULT::SV_DATA_SUCCESS;
			}
				break;
			default:
				
				handle->ResponseStatus =  CHECKING_STATUS_EXCEPTION;
				handle->Status = PineServiceResult::Responsed;//SVRESULT::SV_DATA_ERROR_UNKNOW;
				break;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->ResponseStatus = CHECKING_STATUS_EXCEPTION;
			handle->Status = PineServiceResult::WaitingResponse;//SVRESULT::SV_DATA_WAITING_RESPONSE;
		}
	}
	return handle->ResponseStatus;
}
extern "C" void AsynCreateNewAccount(PineServiceHandle* handle)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	if (handle->isFree())
	{
		AccountData^ acc = ref new AccountData();
		acc->SetAvatarURL(Utils::ConvertCharToString(game->_user._avatarURL));
		acc->SetUserName(Utils::ConvertCharToString(game->_user._userName));
		acc->SetHighScore(game->_user._highScore);
		acc->SetTotalCoin(game->_user._total_coin);
		acc->SetAccountID(game->_user._userId);
		acc->SetLocation(game->_user._lvlLocation);
		Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(MAX_SAVE_BUFFER_CACHE);
		for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
		{
			data[i] = game->_user._buffer[i];
		}
		acc->SetData(data);
		StreamBuffer^ buffer = ref new StreamBuffer(acc->calcBinSize());
		buffer = acc->getBinary();
		handle->Handle = ServerStreamManager::Request(REQUEST_CODE_CREATE_USER, buffer->Data);
		handle->Status = PineServiceResult::WaitingResponse;//SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle>-1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if (!ServerStreamManager::isError(handle->Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				if (stream->ErrorCode == 1)//no record
				{
					handle->Status = PineServiceResult::Responsed;//SVRESULT::SV_DATA_ERROR_NO_RECORD;
				}
				else
				{
					handle->Status = PineServiceResult::Responsed;
				}
			}
			else
			{
				handle->Status = PineServiceResult::Responsed;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = PineServiceResult::WaitingResponse;
		}
	}
}

extern "C" void AsynSyncSaveToServer(PineServiceHandle* handle)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;

	if (handle->isFree())
	{
		AccountData^ acc = ref new AccountData();
		acc->SetAccountID(game->_user._userId);
		acc->SetHighScore(game->_user._highScore);
		acc->SetTotalCoin(game->_user._total_coin);
		acc->SetLocation(game->_user._lvlLocation);
		acc->SetUserName(Utils::ConvertCharToString(game->_user._userName));
		acc->SetAvatarURL(Utils::ConvertCharToString(game->_user._avatarURL));
		Platform::Array<uint8>^ data = ref new Platform::Array<uint8>(MAX_SAVE_BUFFER_CACHE);
		for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
		{
			data[i] = game->_user._buffer[i];
		}
		acc->SetData(data);
		StreamBuffer^ buffer = ref new StreamBuffer(acc->calcBinSize());
		buffer = acc->getBinary();
		handle->Handle = ServerStreamManager::Request(REQUEST_CODE_SYNC_TO_SERVER, buffer->Data);
		handle->Status = PineServiceResult::WaitingResponse;//SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle>-1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if (!ServerStreamManager::isError(handle->Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				if (stream->ErrorCode == 1)//no record
				{
					handle->Status = PineServiceResult::Responsed;
					handle->ResponseStatus = 1;
				}
				else
				{
					handle->Status = PineServiceResult::Responsed;
					handle->ResponseStatus = 0;
				}
			}
			else
			{
				handle->Status = PineServiceResult::Responsed;
				handle->ResponseStatus = 1;//error
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = PineServiceResult::WaitingResponse;
		}
	}
}
extern "C" void AsynSyncSaveFromServer(PineServiceHandle* handle)
{
	WildWildWestGame* game = (WildWildWestGame*)GAME()->_current_game;
	if (handle->isFree())
	{
		StreamBuffer^ buffer = ref new StreamBuffer(8);
		buffer->writeUInt64(game->_user._userId);
		handle->Handle = ServerStreamManager::Request(REQUEST_CODE_SYNC_FROM_SERVER, buffer->Data);
		handle->Status = PineServiceResult::WaitingResponse;//SVRESULT::SV_DATA_WAITING_RESPONSE;
	}
	else if (handle->Handle>-1)
	{
		if (ServerStreamManager::isStreamComplete(handle->Handle))
		{
			if (!ServerStreamManager::isError(handle->Handle))
			{
				ServerStream^ stream = ServerStreamManager::getStream(handle->Handle);
				AccountData^ acc = ref new AccountData(ref new StreamBuffer(stream->ResponseData));
				if (stream->ErrorCode == 1)//no record
				{
					handle->Status = PineServiceResult::Responsed;//SVRESULT::SV_DATA_ERROR_NO_RECORD;
					handle->ResponseStatus = 1;
				}
				else if (stream->ErrorCode == 0)
				{
					if (acc->AvatarUrl != nullptr)
					{
						sprintf(game->_user._avatarURL, Utils::ConvertStringToCharArray(acc->AvatarUrl));
					}
					if (acc->UserName != nullptr)
					{
						sprintf(game->_user._userName, Utils::ConvertStringToCharArray(acc->UserName));
					}
					game->_user._highScore = acc->HighScore;
					game->_user._total_coin = acc->TotalCoin;
					game->_user._userId = acc->AccountID;
					game->_user._lvlLocation = acc->Location;
					for (int i = 0; i < MAX_SAVE_BUFFER_CACHE; i++)
					{
						game->_user._buffer[i] = acc->Data[i];
					}
					game->_user._coin=GAME()->GetInt32At(game->_user._buffer, SAVE_FORMAT_COIN);
					handle->Status = PineServiceResult::Responsed;//SVRESULT::SV_DATA_SUCCESS;
					handle->ResponseStatus = 0;
				}
				else
				{
					handle->Status = PineServiceResult::Responsed;
					handle->ResponseStatus = 1;
				}
			}
			else
			{
				handle->Status = PineServiceResult::WaitingResponse;//SVRESULT::SV_DATA_WAITING_RESPONSE;
			}
			ServerStreamManager::freeStream(handle->Handle);
			handle->Handle = -1;
		}
		else
		{
			handle->Status = PineServiceResult::WaitingResponse;
		}
	}
}