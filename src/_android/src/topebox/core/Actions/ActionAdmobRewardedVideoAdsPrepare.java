//Pine-entertainment
//Android Framework
//duy.nguyen 2014 - 2015

package topebox.core.Actions;

import android.app.AlertDialog;
import android.content.DialogInterface;
import android.content.Context;
import java.lang.*;
import android.app.Activity;
import android.util.Log;
import android.view.ContextThemeWrapper;

// import com.unity3d.ads.android.UnityAds;

import topebox.core.AdmobManager;

public class ActionAdmobRewardedVideoAdsPrepare implements Action {
	ActionAdmobRewardedVideoAdsPrepareArg Arg = null;
	Activity CurrentActivity = null;

	public ActionAdmobRewardedVideoAdsPrepare(ActionAdmobRewardedVideoAdsPrepareArg arg, Activity activity) {
		Arg = arg;
		CurrentActivity = activity;
	}

	public void act() {
		if (Arg == null || CurrentActivity == null) {
			return;
		}
		Arg.onBegin();
		try {
			AdmobManager.LoadRewardedVideo();
		} catch (Exception ex) {
			Log.i("DEBUG", "Prepare Admob rewarded video Ads fail with exception: " + ex.getMessage());
		}
	}
}