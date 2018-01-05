#if !defined(_H_ACCOUNT_MANAGER)
#define _H_ACCOUNT_MANAGER
#include "../../Lib/DataType/CType.h"
class AccountManager{
public:
	bool IsLogin;
	PineAccountType Type;
	PineAccountInfo Info;
	AccountManager();
};
#endif