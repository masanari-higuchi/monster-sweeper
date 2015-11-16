package net.nend.NendModule;

import net.nend.android.NendAdInterstitial;
import net.nend.android.NendAdInterstitial.NendAdInterstitialClickType;
import net.nend.android.NendAdInterstitial.NendAdInterstitialShowResult;
import net.nend.android.NendAdInterstitial.NendAdInterstitialStatusCode;
import net.nend.android.NendAdInterstitial.OnCompleationListener;

import org.cocos2dx.lib.Cocos2dxActivity;

import android.app.Activity;

public class NendInterstitialModule {	

	public static void createInterstitial(final String apiKey, final String spotID) {
		
		final Activity activity = (Activity)Cocos2dxActivity.getContext();

		activity.runOnUiThread(new Runnable() {
			public void run() {

				int intSpotID = Integer.parseInt(spotID);
				
				NendAdInterstitial.loadAd(activity, apiKey, intSpotID);
				// 広告ロード結果の通知を受け取るようにする
				NendAdInterstitial.setListener(new OnCompleationListener(){
					
					@Override
					public void onCompleation(NendAdInterstitialStatusCode statusCode) {
						int resultCode = statusCode.ordinal();
						loadResultStatus(resultCode);	
					}					
					
				});
			}
		});
	}
	
	// インタースティシャル広告の表示
	public static void showInterstitialView() {
		
		final Activity activity = (Activity)Cocos2dxActivity.getContext();
		activity.runOnUiThread(new Runnable() {
			public void run() {
				
				// クリックイベントのタイプが返却される
				NendAdInterstitial.OnClickListener listener = new NendAdInterstitial.OnClickListener() {
					
					@Override
					public void onClick(NendAdInterstitialClickType clickType) {
						int resultCode = clickType.ordinal();
						onClickStatus(resultCode);
					}
				};
				
                // 表示結果が返却される				
                NendAdInterstitialShowResult result = NendAdInterstitial.showAd(activity, listener);
                int resultCode = result.ordinal();
                showResultStatus(resultCode);
			}
		});
	}
	
	// インタースティシャル広告の削除
	public static void dismissNADInterstitialView(){
		final Activity activity = (Activity)Cocos2dxActivity.getContext();
		activity.runOnUiThread(new Runnable() {
			public void run() {
				
				boolean dismissResult = NendAdInterstitial.dismissAd();
                // 広告削除結果で何かの処理する場合
				if (dismissResult){
				}
			}
		});
	}
	
	// 終了時広告表示
	public static void showFinishNADInterstitialView(){
		final Activity activity = (Activity)Cocos2dxActivity.getContext();
		activity.runOnUiThread(new Runnable() {
			public void run() {
				
				// クリックイベントのタイプが返却される
				NendAdInterstitial.OnClickListener listener = new NendAdInterstitial.OnClickListener() {
					
					@Override
					public void onClick(NendAdInterstitialClickType clickType) {
						
						int resultCode = clickType.ordinal();
						onClickStatus(resultCode);
					}
				};
				
				// 終了時広告を表示
				NendAdInterstitial.showFinishAd(activity, listener);
				
			}
		});
	}
	
	public static native void loadResultStatus(final int statusCode);
	public static native void showResultStatus(final int statusCode);
	public static native void onClickStatus(final int statusCode);
	
}
