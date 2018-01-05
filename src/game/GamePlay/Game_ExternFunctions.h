//Game Data
//#include "../GameCore/CType.h"
#if defined(_WINDOWS_PHONE8)
extern "C" void AsynCheckServerRealIsAble(PineServiceHandle* handle);
extern "C" int AsynCheckDataFromPineServer(PineServiceHandle* handle, UINT64 account_id);
extern "C" void AsynCreateNewAccount(PineServiceHandle* handle);
extern "C" void AsynSyncSaveToServer(PineServiceHandle* handle);
extern "C" void AsynSyncSaveFromServer(PineServiceHandle* handle);
#endif
