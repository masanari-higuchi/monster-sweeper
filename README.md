# monster-sweeper

**AppStore** : <https://itunes.apple.com/tr/app/monsutasuipa/id903936139?mt=8>  
**GooglePlay** : <https://play.google.com/store/apps/details?id=com.starnyacs.monstersweeper>

---
###■Windows

[環境]  
Visual Studio 2013

[実行手順]  
VisualStudioで『proj.win32/monster-sweeper.sln』を起動  
monster-sweeperプロジェクトを実行

---
###■iOS

[環境]  
XCode 7.0.1  

[実行手順]  
XCodeで『proj.ios_mac/monster-sweeper.xcodeproj』を起動  
TARGETを『monster-sweeper iOS』に変更  
Simulatorを選択して実行

---
###■Android

[環境]  
Eclipse JUNO 4.2.1(ADT,CDT導入)  
Android NDK r9d  
Python 2.7.10  

[実行手順]  
Eclipseで以下のAndroidプロジェクトをインポート  
 『cocos2d/cocos/platform/android/java』  
 『google-play-services_lib』  
 『proj.android』  
 
monster-sweeperプロジェクトメニューから、  
『Project->Properties->C/C++Build->Environment』に以下の設定を追加  
 Name : NDK_ROOT  Value : NDKへのパス  

monster-sweeperプロジェクトを実行


[AppStore]: https://itunes.apple.com/tr/app/monsutasuipa/id903936139?mt=8
