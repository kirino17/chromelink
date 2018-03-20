#include "stdafx.h"
#include "ThrowInterface.h"
#include "eclass.h"
#include <time.h>
#include <ctime>

using namespace Komari;

#define TO_AVGR_STRING(x) x!=NULL && *x!='\0'?A2W(x):TEXT("")

void APIENTRY chrome_class_accessor_new(LPVOID eclass)
{
	eclass::SetAccessorClass((DWORD**)eclass);
}

void APIENTRY chrome_class_acccessor_free(Komari::IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		pAccessor->Release();
	}
}

LPVOID APIENTRY chrome_class_accessor_copy(LPVOID object)
{
	DWORD *pEClass = (DWORD*)object;
	Komari::IChromeLink *pAccessor = (Komari::IChromeLink*)pEClass[1];
	if (pAccessor != NULL)
	{
		pAccessor->AddRef();
		return (LPVOID)pAccessor;
	}
	return NULL;
}

BOOL APIENTRY chrome_accessor_link_start(const char *ws,LPVOID object)
{
	USES_CONVERSION;
	if (ws[0] != '\0' && object != NULL)
	{
		IChromeLink *pKeepLink = new IChromeLink(A2W(ws));
		if (pKeepLink->LinkStart())
		{
			((DWORD*)object)[1] = (DWORD)pKeepLink;
			return TRUE;
		}
		else
		{
			pKeepLink->Close();
			pKeepLink->Release();
		}
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_link_start2(const char *ws)
{
	USES_CONVERSION;
	if (ws!=NULL && ws[0] != '\0')
	{
		IChromeLink *pKeepLink = new IChromeLink(A2W(ws));
		if (pKeepLink->LinkStart())
		{
			return pKeepLink;
		}
		else
		{
			pKeepLink->Close();
			pKeepLink->Release();
		}
	}
	return NULL;
}

void APIENTRY chrome_accessor_close(Komari::IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		pAccessor->Close();
	}
}

BOOL APIENTRY chrome_accessor_islinked(Komari::IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->IsLinked();
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_get_main_frame(Komari::IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return (LPVOID)&pAccessor->GetMainFrame();
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_frame_id(IChromeFrameId *pFrameId,int *pLength)
{
	if (pFrameId != NULL && pLength != NULL)
	{
		if (pFrameId->strFrameId.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pFrameId->strFrameId.c_str(), pFrameId->strFrameId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pFrameId->strFrameId.c_str(), pFrameId->strFrameId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_frame_loaderid(IChromeFrameId *pFrameId, int *pLength)
{
	if (pFrameId != NULL && pLength != NULL)
	{
		if (pFrameId->strLoaderId.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pFrameId->strLoaderId.c_str(), pFrameId->strLoaderId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pFrameId->strLoaderId.c_str(), pFrameId->strLoaderId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_frame_mimetype(IChromeFrameId *pFrameId, int *pLength)
{
	if (pFrameId != NULL && pLength != NULL)
	{
		if (pFrameId->strMimeType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pFrameId->strMimeType.c_str(), pFrameId->strMimeType.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pFrameId->strMimeType.c_str(), pFrameId->strMimeType.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_frame_name(IChromeFrameId *pFrameId, int *pLength)
{
	if (pFrameId != NULL && pLength != NULL)
	{
		if (pFrameId->strName.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pFrameId->strName.c_str(), pFrameId->strName.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pFrameId->strName.c_str(), pFrameId->strName.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_frame_parentid(IChromeFrameId *pFrameId, int *pLength)
{
	if (pFrameId != NULL && pLength != NULL)
	{
		if (pFrameId->strParentId.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pFrameId->strParentId.c_str(), pFrameId->strParentId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pFrameId->strParentId.c_str(), pFrameId->strParentId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_frame_url(IChromeFrameId *pFrameId, int *pLength)
{
	if (pFrameId != NULL && pLength != NULL)
	{
		if (pFrameId->strUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pFrameId->strUrl.c_str(), pFrameId->strUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pFrameId->strUrl.c_str(), pFrameId->strUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
		else
		{
			return NULL;
		}
	}
	return NULL;
}

void APIENTRY chrome_struct_frame_free(IChromeFrameId *frameId)
{
	if (frameId != NULL)
	{
		delete frameId;
	}
}

int APIENTRY chrome_accessor_get_child_frame_count(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->GetFrameCount();
	}
	return 0;
}

LPVOID APIENTRY chrome_accessor_get_child_frame(IChromeLink *pAccessor, int index)
{
	if (pAccessor != NULL)
	{
		return (LPVOID)&pAccessor->GetChildFrame(index);
	}
	return NULL;
}

int APIENTRY chrome_accessor_get_execution_contextid_count(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->GetExecutionContextCount();
	}
	return 0;
}

LPVOID APIENTRY chrome_accessor_get_execution_contextid(IChromeLink *pAccessor, int index)
{
	if (pAccessor != NULL)
	{
		return (LPVOID)&pAccessor->GetExecutionContextId(index);
	}
	return NULL;
}

DWORD APIENTRY chrome_struct_execution_contextid(IChromeExecutionContextId *pExecutionContextId)
{
	if (pExecutionContextId != NULL)
	{
		return pExecutionContextId->dwExecutionContextId;
	}
	return 0;
}

LPVOID APIENTRY chrome_struct_execution_frameid(IChromeExecutionContextId *pExecutionContextId,int *pLength)
{
	if (pExecutionContextId != NULL && pLength!=NULL)
	{
		if (pExecutionContextId->strFrameId.length() > 0)
		{
			
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pExecutionContextId->strFrameId.c_str(), pExecutionContextId->strFrameId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pExecutionContextId->strFrameId.c_str(), pExecutionContextId->strFrameId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

void APIENTRY chrome_struct_execution_free(IChromeExecutionContextId *pExecutionContextId)
{
	if (pExecutionContextId != NULL)
	{
		delete pExecutionContextId;
	}
}

BOOL APIENTRY chrome_accessor_evaluate(IChromeLink *pAccessor,const char *szExp,const char *szGroupName,BOOL bIncludeCommandLineAPI,BOOL bSilent,DWORD dwExecutionContextId,BOOL bReturnByValue,BOOL bGeneratePreview,BOOL bUserGesture,BOOL bAwaitPromise, IChromeEvalResult **pResult)
{
	if (pAccessor != NULL && szExp!=NULL && szGroupName!=NULL)
	{
		*pResult = NULL;

		IChromeEvalResult *pTempResult = new IChromeEvalResult;
		int nExpLength = strlen(szExp);
		int nInExpLength = MultiByteToWideChar(CP_ACP, NULL, szExp, nExpLength, NULL, NULL);
		wchar_t *wszExpBuffer = new wchar_t[nInExpLength + 1];
		MultiByteToWideChar(CP_ACP, NULL, szExp, nExpLength, wszExpBuffer, nInExpLength);
		wszExpBuffer[nInExpLength] = 0;
		USES_CONVERSION;
		BOOL bRetVal = pAccessor->Evaluate(wszExpBuffer, A2W(szGroupName), bIncludeCommandLineAPI, bSilent, dwExecutionContextId, bReturnByValue, bGeneratePreview, bUserGesture, bAwaitPromise, *pTempResult);
		delete[]wszExpBuffer;
		if (bRetVal == FALSE)
		{
			delete pTempResult;
			return FALSE;
		}
		*pResult = pTempResult;
		return TRUE;
	}
	return FALSE;
}

LPVOID APIENTRY chrome_struct_eval_result_desc(IChromeEvalResult *pResult,int *pLength)
{
	if (pResult != NULL)
	{
		if (pResult->strDescription.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pResult->strDescription.c_str(), pResult->strDescription.length(), NULL, NULL, NULL, NULL);
			char *szOutBUffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pResult->strDescription.c_str(), pResult->strDescription.length(), szOutBUffer, nOutLength, NULL, NULL);
			szOutBUffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBUffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_eval_result_type(IChromeEvalResult *pResult, int *pLength)
{
	if (pResult != NULL)
	{
		if (pResult->strType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pResult->strType.c_str(), pResult->strType.length(), NULL, NULL, NULL, NULL);
			char *szOutBUffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pResult->strType.c_str(), pResult->strType.length(), szOutBUffer, nOutLength, NULL, NULL);
			szOutBUffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBUffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_eval_result_subtype(IChromeEvalResult *pResult, int *pLength)
{
	if (pResult != NULL)
	{
		if (pResult->strSubType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pResult->strSubType.c_str(), pResult->strSubType.length(), NULL, NULL, NULL, NULL);
			char *szOutBUffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pResult->strSubType.c_str(), pResult->strSubType.length(), szOutBUffer, nOutLength, NULL, NULL);
			szOutBUffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBUffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_eval_result_classname(IChromeEvalResult *pResult, int *pLength)
{
	if (pResult != NULL)
	{
		if (pResult->strClassName.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pResult->strClassName.c_str(), pResult->strClassName.length(), NULL, NULL, NULL, NULL);
			char *szOutBUffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pResult->strClassName.c_str(), pResult->strClassName.length(), szOutBUffer, nOutLength, NULL, NULL);
			szOutBUffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBUffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_eval_result_textvalue(IChromeEvalResult *pResult, int *pLength)
{
	if (pResult != NULL)
	{
		if (pResult->strValue.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pResult->strValue.c_str(), pResult->strValue.length(), NULL, NULL, NULL, NULL);
			char *szOutBUffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pResult->strValue.c_str(), pResult->strValue.length(), szOutBUffer, nOutLength, NULL, NULL);
			szOutBUffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBUffer;
		}
	}
	return NULL;
}

double APIENTRY chrome_struct_eval_result_numbervalue(IChromeEvalResult *pResult)
{
	if (pResult != NULL)
	{
		return pResult->flValue;
	}
	return NULL;
}

int APIENTRY chrome_struct_eval_result_intvalue(IChromeEvalResult *pResult)
{
	if (pResult != NULL)
	{
		return pResult->nValue;
	}
	return NULL;
}

BOOL APIENTRY chrome_struct_eval_result_boolvalue(IChromeEvalResult *pResult)
{
	if (pResult != NULL)
	{
		return pResult->bValue;
	}
	return NULL;
}

BOOL APIENTRY chrome_struct_eval_result_exception(IChromeEvalResult *pResult)
{
	if (pResult != NULL)
	{
		return pResult->bException;
	}
	return NULL;
}

void APIENTRY chrome_struct_eval_result_free(IChromeEvalResult *pResult)
{
	if (pResult != NULL)
	{
		delete pResult;
	}
}

BOOL APIENTRY chrome_accessor_release_group_name(IChromeLink *pAccessor,const char *name)
{
	if (pAccessor != NULL && name!=NULL)
	{
		USES_CONVERSION;
		return pAccessor->ReleaseObjectGroup(A2W(name));
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_navigate(IChromeLink *pAccessor,const char *szUrl,const char *szReferrer)
{
	if (pAccessor != NULL && szUrl!=NULL)
	{
		USES_CONVERSION;
		if (szReferrer!=NULL && strlen(szReferrer)>0)
		{
			return pAccessor->Navigate(A2W(szUrl), A2W(szReferrer));
		}
		else
		{
			return pAccessor->Navigate(A2W(szUrl), NULL);
		}
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_reload(IChromeLink *pAccessor, BOOL bIgnoreCache, const char *szScriptToEvaluateOnLoad)
{
	if (pAccessor != NULL)
	{
		if (szScriptToEvaluateOnLoad!=NULL && szScriptToEvaluateOnLoad!='\0')
		{
			int nLength = strlen(szScriptToEvaluateOnLoad);
			int nOutLength = MultiByteToWideChar(CP_ACP, NULL, szScriptToEvaluateOnLoad, nLength, NULL, NULL);
			wchar_t *wszBuffer = new wchar_t[nOutLength + 1];
			MultiByteToWideChar(CP_ACP, NULL, szScriptToEvaluateOnLoad, nLength, wszBuffer, nOutLength);
			wszBuffer[nOutLength] = 0;
			BOOL bRetVal= pAccessor->ReLoad(bIgnoreCache, wszBuffer);
			delete[]wszBuffer;
			return bRetVal;
		}
		else
		{
			return pAccessor->ReLoad(bIgnoreCache, NULL);
		}
		
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_get_resource_content(IChromeLink *pAccessor,const char *szFrameId,const char *szUrl,BOOL *pbBase64Encode,int *pLength)
{
	if (pAccessor != NULL && szFrameId!=NULL && szUrl!=NULL && pbBase64Encode!=NULL)
	{
		USES_CONVERSION;
		IString strContent;
		BOOL bRetVal = pAccessor->GetResourceContent(A2W(szFrameId), A2W(szUrl), strContent, *pbBase64Encode);
		if (bRetVal == FALSE)
		{
			return FALSE;
		}
		if (strContent.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, strContent.c_str(), strContent.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, strContent.c_str(), strContent.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

BOOL APIENTRY chrome_accessor_update_frame(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->UpdateFrame();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_document_content(IChromeLink *pAccessor,const char *szFrameId,const char *szHtml)
{
	if (pAccessor != NULL && szFrameId!=NULL && szHtml!=NULL)
	{
		USES_CONVERSION;
		int nLength = strlen(szHtml);
		int nOutLength = MultiByteToWideChar(CP_ACP, NULL, szHtml, nLength, NULL, NULL);
		wchar_t *wszOutBuffer = new wchar_t[nOutLength + 1];
		MultiByteToWideChar(CP_ACP, NULL, szHtml, nLength, wszOutBuffer, nOutLength);
		wszOutBuffer[nOutLength] = 0;
		BOOL bRetVal = pAccessor->SetDocumentContent(A2W(szFrameId), wszOutBuffer);
		delete[]wszOutBuffer;
		return TRUE;
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_capture_screenshot(IChromeLink *pAccessor,const char *szFormat,int nQuality,BOOL bFromSurface,int *pLength)
{
	if (pAccessor != NULL && pLength!=NULL)
	{
		USES_CONVERSION;
		IString strData;
		BOOL bResult = pAccessor->CaptureScreenshot(A2W(szFormat), nQuality, bFromSurface, strData);
		if (bResult == FALSE || strData.length()<=0)
		{
			return NULL;
		}
		int nOutLength = WideCharToMultiByte(CP_ACP, NULL, strData.c_str(), strData.length(), NULL, NULL, NULL, NULL);
		char *szOutBuffer = (char*)malloc(nOutLength + 1);
		WideCharToMultiByte(CP_ACP, NULL, strData.c_str(), strData.length(), szOutBuffer, nOutLength, NULL, NULL);
		szOutBuffer[nOutLength] = 0;
		*pLength = nOutLength;
		return szOutBuffer;
	}
	return NULL;
}

BOOL APIENTRY chrome_accessor_set_device_metrics_override(IChromeLink *pAccessor,int nWidth, int nHeight, double flDeviceScaleFactor, BOOL bMobile, double flScale, int nScreenWidth, int nScreenHeight, int nPositionX, int nPositionY, BOOL dontSetVisibleSize,const char * szScreenOrientationType,int nScreenOrientationAngle)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;

		return pAccessor->SetDeviceMetricsOverride(nWidth, nHeight, flDeviceScaleFactor, bMobile, flScale, nScreenWidth, nScreenHeight, nPositionX, nPositionY,dontSetVisibleSize,szScreenOrientationType!=NULL&&strlen(szScreenOrientationType)>0?A2W(szScreenOrientationType):NULL,nScreenOrientationAngle);
	}
	return FALSE;
}

BOOL APIENTRY  chrome_accessor_clear_device_metrics_override(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ClearDeviceMetricsOverride();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_force_viewport(IChromeLink *pAccessor,double x, double y, double scale)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ForceViewport(x, y, scale);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_reset_viewport(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ResetViewport();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_reset_page_scale_factor(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ResetPageScaleFactor();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_page_scale_factor(IChromeLink *pAccessor,double flPageScaleFactor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->SetPageScaleFactor(flPageScaleFactor);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_visible_size(IChromeLink *pAccessor,int nWidth, int nHeight)
{
	if (pAccessor != NULL)
	{
		return pAccessor->SetVisibleSize(nWidth, nHeight);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_geolocation_override(IChromeLink *pAccessor,double flLatitude, double flLongitude, double flAccuracy)
{
	if (pAccessor != NULL)
	{
		return pAccessor->SetGeolocationOverride(flLatitude, flLongitude, flAccuracy);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_clear_geolocation_override(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ClearGeolocationOverride();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_touch_emulation_enabled(IChromeLink *pAccessor,BOOL bEnabled,const char* configuration)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->SetTouchEmulationEnabled(bEnabled, A2W(configuration));
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_emulated_media(IChromeLink *pAccessor,const char *szMedia)
{
	if (pAccessor != NULL)
	{
		if (szMedia!=NULL)
		{
			USES_CONVERSION;
			return pAccessor->SetEmulatedMedia(A2W(szMedia));
		}
		else return pAccessor->SetEmulatedMedia(NULL);
	}
	return FALSE;
}

BOOL APIENTRY chrome_acceessor_set_cpu_throttling_rate(IChromeLink *pAccessor,double rate)
{
	if (pAccessor != NULL)
	{
		return pAccessor->SetCPUThrottlingRate(rate);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_can_emulate(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->CanEmulate();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_user_agent_override(IChromeLink *pAccessor,const char* szUserAgent)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->SetUserAgentOverride(A2W(szUserAgent));
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_blocked_urls(IChromeLink *pAccessor,char *szUrls)
{
	if (pAccessor != NULL)
	{
		std::vector<IString> strUrls;
		char *szBegin = szUrls;
		int nLength = 0;
		wchar_t *wszBuffer = NULL;
		while (*szUrls!='\0')
		{
			if (*szUrls == '!')
			{
				*szUrls = '\0';
				nLength = MultiByteToWideChar(CP_ACP, NULL, szBegin, nLength, NULL, NULL);
				wszBuffer = new wchar_t[nLength + 1];
				MultiByteToWideChar(CP_ACP, NULL, szBegin, nLength, wszBuffer, nLength);
				wszBuffer[nLength] = 0;
				strUrls.push_back(wszBuffer);
				delete[]wszBuffer;
				*szUrls = '!';
				szUrls++;
				szBegin = szUrls;
			}
			else
			{
				szUrls++;
			}
		}
		if (strUrls.size() > 0)
		{
			return pAccessor->SetBlockedURLs(strUrls);
		}
		else
		{
			nLength = MultiByteToWideChar(CP_ACP, NULL, szUrls, nLength, NULL, NULL);
			wszBuffer = new wchar_t[nLength + 1];
			MultiByteToWideChar(CP_ACP, NULL, szUrls, nLength, wszBuffer, nLength);
			wszBuffer[nLength] = 0;
			strUrls.push_back(wszBuffer);
			delete[]wszBuffer;
			return pAccessor->SetBlockedURLs(strUrls);
		}
		return FALSE;
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_can_clear_browser_cache(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->CanClearBrowserCache();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_clear_browser_cache(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ClearBrowserCache();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_can_clear_browser_cookies(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->CanClearBrowserCookies();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_clear_browser_cookies(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->ClearBrowserCookies();
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_get_cookies(IChromeLink *pAccessor,const char *szUrl)
{
	if (pAccessor != NULL && szUrl!=NULL)
	{
		USES_CONVERSION;
		IChromeCookieVectors *pRoot=new IChromeCookieVectors;
		if (pAccessor->GetCookies(A2W(szUrl), *pRoot))
		{
			return pRoot;
		}
		delete pRoot;
	}
	return NULL;
}

int APIENTRY chrome_struct_cookies_size(IChromeCookieVectors *pCookie)
{
	if (pCookie != NULL)
	{
		return pCookie->size();
	}
	return 0;
}

LPVOID APIENTRY chrome_struct_cookies_index(IChromeCookieVectors *pCookie,int index)
{
	if (pCookie != NULL)
	{
		return &pCookie->at(index);
	}
	return NULL;
}

void APIENTRY chrome_struct_cookies_free(IChromeCookieVectors *pCookie)
{
	if (pCookie != NULL)
	{
		delete pCookie;
	}
}

LPVOID APIENTRY chrome_struct_cookie_name(IChromeCookie *pCookie,int *pLength)
{
	if (pCookie != NULL)
	{
		if (pCookie->strName.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pCookie->strName.c_str(), pCookie->strName.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pCookie->strName.c_str(), pCookie->strName.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_cookie_value(IChromeCookie *pCookie, int *pLength)
{
	if (pCookie != NULL)
	{
		if (pCookie->strValue.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pCookie->strValue.c_str(), pCookie->strValue.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pCookie->strValue.c_str(), pCookie->strValue.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_cookie_domain(IChromeCookie *pCookie, int *pLength)
{
	if (pCookie != NULL)
	{
		if (pCookie->strDomain.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pCookie->strDomain.c_str(), pCookie->strDomain.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pCookie->strDomain.c_str(), pCookie->strDomain.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_cookie_path(IChromeCookie *pCookie, int *pLength)
{
	if (pCookie != NULL)
	{
		if (pCookie->strPath.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pCookie->strPath.c_str(), pCookie->strPath.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pCookie->strPath.c_str(), pCookie->strPath.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

BOOL APIENTRY chrome_struct_cookie_httponly(IChromeCookie *pCookie)
{
	if (pCookie != NULL)
	{
		return pCookie->bHttpOnly;
	}
	return FALSE;
}

BOOL APIENTRY chrome_struct_cookie_secure(IChromeCookie *pCookie)
{
	if (pCookie != NULL)
	{
		return pCookie->bSecure;
	}
	return FALSE;
}

__int64 APIENTRY chrome_struct_cookie_expires(IChromeCookie *pCookie)
{
	if (pCookie != NULL)
	{
		return pCookie->bSecure;
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_get_all_cookies(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		IChromeCookieVectors *pRoot = new IChromeCookieVectors;
		if (pAccessor->GetAllCookies(*pRoot))
		{
			return pRoot;
		}
		delete pRoot;
	}
	return NULL;
}

BOOL APIENTRY chrome_accessor_delete_cookie(IChromeLink *pAccessor,const char *szUrl,const char *szName)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->DeleteCookie(A2W(szName), A2W(szUrl));
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_set_cookie(IChromeLink *pAccessor,const char *szUrl, const char *szName, const char *szValue, const char *szDomain, const char *szPath, BOOL bSecure,__int64 expires, BOOL bHttpOnly)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->SetCookie(A2W(szUrl), A2W(szName),A2W(szValue),A2W(szDomain),A2W(szPath),bSecure, expires,bHttpOnly);
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_get_resource_list(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		IChromeResourceVectors *objArray = new IChromeResourceVectors;
		if (pAccessor->GetResourceList(*objArray))
		{
			return objArray;
		}
		else
		{
			delete objArray;
		}
	}
	return NULL;
}

void APIENTRY chrome_struct_resource_free(IChromeResourceVectors *pArray)
{
	if (pArray != NULL)
	{
		pArray->clear();
		delete pArray;
	}
}

LPVOID APIENTRY chrome_struct_resource_index(IChromeResourceVectors *pArray,int index)
{
	if (pArray != NULL)
	{
		return (LPVOID)&pArray->at(index);
	}
	return NULL;
}

DWORD APIENTRY chrome_struct_resource_size(IChromeResourceVectors *pArray)
{
	if (pArray != NULL)
	{
		return pArray->size();
	}
	return 0;
}

LPVOID APIENTRY chrome_struct_resource_url(IChromeResource *pSrcData, int *pLength)
{
	if (pSrcData != NULL)
	{
		if (pSrcData->strUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pSrcData->strUrl.c_str(), pSrcData->strUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pSrcData->strUrl.c_str(), pSrcData->strUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_resource_type(IChromeResource *pSrcData, int *pLength)
{
	if (pSrcData != NULL)
	{
		if (pSrcData->strType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pSrcData->strType.c_str(), pSrcData->strType.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pSrcData->strType.c_str(), pSrcData->strType.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_resource_mimetype(IChromeResource *pSrcData, int *pLength)
{
	if (pSrcData != NULL)
	{
		if (pSrcData->strMimeType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pSrcData->strMimeType.c_str(), pSrcData->strMimeType.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pSrcData->strMimeType.c_str(), pSrcData->strMimeType.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_resource_id(IChromeResource *pSrcData, int *pLength)
{
	if (pSrcData != NULL)
	{
		if (pSrcData->strFrameId.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pSrcData->strFrameId.c_str(), pSrcData->strFrameId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pSrcData->strFrameId.c_str(), pSrcData->strFrameId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

DWORD APIENTRY chrome_struct_resource_content_size(IChromeResource *pSrcData)
{
	if (pSrcData != NULL)
	{
		return pSrcData->dwContentSize;
	}
	return 0;
}

BOOL APIENTRY chrome_accessor_isloading(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->IsLoading();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_dispatch_key_event(IChromeLink *pAccessor, LPCSTR szType,
	int nModifiers,
	LPCSTR szText,
	LPCSTR szUnModifiedText,
	LPCSTR szKeyIdfentifier,
	LPCSTR szCode,
	LPCSTR szKey,
	int nWindowsVirtualKeyCode,
	int nNativeVirtualKeyCode,
	BOOL bAutoRepeat,
	BOOL bIsKeypad, BOOL bIsSysttemKey)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		if (szType != NULL && *szType!='\0')
		{
			return pAccessor->DispatchKeyEvent(A2W(szType), nModifiers, 0,TO_AVGR_STRING(szText), \
				TO_AVGR_STRING(szUnModifiedText), \
				TO_AVGR_STRING(szKeyIdfentifier), \
				TO_AVGR_STRING(szCode), \
				TO_AVGR_STRING(szKey), \
				nWindowsVirtualKeyCode, \
				nNativeVirtualKeyCode, \
				bAutoRepeat, \
				bIsKeypad, \
				bIsSysttemKey);
		}
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_dispatch_mouse_event(IChromeLink *pAccessor,LPCSTR szType,
	int x,
	int y,
	int modifiers,
	LPCSTR szButton,
	int clickCount,
	DWORD deltaX,
	DWORD deltaY)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		if (szType != NULL && *szButton!='\0')
		{
			return pAccessor->DispatchMouseEvent(A2W(szType),x,y ,modifiers,
				TO_AVGR_STRING(szButton), \
				clickCount, \
				deltaX,\
				deltaY);
		}
	}
	return FALSE;
}

LPVOID APIENTRY chrome_new_touch_points(void)
{
	return (LPVOID)new IChromeTouchPoints;
}

void APIENTRY chrome_add_touch_point(IChromeTouchPoints *pTouchPoints, double x, double y, double radiusX, double radiusY, double rotationAngle, double force, double id)
{
	if (pTouchPoints != NULL)
	{
		IChromeTouchPoint data;
		data.x = x;
		data.y = y;
		data.rediusX = radiusX;
		data.rediusY = radiusY;
		data.rotationAngle = rotationAngle;
		data.force = force;
		data.id = id;

		pTouchPoints->push_back(data);
	}
}

void APIENTRY chrome_free_touch_points(IChromeTouchPoints *pTouchPoints)
{
	if (pTouchPoints != NULL)
	{
		delete pTouchPoints;
	}
}

BOOL APIENTRY chrome_accessor_dispatch_touch_event(IChromeLink *pAccessor,const char *szType, IChromeTouchPoints *pTouchPoints, int modifiers)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->DispatchTouchEvent(A2W(szType), pTouchPoints, modifiers);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_emulate_touch_from_mouse_event(IChromeLink *pAccessor, const char* szType, int x, int y, const char* szButton, double deltaX, double deltaY, int modifiers, int clickCount)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->EmulateTouchFromMouseEvent(A2W(szType), x, y, A2W(szButton), deltaX, deltaY, modifiers, clickCount);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_synthesize_pinch_gesture(IChromeLink *pAccessor, double x, double y, double scaleFactor, int relativeSpeed, const char* gestureSourceType)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->SynthesizePinchGesture(x, y, scaleFactor, relativeSpeed, A2W(gestureSourceType));
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_synthesize_scroll_gesture(IChromeLink *pAccessor,double x, double y,
	double xDistance, double yDistance,
	double xOverscroll, double yOverscroll,
	BOOL preventFling, int speed, const char* gestureSourceType, int repeatCount, int repeatDelayMs)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->SynthesizeScrollGesture(x, y, xDistance, yDistance, xOverscroll, yOverscroll, preventFling, speed, A2W(gestureSourceType), repeatCount, repeatDelayMs);
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_synthesize_tap_gesture(IChromeLink *pAccessor,double x, double y, int duration, int tapCount, const char* gestureSourceType)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		return pAccessor->SynthesizeTapGesture(x, y, duration, tapCount, A2W(gestureSourceType));
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_is_javascript_dialog_opening(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		return pAccessor->IsJavaScriptDialogOpening();
	}
	return FALSE;
}

BOOL APIENTRY chrome_accessor_handle_javascript_dialog(IChromeLink *pAccessor, BOOL accept, LPCSTR szPromptText)
{
	if (pAccessor != NULL)
	{
		USES_CONVERSION;
		if(szPromptText!=NULL && *szPromptText!='\0')
			return pAccessor->HandleJavaScriptDialog(accept, A2W(szPromptText));
		else 
			return pAccessor->HandleJavaScriptDialog(accept, NULL);
	}
	return FALSE;
}

LPVOID APIENTRY chrome_accessor_get_javascript_infomation(IChromeLink *pAccessor)
{
	if (pAccessor != NULL)
	{
		IChromeJavaScriptDialogInfomation *pData = new IChromeJavaScriptDialogInfomation;
		pAccessor->GetJavaScriptDialogInfomation((*pData));
		return pData;
	}
	return NULL;
}

LPVOID APIENTRY chrome_accessor_get_console_logs(IChromeLink *pAccessor, int *pLength)
{
	if (pAccessor != NULL)
	{
		std::wstring strBuffer;
		pAccessor->GetConsoleLogs(strBuffer);
		if (strBuffer.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, strBuffer.c_str(), strBuffer.length(), NULL, NULL, NULL, NULL);
			char *szOutBUffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, strBuffer.c_str(), strBuffer.length(), szOutBUffer, nOutLength, NULL, NULL);
			szOutBUffer[nOutLength] = 0;
			*pLength = nOutLength;
			return szOutBUffer;
		}
	}
	return NULL;
}

int APIENTRY chrome_get_javascript_dialog_info_url(IChromeJavaScriptDialogInfomation *pData,char *szBuffer,int len)
{
	if (pData != NULL)
	{
		if (len == 0)
		{
			return WideCharToMultiByte(CP_ACP, NULL, pData->strURL.c_str(), pData->strURL.length(), NULL, NULL, NULL, NULL);
		}
		else
		{
			return WideCharToMultiByte(CP_ACP, NULL, pData->strURL.c_str(), pData->strURL.length(), szBuffer, len, NULL, NULL);
		}
	}
	return 0;
}

int APIENTRY chrome_get_javascript_dialog_info_message(IChromeJavaScriptDialogInfomation *pData, char *szBuffer, int len)
{
	if (pData != NULL)
	{
		if (len == 0)
		{
			return WideCharToMultiByte(CP_ACP, NULL, pData->strMessage.c_str(), pData->strMessage.length(), NULL, NULL, NULL, NULL);
		}
		else
		{
			return WideCharToMultiByte(CP_ACP, NULL, pData->strMessage.c_str(), pData->strMessage.length(), szBuffer, len, NULL, NULL);
		}
	}
	return 0;
}

int APIENTRY chrome_get_javascript_dialog_info_type(IChromeJavaScriptDialogInfomation *pData)
{
	if (pData != NULL)
	{
		return pData->type;
	}
	return NULL;
}

int APIENTRY chrome_get_javascript_dialog_info_default_prompt(IChromeJavaScriptDialogInfomation *pData, char *szBuffer, int len)
{
	if (pData != NULL)
	{
		if (len == 0)
		{
			return WideCharToMultiByte(CP_ACP, NULL, pData->strDefaultPrompt.c_str(), pData->strDefaultPrompt.length(), NULL, NULL, NULL, NULL);
		}
		else
		{
			return WideCharToMultiByte(CP_ACP, NULL, pData->strDefaultPrompt.c_str(), pData->strDefaultPrompt.length(), szBuffer, len, NULL, NULL);
		}
	}
	return 0;
}

void APIENTRY chrome_javascript_dialog_info_free(IChromeJavaScriptDialogInfomation *pData)
{
	if (pData != NULL)
	{
		delete pData;
	}
}

__int64 APIENTRY GetTimestamp(void) {
	std::chrono::time_point<std::chrono::system_clock,
		std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
	auto tmp = std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
	return  (__int64)tmp.count();
}