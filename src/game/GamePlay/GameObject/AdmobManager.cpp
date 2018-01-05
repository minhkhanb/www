#include "pch.h"
#include "AdmobManager.h"
#include "../../Define/Defines.h"
AdmobManager::AdmobManager(){
	_svc_interstitial_prepare.reset();
	_is_request_prepare = false;
}
void AdmobManager::update(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::WaitingResponse){
		OS_AsynAdmobInterstitialPrepare(&_svc_interstitial_prepare, false, ADMOB_INTERSTITIAL_ID);
		//if (_svc_interstitial_prepare.isFinish()){
		//	showInterstitial();
		//}
	}
}
void AdmobManager::prepareInterstitial(){
	if (_svc_interstitial_prepare.Status != PineServiceResult::WaitingResponse){
		_svc_interstitial_prepare.reset();
		OS_AsynAdmobInterstitialPrepare(&_svc_interstitial_prepare, false, ADMOB_INTERSTITIAL_ID);
	}
}
void AdmobManager::showInterstitial(){
	if (_svc_interstitial_prepare.Status == PineServiceResult::Responsed && _svc_interstitial_prepare.ResponseStatus == 0){
		_svc_interstitial_show.reset();
		OS_AsynAdmobInterstitialShow(&_svc_interstitial_show);
		prepareInterstitial();
	}
}