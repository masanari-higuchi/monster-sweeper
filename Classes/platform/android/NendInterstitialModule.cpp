//  NendInterstitialModule.cpp

#include "NendInterstitialModule.h"
#include <jni.h>
#include "platform/android/jni/JniHelper.h"

#define CLASS_NAME "net/nend/NendModule/NendInterstitialModule"

//シングルトンの設定
NendInterstitialModule* NendInterstitialModule::m_mySingleton = NULL;

NendInterstitialModule::NendInterstitialModule()
{
}

NendInterstitialModule* NendInterstitialModule::sharedNendInterstitialModule(){

    if (NULL == m_mySingleton) {
        //クラス未生成の場合は生成する
        m_mySingleton = new NendInterstitialModule();
    }

    return m_mySingleton;
}

// NendInterstitialの生成
void NendInterstitialModule::createNADInterstitial(char* apiKey, char* spotID){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "createInterstitial", "(Ljava/lang/String;Ljava/lang/String;)V")) {
		jstring stringApiKey = t.env->NewStringUTF(apiKey);
		jstring stringSpotID = t.env->NewStringUTF(spotID);
		t.env->CallStaticVoidMethod(t.classID, t.methodID, stringApiKey, stringSpotID);
		t.env->DeleteLocalRef(stringApiKey);
		t.env->DeleteLocalRef(stringSpotID);
		t.env->DeleteLocalRef(t.classID);
	}
}

// NendInterstitialの表示
void NendInterstitialModule::showNADInterstitialView(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showInterstitialView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

// NendInterstitialの削除
void NendInterstitialModule::dismissNADInterstitialView(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "dismissNADInterstitialView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

// NendInterstitialの終了時広告表示
void NendInterstitialModule::showFinishNADInterstitialView(){
	cocos2d::JniMethodInfo t;
	if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "showFinishNADInterstitialView", "()V")) {
		t.env->CallStaticVoidMethod(t.classID, t.methodID);
		t.env->DeleteLocalRef(t.classID);
	}
}

//callback処理
extern "C"
{
	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendInterstitialModule_loadResultStatus(JNIEnv* env, jobject thiz, jint statusCode) {
		// 広告のロード結果のEventDispatcher通知
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateLoadResultNotification, &statusCode);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendInterstitialModule_showResultStatus(JNIEnv* env, jobject thiz, jint statusCode) {
		// 広告の表示結果のEventDispatcher通知
		int num = statusCode;
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateShowResultNotification, &statusCode);
	}

	JNIEXPORT void JNICALL Java_net_nend_NendModule_NendInterstitialModule_onClickStatus(JNIEnv* env, jobject thiz, jint statusCode) {
		// 広告のクリックタイプのEventDispatcher通知
        Director::getInstance()->getEventDispatcher()->dispatchCustomEvent(NADInterstitialDelegateClickResultNotification, &statusCode);
	}
}
