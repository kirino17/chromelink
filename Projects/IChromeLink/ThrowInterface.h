#pragma once
#include "IChromeLink.h"
#include <atlconv.h>

LPVOID APIENTRY chrome_get_tabs(const char *szTraceAddr)
{
	if (szTraceAddr!=NULL)
	{
		Komari::IChromeCurTabs *pCurTabs = new Komari::IChromeCurTabs;
		USES_CONVERSION;
		if (Komari::IChromeLink::GetTabs(A2W(szTraceAddr), *pCurTabs) == FALSE)
		{
			delete pCurTabs;
			return NULL;
		}
		return (LPVOID)pCurTabs;
	}
	return NULL;
}

int APIENTRY chrome_get_tab_count(const char *szTraceAddr)
{
	if (szTraceAddr != NULL)
	{
		USES_CONVERSION;
		return Komari::IChromeLink::GetTabCount(A2W(szTraceAddr));
	}
	return 0;
}


LPVOID APIENTRY chrome_struct_tabs_desc(Komari::IChromeCurTabs *pTabs,int index,int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strDescription.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strDescription.c_str(), Info.strDescription.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strDescription.c_str(), Info.strDescription.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tabs_devtools(Komari::IChromeCurTabs *pTabs, int index, int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strDevToolsFrontendUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strDevToolsFrontendUrl.c_str(), Info.strDevToolsFrontendUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strDevToolsFrontendUrl.c_str(), Info.strDevToolsFrontendUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tabs_id(Komari::IChromeCurTabs *pTabs, int index, int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strId.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strId.c_str(), Info.strId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strId.c_str(), Info.strId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tabs_title(Komari::IChromeCurTabs *pTabs, int index, int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strTitle.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strTitle.c_str(), Info.strTitle.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strTitle.c_str(), Info.strTitle.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tabs_type(Komari::IChromeCurTabs *pTabs, int index, int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strType.c_str(), Info.strType.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strType.c_str(), Info.strType.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tabs_url(Komari::IChromeCurTabs *pTabs, int index, int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strUrl.c_str(), Info.strUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strUrl.c_str(), Info.strUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tabs_ws(Komari::IChromeCurTabs *pTabs, int index, int *pLength)
{
	if (pTabs != NULL && pLength != NULL)
	{
		const Komari::IChromeTabInfo &Info = pTabs->at(index);
		if (Info.strWebSocketDebuggerUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, Info.strWebSocketDebuggerUrl.c_str(), Info.strWebSocketDebuggerUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, Info.strWebSocketDebuggerUrl.c_str(), Info.strWebSocketDebuggerUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

int APIENTRY chrome_struct_tabs_size(Komari::IChromeCurTabs *pTabs)
{
	if (pTabs != NULL)
		return pTabs->size();
	return 0;
}

void APIENTRY chrome_buffer_free(char *buffer)
{
	if (buffer != NULL)
	{
		free(buffer);
	}
}

void APIENTRY chrome_struct_tabs_free(Komari::IChromeCurTabs *pTabs)
{
	if (pTabs != NULL)
	{
		pTabs->clear();
		delete pTabs;
	}
}

LPVOID APIENTRY chrome_new_tab(const char *szTraceAddr, const char *szUrl)
{
	if (szTraceAddr!=NULL)
	{
		Komari::IChromeTabInfo *pNewInfo = new Komari::IChromeTabInfo;
		USES_CONVERSION;
		BOOL bResult = FALSE;
		if (szUrl!=NULL)
		{
			bResult = Komari::IChromeLink::NewTab(A2W(szTraceAddr), A2W(szUrl), *pNewInfo);
		}
		else
		{
			bResult = Komari::IChromeLink::NewTab(A2W(szTraceAddr), TEXT(""), *pNewInfo);
		}
		if (bResult == FALSE)
		{
			delete pNewInfo;
			return NULL;
		}
		return pNewInfo;
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_desc(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strDescription.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strDescription.c_str(), pTab->strDescription.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strDescription.c_str(), pTab->strDescription.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_devtools(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strDevToolsFrontendUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strDevToolsFrontendUrl.c_str(), pTab->strDevToolsFrontendUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strDevToolsFrontendUrl.c_str(), pTab->strDevToolsFrontendUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_id(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strId.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strId.c_str(), pTab->strId.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strId.c_str(), pTab->strId.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_title(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strTitle.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strTitle.c_str(), pTab->strTitle.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strTitle.c_str(), pTab->strTitle.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_type(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strType.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strType.c_str(), pTab->strType.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strType.c_str(), pTab->strType.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_url(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strUrl.c_str(), pTab->strUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strUrl.c_str(), pTab->strUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

LPVOID APIENTRY chrome_struct_tab_ws(Komari::IChromeTabInfo *pTab, int *pLength)
{
	if (pTab != NULL && pLength != NULL)
	{
		if (pTab->strWebSocketDebuggerUrl.length() > 0)
		{
			int nOutLength = WideCharToMultiByte(CP_ACP, NULL, pTab->strWebSocketDebuggerUrl.c_str(), pTab->strWebSocketDebuggerUrl.length(), NULL, NULL, NULL, NULL);
			char *szOutBuffer = (char*)malloc(nOutLength + 1);
			WideCharToMultiByte(CP_ACP, NULL, pTab->strWebSocketDebuggerUrl.c_str(), pTab->strWebSocketDebuggerUrl.length(), szOutBuffer, nOutLength, NULL, NULL);
			szOutBuffer[nOutLength] = '\0';
			*pLength = nOutLength;
			return szOutBuffer;
		}
	}
	return NULL;
}

void APIENTRY chrome_struct_tab_free(Komari::IChromeTabInfo *pTab)
{
	if (pTab != NULL)
	{
		delete pTab;
	}
}

BOOL APIENTRY chrome_close_tab(const char *szTraceAddr, const char *szId)
{
	if (szTraceAddr!=NULL && szId!=NULL)
	{
		USES_CONVERSION;
		return Komari::IChromeLink::CloseTab(A2W(szTraceAddr), A2W(szId));
	}
	return FALSE;
}

BOOL APIENTRY chrome_activate_tab(const char *szTraceAddr, const char *szId)
{
	if (szTraceAddr!=NULL && szId!=NULL)
	{
		USES_CONVERSION;
		return Komari::IChromeLink::ActivateTab(A2W(szTraceAddr), A2W(szId));
	}
	return FALSE;
}