// NendInterstitialModule.cpp

#include "NendInterstitialModule.h"

// static
NendInterstitialModule* NendInterstitialModule::m_mySingleton = nullptr;

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
NendInterstitialModule::NendInterstitialModule()
{
}

////////////////////////////////////////////////////////////////////////////////
// -----------------------------------------------------------------------------
NendInterstitialModule* NendInterstitialModule::sharedNendInterstitialModule()
{
    if (m_mySingleton == nullptr) {
        m_mySingleton = new NendInterstitialModule();
    }
    return m_mySingleton;
}

// -----------------------------------------------------------------------------
void NendInterstitialModule::createNADInterstitial(char* apiKey, char* spotID)
{
}

// -----------------------------------------------------------------------------
void NendInterstitialModule::showNADInterstitialView()
{
}

// -----------------------------------------------------------------------------
void NendInterstitialModule::dismissNADInterstitialView()
{
}

/******************************************************************************/
//	End Of File
/******************************************************************************/