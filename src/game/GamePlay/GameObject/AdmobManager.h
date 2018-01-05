#if !defined(_H_ADMOB_MANAGER)
#define _H_ADMOB_MANAGER
#include "../../Lib/DataType/CType.h"
#include "../../GameCore/Os_Functions.h"

class AdmobManager{
private:
	PineServiceHandle _svc_interstitial_prepare, _svc_interstitial_show;
	bool _is_request_prepare;
public :
	AdmobManager();
	void update();
	void prepareInterstitial();
	void showInterstitial();
};
#endif