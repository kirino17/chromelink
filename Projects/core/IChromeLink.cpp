
#include "IChromeLink.h"
#include <sstream>

using namespace Komari;

/*
函数: GetTabs
说明: 获取浏览器中所有已打开的标签页信息
szTraceAddr : 浏览器调试地址
Tabs : 返回获得的标签页列表
返回: 如果返回真则说明调用是成功的。
*/
BOOL IChromeLink::GetTabs(LPCTSTR szTraceAddr, IChromeCurTabs &Tabs)
{
	Tabs.clear();
	BOOL bResult = FALSE;
	IStream strBuilderUrl;

	strBuilderUrl << TEXT("http://") << szTraceAddr << TEXT("/json/list");
	IString strPullUrl = strBuilderUrl.str();
	http_client client(strPullUrl.c_str());
	try
	{
		client.request(methods::GET, TEXT("")).then([&](http_response response) {

			//参考访问:  http://localhsot:9222/json/list
			IChromeTabInfo TabInfo;
			if (response.status_code() == status_codes::OK)
			{
				const json::value &root = response.extract_json().get();
				int nMax = root.size();
				for (size_t i = 0; i < nMax; i++)
				{
					auto child = root.at(i);
					TabInfo.strDescription = child.at(U("description")).as_string();
					TabInfo.strId = child.at(TEXT("id")).as_string();
					TabInfo.strTitle = child.at(TEXT("title")).as_string();
					TabInfo.strType = child.at(TEXT("type")).as_string();
					TabInfo.strUrl = child.at(TEXT("url")).as_string();
					try
					{
						TabInfo.strWebSocketDebuggerUrl = child.at(TEXT("webSocketDebuggerUrl")).as_string();
					}
					catch (const std::exception&)
					{
						TabInfo.strWebSocketDebuggerUrl = TEXT("ws://");
						TabInfo.strWebSocketDebuggerUrl += szTraceAddr;
						TabInfo.strWebSocketDebuggerUrl += TEXT("/devtools/page/");
						TabInfo.strWebSocketDebuggerUrl += TabInfo.strId;
					}
					Tabs.push_back(TabInfo);
				}
				bResult = TRUE;
			}

		}).wait();
	}
	catch (const std::exception&)
	{

	}
	return bResult;
}

/*
函数: GetTabCount
说明: 获取标签页数量。
返回: 如果成功将返回数量。
*/
int IChromeLink::GetTabCount(LPCTSTR szTraceAddr)
{

	int nResult = 0;
	IStream strBuilderUrl;

	strBuilderUrl << TEXT("http://") << szTraceAddr << TEXT("/json/list");
	IString strPullUrl = strBuilderUrl.str();
	http_client client(strPullUrl.c_str());
	try
	{
		client.request(methods::GET, TEXT("")).then([&](http_response response) {

			//参考访问:  http://localhsot:9222/json/list
			IChromeTabInfo TabInfo;
			if (response.status_code() == status_codes::OK)
			{
				const json::value &root = response.extract_json().get();
				nResult = root.size();
			}

		}).wait();
	}
	catch (const std::exception&)
	{

	}
	return nResult;
}

/*
函数: NewTab
说明: 打开一个新的标签页
szTraceAddr : 浏览器调试地址
szUrl : 设置页访问的地址
Tab : 返回已打开的标签页信息
返回: 如果返回真则说明调用是成功的。
*/
BOOL IChromeLink::NewTab(LPCTSTR szTraceAddr, LPCTSTR szUrl, IChromeTabInfo &Tab)
{
	IStream strBuilderUrl;
	BOOL bResult = FALSE;

	if (szUrl!=NULL)
	{
		strBuilderUrl << TEXT("http://") << szTraceAddr << TEXT("/json/new?") << szUrl;
	}
	else
	{
		strBuilderUrl << TEXT("http://") << szTraceAddr << TEXT("/json/new");
	}
	IString strPullUrl = strBuilderUrl.str();
	http_client client(strPullUrl);
	try
	{
		client.request(methods::GET, TEXT("")).then([&](http_response response) {

			//访问参考: http://localhost:9222/json/new?url
			IChromeTabInfo TabInfo;
			if (response.status_code() == status_codes::OK)
			{
				const web::json::value &root = response.extract_json().get();
				Tab.strDescription = root.at(TEXT("description")).as_string();
				Tab.strDevToolsFrontendUrl = root.at(TEXT("devtoolsFrontendUrl")).as_string();
				Tab.strId = root.at(TEXT("id")).as_string();
				Tab.strTitle = root.at(TEXT("title")).as_string();
				Tab.strType = root.at(TEXT("type")).as_string();
				Tab.strUrl = root.at(TEXT("url")).as_string();
				Tab.strWebSocketDebuggerUrl = root.at(TEXT("webSocketDebuggerUrl")).as_string();
				bResult = TRUE;
			}
		}).wait();
	}
	catch (const std::exception&)
	{

	}
	return bResult;
}

/*
函数: CloseTab
说明: 关闭一个标签页。
szTraceAddr : 浏览器调试地址
szId : 标签的ID
返回: 如果返回真则说明调用是成功的。
*/
BOOL IChromeLink::CloseTab(LPCTSTR szTraceAddr, LPCTSTR szId)
{
	IStream strBuilderUrl;
	BOOL bResult = FALSE;

	strBuilderUrl << TEXT("http://") << szTraceAddr << TEXT("/json/close/") << szId;
	IString strPullUrl = strBuilderUrl.str();

	http_client client(strPullUrl);

	try
	{
		client.request(methods::GET, TEXT("")).then([&](http_response response) {

			//访问参考: http://localhost:9222/json/close/id
			if (response.status_code() == status_codes::OK)
			{
				auto bodyStream = response.body();
				streams::stringstreambuf sBuffer;
				auto &strText = sBuffer.collection();
				bodyStream.read_to_end(sBuffer).get();
				if (_strcmpi(strText.c_str(), "Target is closing") == 0)
				{
					bResult = TRUE;
				}
				else
				{
					bResult = FALSE;
				}
			}
		}).wait();
	}
	catch (const std::exception&)
	{

	}
	return bResult;
}

/*
函数: ActivateTab
说明: 激活一个标签页。
szTraceAddr : 浏览器调试地址
szId : 标签的ID
返回: 如果返回真则说明调用是成功的。
*/
BOOL IChromeLink::ActivateTab(LPCTSTR szTraceAddr, LPCTSTR szId)
{
	IStream strBuilderUrl;
	BOOL bResult = FALSE;

	strBuilderUrl << TEXT("http://") << szTraceAddr << TEXT("/json/activate/") << szId;
	IString strPullUrl = strBuilderUrl.str();

	http_client client(strPullUrl);

	try
	{
		client.request(methods::GET, TEXT("")).then([&](http_response response) {

			//访问参考: http://localhost:9222/json/activate/id
			if (response.status_code() == status_codes::OK)
			{
				auto bodyStream = response.body();
				streams::stringstreambuf sBuffer;
				auto &strText = sBuffer.collection();
				bodyStream.read_to_end(sBuffer).get();
				if (_strcmpi(strText.c_str(), "Target activated") == 0)
				{
					bResult = TRUE;
				}
				else
				{
					bResult = FALSE;
				}
			}

		}).wait();
	}
	catch (const std::exception&)
	{

	}
	return bResult;
}

/////////////////////////////////////////////////////////////////////////////////////////////////
IChromeLink::IChromeLink(LPCTSTR szWebSocketDebuggerUrl)
{
	this->m_strTraceAddr = szWebSocketDebuggerUrl;
	this->m_bLinkStatus = FALSE;
	this->m_dwAutoIdentify = 0;
	this->m_bReadyState = FALSE;
	this->m_bInitializedSucess = FALSE;
	this->m_javascriptDialogInfomation.bDialogLifed = FALSE;
	this->m_javascriptDialogInfomation.type = IC_JSDT_ALERT;
	this->AddRef();
}

IChromeLink::~IChromeLink()
{
	this->Close();
	printf("对象销毁 \n");

}

/*
函数: LinkStart
说明: 连接至指定的标签页。
返回: 如果连接成功将返回真。
*/
BOOL IChromeLink::LinkStart(void)
{
	this->m_bLinkStatus = FALSE;
	if (this->m_strTraceAddr.length() <= 0)
	{
		return FALSE;
	}
	try
	{
		this->m_wsKeepLink.connect(this->m_strTraceAddr).get();
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	this->m_bLinkStatus = TRUE;
	std::thread t(&IChromeLink::OnEventDispatchs,this,this);
	t.detach();
	this->m_bInitializedSucess = FALSE;
	return this->OnLinkSucess();
}

void IChromeLink::Close(void)
{
	if (this->m_bLinkStatus == TRUE)
	{
		this->m_wsKeepLink.close().get();
		if (this->m_mapRequestQueues.size() > 0)
		{
			IChromeResponse::iterator iter = this->m_mapRequestQueues.begin();
			IChromeResponse::iterator iend = this->m_mapRequestQueues.end();
			for (;iter!=iend;iter++)
			{
				iter->second.set_exception(websocket_exception("miss connected!"));
			}
		}
		this->m_taskReady.set_exception("miss connected!");
		this->m_bLinkStatus = FALSE;
	}
}

//内部消息调度线程
void CALLBACK IChromeLink::OnEventDispatchs(IChromeLink *pChromeLink)
{
	if (pChromeLink != NULL)
	{
		pChromeLink->AddRef();
		while (true)
		{
			try
			{
				websocket_incoming_message objMsg = pChromeLink->m_wsKeepLink.receive().get();
				if (objMsg.message_type() == websocket_message_type::text_message)
				{
					streams::stringstreambuf sBuffer;
					auto &strBody = sBuffer.collection();
					objMsg.body().read_to_end(sBuffer).get();
					json::value objRoot = json::value::parse(conversions::to_string_t(strBody));
					if (pChromeLink->m_bLinkStatus == FALSE)
					{
						break;
					}
					if (!pChromeLink->m_mapRequestQueues.empty())
					{
						//关联Request
						json::value objAutoIdentify = objRoot[U("id")];
						if (!objAutoIdentify.is_null())
						{
							//寻找Request
							DWORD dwAutoId = objAutoIdentify.as_integer();
							IChromeResponse::iterator itRequest = pChromeLink->m_mapRequestQueues.find(dwAutoId);
							if (itRequest != pChromeLink->m_mapRequestQueues.end())
							{
								//找到Request
								itRequest->second.set(objRoot);	//设置Request;
								pChromeLink->m_hUnionMutex.lock();
								pChromeLink->m_mapRequestQueues.erase(itRequest);
								pChromeLink->m_hUnionMutex.unlock();
								continue;
							}
						}
					}
					//如果没有id成员,则为事件信息
					this->SendEventDispatchs(objRoot);
				}
			}
			catch (const std::exception &ex)
			{
				break;
			}
		}
		pChromeLink->Close();
		pChromeLink->Release();
	}
}

//调度事件消息
void IChromeLink::SendEventDispatchs(const json::value &objRoot)
{
	IString strMethod = objRoot.at(U("method")).as_string();
	TCHAR szVar[100];
	if (lstrcmpi(strMethod.c_str(), TEXT("Runtime.executionContextCreated")) == 0)
	{
		//V8引擎创建完毕事件
		IChromeExecutionContextId Info;

		json::value objParams = objRoot.at(U("params"));
		json::value objContext = objParams.at(U("context"));
		json::value objAuxData = objContext.at(U("auxData"));

		Info.dwExecutionContextId = objContext.at(U("id")).as_integer();
		Info.strFrameId = objAuxData.at(U("frameId")).as_string();
		this->m_vecExecutionContextId.push_back(Info);
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Runtime.executionContextDestroyed")) == 0)
	{
		//V8引擎销毁事件
		json::value objParams = objRoot.at(U("params"));
		DWORD dwExecutionContextId = objParams.at(U("executionContextId")).as_integer();
		IChromeV8ExecutionContextIdVectors::iterator iter = this->m_vecExecutionContextId.begin();
		IChromeV8ExecutionContextIdVectors::iterator iend = this->m_vecExecutionContextId.end();
		for (;iter!=iend;iter++)
		{
			if (iter->dwExecutionContextId == dwExecutionContextId)
			{
				this->m_vecExecutionContextId.erase(iter);
				break;
			}
		}
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Runtime.executionContextsCleared")) == 0)
	{
		this->m_vecExecutionContextId.clear();
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Page.loadEventFired")) == 0)
	{
		this->m_vecChildFrameIdLists.clear();
		this->GetWebSheet().then([this](json::value root) {
			this->OnParserWebTree(root);
			this->m_bReadyState = FALSE;
		});
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Page.frameStartedLoading")) == 0)
	{
		this->m_bReadyState = TRUE;
		this->m_vecChildFrameIdLists.clear();
		m_consoleInfos.clear();
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Page. frameStoppedLoading")) == 0)
	{

	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Page.javascriptDialogOpening")) == 0)
	{
		json::value objParams = objRoot.at(U("params"));
		this->m_javascriptDialogInfomation.bDialogLifed = TRUE;
		try
		{
			this->m_javascriptDialogInfomation.strURL = objParams[U("url")].as_string().c_str();
		}
		catch (const std::exception&)
		{

		}
		
		try
		{
			this->m_javascriptDialogInfomation.strMessage = objParams[U("message")].as_string().c_str();
		}
		catch (const std::exception&)
		{

		}
		IString strType= objParams[U("type")].as_string();
		if (strType == TEXT("alert"))
		{
			this->m_javascriptDialogInfomation.type = IC_JSDT_ALERT;
		}
		else if (strType == TEXT("confirm"))
		{
			this->m_javascriptDialogInfomation.type = IC_JSDT_CONFIRM;
		}
		else if (strType == TEXT("prompt"))
		{
			this->m_javascriptDialogInfomation.type = IC_JSDT_PROMPT;
		}
		else if (strType == TEXT("beforeunload"))
		{
			this->m_javascriptDialogInfomation.type = IC_JSDT_BEFOREUNLOAD;
		}
		else this->m_javascriptDialogInfomation.type = IC_JSDT_ALERT;
		try
		{
			this->m_javascriptDialogInfomation.strDefaultPrompt = objParams[U("defaultPrompt")].as_string();
		}
		catch (const std::exception&)
		{

		}
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Page.javascriptDialogClosed")) == 0)
	{
		this->m_javascriptDialogInfomation.bDialogLifed = FALSE;
		this->m_javascriptDialogInfomation.strMessage = TEXT("");
		this->m_javascriptDialogInfomation.strURL = TEXT("");
		this->m_javascriptDialogInfomation.strDefaultPrompt = TEXT("");
	}
	else if (lstrcmpi(strMethod.c_str(), TEXT("Runtime.consoleAPICalled")) == 0)
	{
		json::value objParams = objRoot.at(U("params"));
		json::value objArgs = objParams[U("args")];
		int nMax = objArgs.size();
		std::wstringstream ssBuffer;
		std::wstring strType;
		for (size_t i = 0; i < nMax; i++)
		{
			ssBuffer.clear();
			strType = objArgs[i][U("type")].as_string();
			if (strType == TEXT("string"))
			{
				ssBuffer<<TEXT("[string]: ")<< objArgs[i][U("value")].as_string();
				std::wstring &strBuffer = ssBuffer.str();
				m_consoleInfos.push_back(strBuffer);
			}
			else if (strType == TEXT("number"))
			{
				json::value number = objArgs[i][U("value")];
				if(number.is_integer())
					ssBuffer << TEXT("[number]: ") << number.as_integer();
				else if (number.is_double())
					ssBuffer << TEXT("[number]: ") << number.as_double();
				std::wstring &strBuffer = ssBuffer.str();
				m_consoleInfos.push_back(strBuffer);
			}
			else if (strType == TEXT("boolean"))
			{
				ssBuffer << TEXT("[boolean]: ") << objArgs[i][U("value")].as_bool();
				std::wstring &strBuffer = ssBuffer.str();
				m_consoleInfos.push_back(strBuffer);
			}
		}
	}
}

//获取一个增量ID
DWORD IChromeLink::GetAutoRand(void)
{
	InterlockedCompareExchange(&this->m_dwAutoIdentify, 0, 0x7FFFFFFF);
	DWORD dwNewIdentify = InterlockedIncrement(&this->m_dwAutoIdentify);
	return dwNewIdentify;
}

/*
函数: OnLinkSucess
说明: 在成功连接到浏览器后进行初始化设置。
*/
BOOL IChromeLink::OnLinkSucess(void)
{
	try
	{
		json::value objRoot1 = this->OpenPageEvents().get();
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	this->m_vecExecutionContextId.clear();
	this->OpenRuntimeEvents().then([](json::value objRoot2) {
		///

	}).then([](pplx::task<void> t) {
		try
		{
			t.get();
		}
		catch (const std::exception&)
		{

		}
	});

	try
	{
		json::value objRoot = this->GetWebSheet().get();
		this->OnParserWebTree(objRoot);
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}

	BOOL bResult = FALSE;
	bResult = this->OpenNetwork();
	return bResult;
}

pplx::task<json::value> IChromeLink::GetWebSheet(void)
{
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.getResourceTree"));
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		objResponse.set_exception(ex);
		return pplx::create_task(objResponse);
	}
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	return pplx::create_task(objResponse);
}

/*
函数: OpenPageEvents
说明: 开启接收"Page"域的事件信息
返回: 成功返回Response。
*/
pplx::task<json::value> IChromeLink::OpenPageEvents(void)
{
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.enable"));
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		objResponse.set_exception(ex);
		return pplx::create_task(objResponse);
	}
	return pplx::create_task(objResponse);
}

/*
函数: OpenRuntimeEvents
说明: 开启接收"Runtime"域的事件信息
返回: 成功返回Response
*/
pplx::task<json::value> IChromeLink::OpenRuntimeEvents(void)
{
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Runtime.enable"));
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		objResponse.set_exception(ex);
		return pplx::create_task(objResponse);
	}
	return pplx::create_task(objResponse);
}

BOOL IChromeLink::OpenNetwork(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.enable"));
	json::value objParams;
	objParams[U("maxTotalBufferSize")] = json::value::number(0x8000);
	objParams[U("maxResourceBufferSize")] = json::value::number(524288);
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

//解析网页结构
void IChromeLink::OnParserWebTree(const json::value &objRoot)
{
	char szVar[100];
	this->m_vecChildFrameIdLists.clear();
	this->m_objMainFrameId.strFrameId = TEXT("");
	this->m_objMainFrameId.strLoaderId = TEXT("");
	this->m_objMainFrameId.strMimeType = TEXT("");
	this->m_objMainFrameId.strName = TEXT("");
	this->m_objMainFrameId.strParentId = TEXT("");
	this->m_objMainFrameId.strUrl = TEXT("");
	const json::value &objResult = objRoot.at(U("result"));
	const json::value &objFrameTree = objResult.at(U("frameTree"));
	EnumWebFrame(objFrameTree);
	/*json::value objChildFrames;
	int nMax = 0;
	MessageBox(NULL, objFrameTree.serialize().c_str(), NULL, NULL);
	try
	{
		objChildFrames = objFrameTree.at(U("childFrames"));
		nMax = objChildFrames.size();
		sprintf_s(szVar, 100, "%d", nMax);
		MessageBoxA(NULL, szVar, NULL, NULL);
	}
	catch (const std::exception&)
	{
		nMax = 0;
	}
	IChromeFrameId Info;
	for (size_t i = 0; i < nMax; i++)
	{
		json::value objFrame = objChildFrames[i].at(U("frame"));
		Info.strFrameId = objFrame.at(U("id")).as_string();
		Info.strLoaderId = objFrame.at(U("loaderId")).as_string();
		Info.strMimeType = objFrame.at(U("mimeType")).as_string();
		Info.strName = objFrame.at(U("name")).as_string();
		try
		{
			json::value objParentId = objFrame.at(U("parentId"));
			Info.strParentId = objParentId.as_string();
		}
		catch (const std::exception&)
		{
			Info.strParentId = TEXT("");
		}
		Info.strUrl = objFrame.at(U("url")).as_string();
		this->m_vecChildFrameIdLists.push_back(Info);
	}*/


	json::value objMainFrame= objFrameTree.at(U("frame"));
	this->m_objMainFrameId.strFrameId = objMainFrame.at(U("id")).as_string();
	this->m_objMainFrameId.strLoaderId = objMainFrame.at(U("loaderId")).as_string();
	this->m_objMainFrameId.strMimeType = objMainFrame.at(U("mimeType")).as_string();
	this->m_objMainFrameId.strName = TEXT("");
	this->m_objMainFrameId.strParentId = TEXT("");
	this->m_objMainFrameId.strUrl = objMainFrame.at(U("url")).as_string();
}

//解析网页框架
void IChromeLink::EnumWebFrame(const json::value &objRoot)
{
	try
	{
		const json::value &childFrames = objRoot.at(L"childFrames");
		int frameCount = childFrames.size();
		for (size_t i = 0; i < frameCount; i++)
		{
			const auto &element = childFrames.at(i);
			EnumWebFrame(element);
			GetWebFrame(element.at(L"frame"));
		}
	}
	catch (const std::exception&)
	{

	}
}

//获取框架
void IChromeLink::GetWebFrame(const json::value &objRoot)
{
	IChromeFrameId Info;
	Info.strFrameId = objRoot.at(U("id")).as_string();
	Info.strLoaderId = objRoot.at(U("loaderId")).as_string();
	Info.strMimeType = objRoot.at(U("mimeType")).as_string();
	Info.strName = objRoot.at(U("name")).as_string();
	try
	{
		json::value objParentId = objRoot.at(U("parentId"));
		Info.strParentId = objParentId.as_string();
	}
	catch (const std::exception&)
	{
		Info.strParentId = TEXT("");
	}
	Info.strUrl = objRoot.at(U("url")).as_string();
	this->m_vecChildFrameIdLists.push_back(Info);
}

pplx::task<BOOL> IChromeLink::GetExecutionContextId(void)
{
	return pplx::create_task(this->m_taskReady);
}

/*
函数: GetMainFrame
说明: 获取标签页中的主框架对象
返回: 成功将返回主框架信息。
*/
const IChromeFrameId &IChromeLink::GetMainFrame(void)
{
	return this->m_objMainFrameId;
}

/*
函数: GetFrameCount
说明: 获取标签页中的子框架数
返回: 成功返回子框架数量。
*/
int IChromeLink::GetFrameCount(void)
{
	return this->m_vecChildFrameIdLists.size();
}

/*
函数: GetChildFrame
说明: 获取子框架对象
index: 下标,从0开始
返回: 成功将返回子框架信息。
*/
const IChromeFrameId &IChromeLink::GetChildFrame(int index)
{
	return this->m_vecChildFrameIdLists[index];
}

/*
函数: GetExecutionContextCount
说明: 获取V8上下文执行环境数量
返回: 成功返回环境数量
*/
int IChromeLink::GetExecutionContextCount(void)
{
	return this->m_vecExecutionContextId.size();
}

/*
函数: GetExecutionContextId
说明: 获取V8上下文执行环境
返回: 成功返回上下文执行环境对象
*/
const IChromeExecutionContextId &IChromeLink::GetExecutionContextId(int index)
{
	return this->m_vecExecutionContextId[index];
}

/*
函数: Evaluate
说明: 执行表达式
szExpression: 表达式文本
szObjectGroup: 可用于释放多个对象的符号组名称。
bIncludeCommandLineAPI: 确定在评估期间Command Line API是否可用。
bSilent: 在静默模式下，评估期间抛出的异常不报告，不要暂停执行。 覆盖setPauseOnException状态。
dwExecutionContextId: 指定执行上下文执行评估。 如果省略参数，将在检查页面的上下文中执行评估。
bReturnByValue: 结果是否应该是应该通过值发送的JSON对象。
bGeneratePreview: 是否应为结果生成预览。
bUserGesture: 执行是否应该被视为用户在UI中发起的。
bAwaitPromise: 执行是否应该等待承诺解决。 如果评估结果不是承诺，则认为是错误。
objResult: 用于返回表达式结果
返回: 成功返回编译信息。
*/
BOOL IChromeLink::Evaluate(LPCTSTR szExpression, LPCTSTR szObjectGroup, BOOL bIncludeCommandLineAPI, BOOL bSilent, DWORD dwExecutionContextId, BOOL bReturnByValue, BOOL bGeneratePreview, BOOL bUserGesture, BOOL bAwaitPromise, IChromeEvalResult &objResult)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Runtime.evaluate"));
	json::value objParams;
	objParams[U("expression")] = json::value::string(szExpression);
	objParams[U("objectGroup")] = json::value::string(szObjectGroup);
	objParams[U("includeCommandLineAPI")] = json::value::boolean(bIncludeCommandLineAPI);
	objParams[U("silent")] = json::value::boolean(bSilent);
	objParams[U("contextId")] = json::value::number((int)dwExecutionContextId);
	objParams[U("returnByValue")] = json::value::boolean(bReturnByValue);
	objParams[U("generatePreview")] = json::value::boolean(bGeneratePreview);
	objParams[U("userGesture")] = json::value::boolean(bUserGesture);
	objParams[U("awaitPromise")] = json::value::boolean(bAwaitPromise);

	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}


	try
	{
		json::value objRetResult = pplx::create_task(objResponse).get();
		json::value objReturn = objRetResult.at(U("result"));
		try
		{
			json::value obExceptionDetails = objReturn.at(U("exceptionDetails"));
			objResult.bException = TRUE;
		}
		catch (const std::exception&)
		{
			objResult.bException = FALSE;
		}

		json::value objChild = objReturn.at(U("result"));
		try
		{
			objResult.strClassName = objChild.at(U("className")).as_string();
		}
		catch (const std::exception&)
		{
			
		}
		try
		{
			objResult.strSubType = objChild.at(U("subtype")).as_string();
		}
		catch (const std::exception&)
		{

		}
		try
		{
			objResult.strType = objChild.at(U("type")).as_string();
			if (objResult.strType == TEXT("string"))
			{
				objResult.strValue = objChild.at(U("value")).as_string();
			}
			else if (objResult.strType == TEXT("number"))
			{
				json::value val = objChild.at(U("value"));
				if (val.is_double())
				{
					objResult.flValue = val.as_double();
					objResult.strType = U("double");

				}
				else if (val.is_integer())
				{
					objResult.nValue = val.as_integer();
				}
			}
			else if (objResult.strType == TEXT("boolean"))
			{
				json::value val = objChild.at(U("value"));
				objResult.bValue = val.as_bool();
			}
		}
		catch (const std::exception&)
		{

		}
		try
		{
			objResult.strDescription = objChild.at(U("description")).as_string();
		}
		catch (const std::exception&)
		{

		}
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ReleaseObjectGroup
说明: 释放属于给定组的所有远程对象。
szObjectGroup: 符合组名
返回: 成功返回真,失败返回假
*/
BOOL IChromeLink::ReleaseObjectGroup(LPCTSTR szObjectGroup)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Runtime.releaseObjectGroup"));
	json::value objParams;
	objParams[U("objectGroup")] = json::value::string(szObjectGroup);
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数:  SetReady
说明: 设置就绪状态
enStatus: 设置就绪状态类型
*/
void IChromeLink::SetReady(IChromeReadyStatus enStatus)
{
	if (enStatus == ICR_SIGNAL)
	{
		SetEvent(this->m_hEventReady);
	}
	else
	{
		ResetEvent(this->m_hEventReady);
	}
}

/*
函数: WaitReady
nWaitTime: 等待的时间,默认为: INFINITE,但不推荐使用默认
说明: 等待至就绪状态
返回: 返回等待的结果
*/
IChromeReadyStatus IChromeLink::WaitReady(int nWaitTime)
{
	if (WaitForSingleObject(this->m_hEventReady, nWaitTime) == WAIT_OBJECT_0)
	{
		return ICR_SIGNAL;
	}
	return ICR_WAITING;
}

/*
函数: Navigate
说明: 跳转到指定页面
szUrl: 欲访问的地址
szReferrer: 来路设置
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::Navigate(LPCTSTR szUrl, LPCTSTR szReferrer)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.navigate"));
	json::value objParams;
	objParams[U("url")] = json::value::string(szUrl);
	if (szReferrer != NULL && lstrlen(szReferrer)>0)
	{
		objParams[U("referer")] = json::value::string(szReferrer);
	}
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ReLoad
说明: 重新加载页面
bIgnoreCache: 是否忽略缓存进行重新加载
szScriptToEvaluateOnLoad: 如果设置，脚本将被重新加载后注入被检查页面的所有帧。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ReLoad(BOOL bIgnoreCache, LPCTSTR szScriptToEvaluateOnLoad)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.reload"));
	json::value objParams;
	objParams[U("ignoreCache")] = json::value::boolean(bIgnoreCache);
	if (szScriptToEvaluateOnLoad!=NULL)
	{
		objParams[U("scriptToEvaluateOnLoad")] = json::value::string(szScriptToEvaluateOnLoad);
	}
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: GetResourceContent
说明: 返回给定资源的内容
strRetContent: 返回获得的资源内容
bBase64Encoded: 返回的内容是否已使用base64编码
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::GetResourceContent(LPCTSTR szFrameId, LPCTSTR szUrl, IString &strRetContent, BOOL &bBase64Encoded)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.getResourceContent"));
	json::value objParams;
	objParams[U("frameId")] = json::value::string(szFrameId);
	if (szUrl != NULL)
	{
		objParams[U("url")] = json::value::string(szUrl);
	}
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());

	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		json::value objResult = objRoot[U("result")];
		bBase64Encoded = objResult[U("base64Encoded")].as_bool();
		strRetContent= objResult[U("content")].as_string();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

BOOL IChromeLink::UpdateFrame(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	try
	{
		json::value objRoot = this->GetWebSheet().get();
		//MessageBox(NULL, objRoot.serialize().c_str(), NULL, NULL);
		this->OnParserWebTree(objRoot);
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetDocumentContent
说明: 设置指定框架中的HTML代码
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetDocumentContent(LPCTSTR szFrameId, LPCTSTR szHtml)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	if (szFrameId == NULL || szHtml == NULL)
	{
		return FALSE;
	}
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.setDocumentContent"));
	json::value objParams;
	objParams[U("frameId")] = json::value::string(szFrameId);
	objParams[U("html")] = json::value::string(szHtml);
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: CaptureScreenshot
说明: 拍摄页面快照
szFormat: 快照的存储格式,可选值: jpeg,png
nQuality: 压缩质量范围(0-100),仅限jpeg
bFromSurface: 从表面捕获屏幕截图，而不是视图。 默认为true.
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::CaptureScreenshot(LPCTSTR szFormat, int nQuality, BOOL bFromSurface, IString &strData)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	if (szFormat == NULL)
	{
		return FALSE;
	}
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.captureScreenshot"));
	json::value objParams;
	objParams[U("format")] = json::value::string(szFormat);
	objParams[U("quality")] = json::value::number(nQuality);
	objParams[U("fromSurface")] = json::value::boolean(bFromSurface);
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		json::value objResult = objRoot[U("result")];
		json::value objData= objResult[U("data")];
		strData = objData.as_string();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetDeviceMetricsOverride
说明: 覆盖设备屏幕尺寸的值（window.screen.width，window.screen.height，window.innerWidth，window.innerHeight和“device-width”/“device-height”）相关的CSS媒体查询结果）。
nWidth: 覆盖宽度值（以像素为单位）（最小值0，最大10000000）。 0禁用覆盖。
nHeight: 覆盖高度值（以像素为单位）（最小值0，最大值10000000）。 0禁用覆盖。
flDeviceScaleFactor: 覆盖设备比例因子值。 0禁用覆盖。
bMobile: 是否模拟移动设备。 这包括视口元标记，覆盖滚动条，文本自动调整等等。
bFitWindow: 超过可用浏览器窗口区域的视图是否应缩小以适应。
flScale: 缩放以应用于结果视图图像。 忽略| fitWindow | 模式。
nScreenWidth: 覆盖屏幕宽度值（以像素为单位）（最小值0，最大10000000）。 只用于| mobile == true |。
nScreenHeight: 覆盖屏幕高度值（以像素为单位）（最小0，最大10000000）。 只用于| mobile == true |。
nPositionX: 在屏幕上覆盖视图X位置（以像素为单位）（最小值0，最大值10000000）。 只用于| mobile == true |。
nPositionY: 在屏幕上覆盖视图Y位置（以像素为单位）（最小值0，最大值10000000）。 只用于| mobile == true |。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetDeviceMetricsOverride(int nWidth,
	int nHeight,
	double flDeviceScaleFactor,
	BOOL bMobile,
	double flScale,
	int nScreenWidth,
	int nScreenHeight,
	int nPositionX,
	int nPositionY,
	BOOL dontSetVisibleSize,
	LPCTSTR szScreenOrientationType,
	int nScreenOrientationAngle)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setDeviceMetricsOverride"));
	json::value objParams;
	json::value objScreenOrientation;
	objParams[U("width")] = json::value::number(nWidth);
	objParams[U("height")] = json::value::number(nHeight);
	objParams[U("deviceScaleFactor")] = json::value::number(flDeviceScaleFactor);
	objParams[U("mobile")] = json::value::boolean(flDeviceScaleFactor);
	objParams[U("scale")] = json::value::number(flScale);
	objParams[U("screenWidth")] = json::value::number(nScreenWidth);
	objParams[U("screenHeight")] = json::value::number(nScreenHeight);
	objParams[U("positionX")] = json::value::number(nPositionX);
	objParams[U("positionY")] = json::value::number(nPositionY);
	objParams[U("fitWindow")] = json::value::boolean(true);
	if(szScreenOrientationType!=NULL)
		objScreenOrientation[U("type")] = json::value::string(szScreenOrientationType);
	objScreenOrientation[U("angle")] = json::value::number(nScreenOrientationAngle);

	objParams[U("screenOrientation")] = objScreenOrientation;

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ClearDeviceMetricsOverride
说明: 清除覆盖设备指标。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ClearDeviceMetricsOverride(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.clearDeviceMetricsOverride"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ForceViewport
说明: 覆盖页面的可见区域。 更改从页面隐藏，即可观察的滚动位置和页面比例不改变。 实际上，该命令将页面的指定区域移动到框架的左上角。
x: 区域左上角的X坐标（CSS像素）。
y: 区域左上角的Y坐标（CSS像素）。
scale: 缩放以应用于该区域（相对于1.0的页面比例）。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ForceViewport(double x, double y, double scale)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.forceViewport"));
	json::value objParams;
	objParams[U("x")] = json::value::number(x);
	objParams[U("y")] = json::value::number(y);
	objParams[U("scale")] = json::value::number(scale);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ResetViewport
说明: 将页面的可见区域重置为原始视口，撤消forceViewport命令的任何效果。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ResetViewport(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.resetViewport"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ResetPageScaleFactor
说明: 请求页面比例因子被重置为初始值。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ResetPageScaleFactor(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.resetPageScaleFactor"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetPageScaleFactor
说明: 设置特定的页面比例因子
flPageScaleFactor: 页面比例因子。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetPageScaleFactor(double flPageScaleFactor)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setPageScaleFactor"));
	json::value objParams;
	objParams[U("pageScaleFactor")] = json::value::number(flPageScaleFactor);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetVisibleSize
说明: 调整页面的框架/视口大小。 请注意，这不会影响框架的容器（例如浏览器窗口）。 可用于生成指定大小的屏幕截图。 Android不支持
nWidth: 宽度（DIP）。
nHeight: 高度（DIP）。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetVisibleSize(int nWidth, int nHeight)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setVisibleSize"));
	json::value objParams;
	objParams[U("width")] = json::value::number(nWidth);
	objParams[U("width")] = json::value::number(nWidth);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetGeolocationOverride
说明: 覆盖地理位置位置或错误。 省略任何参数模拟位置不可用。
flLatitude: 模拟纬度
flLongitude: 模拟经度
flAccuracy: 模拟精度
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetGeolocationOverride(double flLatitude, double flLongitude, double flAccuracy)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setGeolocationOverride"));
	json::value objParams;
	objParams[U("latitude")] = json::value::number(flLatitude);
	objParams[U("longitude")] = json::value::number(flLongitude);
	objParams[U("accuracy")] = json::value::number(flAccuracy);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ClearGeolocationOverride
说明: 清除覆盖的地理位置位置和错误
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ClearGeolocationOverride(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.clearGeolocationOverride"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetTouchEmulationEnabled
说明: 切换基于鼠标事件的触摸事件仿真。
bEnabled: 触摸事件仿真是否应启用
szConfiguration: 触摸/手势事件配置。 默认：当前平台。 允许的值：mobile, desktop
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetTouchEmulationEnabled(BOOL bEnabled, LPCTSTR configuration)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setEmitTouchEventsForMouse"));
	json::value objParams;
	objParams[U("enabled")] = json::value::boolean(bEnabled);
	objParams[U("configuration")] = json::value::string(configuration);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetEmulatedMedia
说明: 为CSS媒体查询模拟给定媒体。
szMedia: 要模拟的媒体类型 空字符串禁用覆盖。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetEmulatedMedia(LPCTSTR szMedia)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setEmulatedMedia"));
	json::value objParams;
	if(szMedia!=NULL)
		objParams[U("media")] = json::value::string(szMedia);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetCPUThrottlingRate
说明: 启用CPU限制来模拟缓慢的CPU。
rate: 节流率为减速因子（1为无油门，2为2倍减速等）。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetCPUThrottlingRate(double rate)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.setCPUThrottlingRate"));
	json::value objParams;
	objParams[U("rate")] = json::value::number(rate);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: CanEmulate
说明: 告诉是否支持仿真。
返回: 如果返回真则说明已开启仿真支持
*/
BOOL IChromeLink::CanEmulate(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Emulation.canEmulate"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		json::value objResult = objRoot[U("result")];
		return objResult[U("result")].as_bool();
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetUserAgentOverride
说明: 允许使用给定的字符串覆盖用户代理。
szUserAgent: 用户代理使用。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetUserAgentOverride(LPCTSTR szUserAgent)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.setUserAgentOverride"));
	json::value objParams;
	objParams[U("userAgent")] = json::value::string(szUserAgent);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetBlockedURLs
说明: 设置阻止加载的URL
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetBlockedURLs(std::vector<IString> &Urls)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	int nMax = Urls.size();
	if (nMax <= 0) return FALSE;
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.setBlockedURLs"));
	json::value objParams;
	json::value objArray;
	for (size_t i = 0; i < nMax; i++)
	{
		objArray[i] = json::value::string(Urls[i]);
	}
	objParams[U("urls")] = objArray;

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: CanClearBrowserCache
说明: 告诉是否支持清除浏览器缓存。
返回: 如果返回真则支持清除浏览器缓存
*/
BOOL IChromeLink::CanClearBrowserCache(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.canClearBrowserCache"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		json::value objResult = objRoot[U("result")];
		return objResult[U("result")].as_bool();
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ClearBrowserCache
说明: 清除浏览器缓存
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ClearBrowserCache(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.clearBrowserCache"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: CanClearBrowserCookies
说明: 告诉是否支持清除浏览器cookie。
返回: 如果返回真则支持清除浏览器cookie
*/
BOOL IChromeLink::CanClearBrowserCookies(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.canClearBrowserCookies"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		json::value objResult = objRoot[U("result")];
		return objResult[U("result")].as_bool();
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: ClearBrowserCookies
说明: 清除浏览器中的所有cookie
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::ClearBrowserCookies(void)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.clearBrowserCookies"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: GetCookies
说明: 返回当前网址的所有浏览器Cookie。 根据后端支持，将在Cookie字段中返回详细的Cookie信息。
szUrl: 将提取适用的Cookie的URL列表
Result: 存放取得的Cookie列表
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::GetCookies(LPCTSTR szUrl, IChromeCookieVectors &Result)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.getCookies"));
	json::value objParams;
	json::value objArray;
	objArray[0] = json::value::string(szUrl);
	Result.clear();
	objParams[U("urls")] = objArray;
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		try
		{
			json::value objResult = objRoot[U("result")];
			json::value objCookies = objResult[U("cookies")];
			int nMax = objCookies.size();
			IChromeCookie Info;
			for (size_t i = 0; i < nMax; i++)
			{
				json::value objChild = objCookies[i];
				Info.strDomain = objChild[U("domain")].as_string();
				Info.strName = objChild[U("name")].as_string();
				Info.strValue = objChild[U("value")].as_string();
				Info.strPath = objChild[U("path")].as_string();
				Info.bHttpOnly = objChild[U("httpOnly")].as_bool();
				Info.bSecure = objChild[U("secure")].as_bool();
				Result.push_back(Info);
			}
		}
		catch (const std::exception&)
		{
			return FALSE;
		}
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数:GetAllCookies
说明: 回所有浏览器Cookie。 根据后端支持，将在Cookie字段中返回详细的Cookie信息。
Result: 存放取得的Cookie列表
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::GetAllCookies(IChromeCookieVectors &Result)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.getAllCookies"));
	Result.clear();
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		try
		{
			json::value objResult = objRoot[U("result")];
			//MessageBox(NULL, objResult.serialize().c_str(), NULL, NULL);
			json::value objCookies = objResult[U("cookies")];
			int nMax = objCookies.size();
			IChromeCookie Info;
			for (size_t i = 0; i < nMax; i++)
			{
				json::value objChild = objCookies[i];
				Info.strDomain = objChild[U("domain")].as_string();
				Info.strName = objChild[U("name")].as_string();
				Info.strValue = objChild[U("value")].as_string();
				Info.strPath = objChild[U("path")].as_string();
				Info.bHttpOnly = objChild[U("httpOnly")].as_bool();
				Info.bSecure = objChild[U("secure")].as_bool();
				Info.expires = objChild[U("expires")].as_number().to_int64();
				Result.push_back(Info);
			}
		}
		catch (const std::exception&)
		{
			return FALSE;
		}
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: DeleteCookie
说明: 删除具有给定名称，域和路径的浏览器cookie。
szCookieName: 名称
szUrl: 匹配domain和path的URL。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::DeleteCookie(LPCTSTR szCookieName, LPCTSTR szUrl)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.deleteCookie"));
	json::value objParams;

	objParams[U("cookieName")] = json::value::string(szCookieName);
	objParams[U("url")] = json::value::string(szUrl);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SetCookie
说明: 使用给定的Cookie数据设置Cookie; 如果存在等同的cookie可能会覆盖。
szUrl: 与cookie设置关联的request-URI。 此值可能会影响创建的Cookie的默认domain和path值。
szName: 名称
szValue: 值
szDomain: 域
szPath: 路径
bSecure: 默认为false。
bHttpOnly: 默认为false.
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::SetCookie(LPCTSTR szUrl, LPCTSTR szName, LPCTSTR szValue, LPCTSTR szDomain, LPCTSTR szPath, BOOL bSecure,__int64 expires, BOOL bHttpOnly)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Network.setCookie"));
	json::value objParams;

	objParams[U("url")] = json::value::string(szUrl);
	objParams[U("name")] = json::value::string(szName);
	objParams[U("value")] = json::value::string(szValue);
	objParams[U("domain")] = json::value::string(szDomain);
	objParams[U("path")] = json::value::string(szPath);
	objParams[U("secure")] = json::value::boolean(bSecure);
	objParams[U("expires")] = json::value::number(expires);
	objParams[U("httpOnly")] = json::value::boolean(bHttpOnly);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: GetResourceList
说明: 获取页面资源列表
szFrameId: 要获取资源的框架
objVectors: 返回的资源列表
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::GetResourceList( IChromeResourceVectors &objVectors)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	objVectors.clear();
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.getResourceTree"));

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();

		try
		{
			json::value  objResult = objRoot[U("result")];
			json::value objFrameTree = objResult[U("frameTree")];
			EnumResourceCallback(objVectors, objFrameTree);

		}
		catch (const std::exception&)
		{
			return FALSE;
		}
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

void IChromeLink::EnumResourceCallback(IChromeResourceVectors &result, json::value &root) {
	IChromeResource info;
	json::value &frame = root[U("frame")];
	info.strFrameId = frame[U("id")].as_string();

	json::value &res = root[U("resources")];
	int len = res.size();
	for (size_t i = 0; i < len; i++)
	{
		auto &elenment = res[i];
		info.strUrl = elenment[U("url")].as_string();
		info.strMimeType = elenment[U("mimeType")].as_string();
		info.strType = elenment[U("type")].as_string();
		info.dwContentSize = elenment[U("contentSize")].as_number().to_uint32();
		result.push_back(info);
	}

	try
	{
		auto &child = root[U("childFrames")];
		EnumResourceCallback(result, child);
		return;
	}
	catch (const std::exception&)
	{

	}

}

/*
函数: DispatchKeyEvent
说明: 将一个关键事件发送到页面。
szType: 键事件的类型。 允许的值：keyDown，keyUp，rawKeyDown，char。
nModifiers: 位字段表示按下的修改键。 Alt = 1，Ctrl = 2，Meta / Command = 4，Shift = 8（默认值：0）。
dwTimeSinceEpoch: UTC时间，以秒计，从1970年1月1日开始
szText: 通过使用键盘布局处理虚拟键代码生成的文本。 keyUp和rawKeyDown事件不需要（默认：“”）
szUnModifiedText: 如果没有修改器被按下，则由键盘生成的文本（移位除外）。 用于快捷键（加速器）键处理（默认：“”）。
szKeyIdfentifier: 唯一键标识符（例如，“U + 0041”）（默认值：“”）。
szCode: 每个物理键的唯一DOM定义的字符串值（例如，“KeyA”）（默认：“”）。
szKey: 唯一的DOM定义的字符串值，描述了活动修饰符，键盘布局等上下文中键的含义（例如，“AltGr”）（默认值：“”）。
nWindowsVirtualKeyCode: Windows虚拟键代码（默认值：0）。
nNativeVirtualKeyCode: 本地虚拟键代码（默认值：0）。
bAutoRepeat: 事件是否由自动重复生成（默认值：false）。
bIsKeypad: 事件是否从小键盘生成（默认值：false）。
bIsSysttemKey: 事件是否是系统键事件（默认值：false）。
将一个关键事件发送到页面。
*/
BOOL IChromeLink::DispatchKeyEvent(LPCTSTR szType,
	int nModifiers,
	DWORD dwTimeSinceEpoch,
	LPCTSTR szText,
	LPCTSTR szUnModifiedText,
	LPCTSTR szKeyIdfentifier,
	LPCTSTR szCode,
	LPCTSTR szKey,
	int nWindowsVirtualKeyCode,
	int nNativeVirtualKeyCode,
	BOOL bAutoRepeat,
	BOOL bIsKeypad, BOOL bIsSysttemKey)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.dispatchKeyEvent"));
	json::value objParams;
	objParams[U("type")] = json::value::string(szType);
	objParams[U("modifiers")] = json::value::number(nModifiers);
	objParams[U("timestamp")] = json::value::number((unsigned)time(NULL));
	objParams[U("text")] = json::value::string(szText);
	objParams[U("unmodifiedText")] = json::value::string(szUnModifiedText);
	objParams[U("keyIdentifier")] = json::value::string(szKeyIdfentifier);
	objParams[U("code")] = json::value::string(szCode);
	objParams[U("key")] = json::value::string(szKey);
	objParams[U("windowsVirtualKeyCode")] = json::value::number(nWindowsVirtualKeyCode);
	objParams[U("nativeVirtualKeyCode")] = json::value::number(nNativeVirtualKeyCode);
	objParams[U("autoRepeat")] = json::value::boolean(bAutoRepeat);
	objParams[U("isKeypad")] = json::value::boolean(bIsKeypad);
	objParams[U("isSystemKey")] = json::value::boolean(bIsKeypad);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: DispatchMouseEvent
说明: 将鼠标事件调度到页面。
szType: 鼠标事件的类型。 允许的值：mousePressed，mouseReleased，mouseMoved。
x: 事件的X坐标相对于CSS像素中的主框架的视口。
y: 事件的Y坐标相对于CSS像素中的主框架视口。 0表示视口的顶部，Y随着进入视口底部而增加。
dwTimeSinceEpoch: UTC时间，以秒计，从1970年1月1日开始
szButton: 鼠标按钮（默认值：“无”）。 允许值：none,left,middle,right
clickCount: 单击鼠标按钮的次数（默认值：0）。
返回: 如果返回真则操作是成功的
*/
BOOL IChromeLink::DispatchMouseEvent(LPCTSTR szType,
	int x,
	int y,
	int modifiers,
	LPCTSTR szButton,
	int clickCount,
	DWORD deltaX,
	DWORD deltaY)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.dispatchMouseEvent"));
	json::value objParams;
	objParams[U("type")] = json::value::string(szType);
	objParams[U("x")] = json::value::number(x);
	objParams[U("y")] = json::value::number(y);
	objParams[U("modifiers")] = json::value::number(modifiers);
	objParams[U("timestamp")] = json::value::number((unsigned)time(NULL));
	objParams[U("button")] = json::value::string(szButton);
	objParams[U("clickCount")] = json::value::number(clickCount);
	objParams[U("deltaX")] = json::value::number((uint32_t)deltaX);
	objParams[U("deltaY")] = json::value::number((uint32_t)deltaY);

	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: DispatchTouchEvent
说明: 在页面上调度触摸事件。
szType: 触摸事件的类型。 允许的值：touchStart，touchEnd，touchMove。
nModifiers:
dwTimeStamp:
*/
BOOL IChromeLink::DispatchTouchEvent(LPCTSTR szType, IChromeTouchPoints *pTouchPoints, int nModifiers)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.dispatchTouchEvent"));
	json::value objParams;
	objParams[U("type")] = json::value::string(szType);
	if (pTouchPoints != NULL)
	{
		int nMax = pTouchPoints->size();
		if (nMax > 0)
		{
			json::value objTouchPoints = json::value::array(nMax);
			json::value objData;
			for (size_t i = 0; i < nMax; i++)
			{
				objData = json::value::null();
				objData[U("x")] = json::value::number((*pTouchPoints)[i].x);
				objData[U("y")] = json::value::number((*pTouchPoints)[i].y);
				objData[U("radiusX")] = json::value::number((*pTouchPoints)[i].rediusX);
				objData[U("radiusY")] = json::value::number((*pTouchPoints)[i].rediusY);
				objData[U("rotationAngle")] = json::value::number((*pTouchPoints)[i].rotationAngle);
				objData[U("force")] = json::value::number((*pTouchPoints)[i].force);
				objData[U("id")] = json::value::number((*pTouchPoints)[i].id);
				objTouchPoints[i] = objData;
			}
			objParams[U("touchPoints")] = objTouchPoints;
		}
	}
	else
	{
		json::value objTouchPoints = json::value::array(0);
		objParams[U("touchPoints")] = objTouchPoints;
	}
	objParams[U("modifiers")] = json::value::number(nModifiers);
	objParams[U("timestamp")] = json::value::number((unsigned)time(NULL));
	objRoot[TEXT("params")] = objParams;

	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;
	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot2 = pplx::create_task(objResponse).get();
		//MessageBox(NULL, objRoot2.serialize().c_str(), NULL, NULL);
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: EmulateTouchFromMouseEvent
说明: 从鼠标事件参数模拟发送一个触摸事件到页面
*/
BOOL IChromeLink::EmulateTouchFromMouseEvent(LPCTSTR szType, int x, int y, LPCTSTR szButton, double deltaX, double deltaY, int modifiers, int clickCount)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.emulateTouchFromMouseEvent"));
	json::value objParams;
	objParams[U("type")] = json::value::string(szType);
	objParams[U("x")] = json::value::number(x);
	objParams[U("y")] = json::value::number(y);
	objParams[U("timestamp")] = json::value::number((unsigned)time(NULL));
	objParams[U("button")] = json::value::string(szButton);
	objParams[U("deltaX")] = json::value::number((uint32_t)deltaX);
	objParams[U("deltaY")] = json::value::number((uint32_t)deltaY);
	objParams[U("modifiers")] = json::value::number(modifiers);
	objParams[U("clickCount")] = json::value::number(clickCount);
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数:  SynthesizePinchGesture
说明: 通过发送适当的触摸事件,在一段事件内合成捏合手势
*/
BOOL IChromeLink::SynthesizePinchGesture(double x, double y, double scaleFactor, int relativeSpeed, LPCTSTR gestureSourceType)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.synthesizePinchGesture"));
	json::value objParams;
	objParams[U("x")] = json::value::number(x);
	objParams[U("y")] = json::value::number(y);
	objParams[U("scaleFactor")] = json::value::number(scaleFactor);
	objParams[U("relativeSpeed")] = json::value::number(relativeSpeed);
	objParams[U("gestureSourceType")] = json::value::string(gestureSourceType);
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SynthesizeScrollGesture
说明: 通过发送适当的触摸事件,在一段事件内合成滚动手势
*/
BOOL IChromeLink::SynthesizeScrollGesture(double x, double y,
	double xDistance, double yDistance,
	double xOverscroll, double yOverscroll,
	BOOL preventFling, int speed, LPCTSTR gestureSourceType, int repeatCount, int repeatDelayMs)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.synthesizeScrollGesture"));
	json::value objParams;
	objParams[U("x")] = json::value::number(x);
	objParams[U("y")] = json::value::number(y);
	objParams[U("xDistance")] = json::value::number(xDistance);
	objParams[U("yDistance")] = json::value::number(yDistance);
	objParams[U("xOverscroll")] = json::value::number(xOverscroll);
	objParams[U("yOverscroll")] = json::value::number(yOverscroll);
	objParams[U("preventFling")] = json::value::boolean(preventFling);
	objParams[U("speed")] = json::value::number(speed);
	objParams[U("gestureSourceType")] = json::value::string(gestureSourceType);
	objParams[U("repeatCount")] = json::value::number(repeatCount);
	objParams[U("repeatDelayMs")] = json::value::number(repeatDelayMs);
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: SynthesizeTapGesture
说明: 通过发送适当的触摸事件,在一段事件内合成点击(Tep)手势
*/
BOOL IChromeLink::SynthesizeTapGesture(double x, double y, int duration, int tapCount, LPCTSTR gestureSourceType)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Input.synthesizeTapGesture"));
	json::value objParams;
	objParams[U("x")] = json::value::number(x);
	objParams[U("y")] = json::value::number(y);
	objParams[U("duration")] = json::value::number(duration);
	objParams[U("tapCount")] = json::value::number(tapCount);
	objParams[U("gestureSourceType")] = json::value::string(gestureSourceType);
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数:IsJavaScriptDialogOpening
说明: 检查当前页面是否已弹出alert,confirm,prompt,beforeunload等提示框
返回: 如果返回真则表示有对话框存在
*/
BOOL IChromeLink::IsJavaScriptDialogOpening(void)
{
	return this->m_javascriptDialogInfomation.bDialogLifed;
}

/*
函数: HandleJavaScriptDialog
说明: 接受或解除JavaScript启动的对话框（alert，confirm，prompt或onbeforeunload）。
*/
BOOL IChromeLink::HandleJavaScriptDialog(BOOL accept, LPCTSTR promptText)
{
	ASSERT_KEEP_LINKEDR(FALSE);
	DWORD dwAutoId = this->GetAutoRand();
	json::value objRoot;
	objRoot[U("id")] = json::value::number((int)dwAutoId);
	objRoot[U("method")] = json::value::string(U("Page.handleJavaScriptDialog"));
	json::value objParams;
	objParams[U("accept")] = json::value::boolean(accept);
	if(promptText!=NULL && lstrlen(promptText)>0)
		objParams[U("promptText")] = json::value::string(promptText);
	objRoot[TEXT("params")] = objParams;
	std::string strBody = conversions::utf16_to_utf8(objRoot.serialize());
	websocket_outgoing_message objMsg;
	pplx::task_completion_event<json::value> objResponse;

	objMsg.set_utf8_message(strBody);
	this->m_hUnionMutex.lock();
	this->m_mapRequestQueues.insert(std::make_pair(dwAutoId, objResponse));
	this->m_hUnionMutex.unlock();
	try
	{
		this->m_wsKeepLink.send(objMsg).get();
	}
	catch (const std::exception &ex)
	{
		return FALSE;
	}
	try
	{
		json::value objRoot = pplx::create_task(objResponse).get();
		return TRUE;
	}
	catch (const std::exception&)
	{
		return FALSE;
	}
	return FALSE;
}

/*
函数: GetJavaScriptDialogInfomation
Info: 返回获取到的javascript对话框信息
说明: 获取JavaScript对话框信息
*/
void IChromeLink::GetJavaScriptDialogInfomation(IChromeJavaScriptDialogInfomation &Info)
{
	Info.bDialogLifed = m_javascriptDialogInfomation.bDialogLifed;
	Info.strURL = m_javascriptDialogInfomation.strURL;
	Info.strMessage = m_javascriptDialogInfomation.strMessage;
	Info.strDefaultPrompt = m_javascriptDialogInfomation.strDefaultPrompt;
	Info.type = m_javascriptDialogInfomation.type;
}

/*
函数: GetConsoleLogs
logs: 返回控制台信息
获取控制日志信息
*/
void IChromeLink::GetConsoleLogs(std::wstring & logs)
{
	std::wstringstream ssBuffer;
	if (m_consoleInfos.empty())
		return;
	for (auto v: m_consoleInfos)
	{
		ssBuffer << v << TEXT("\r\n");
	}
	logs = ssBuffer.str();
	m_consoleInfos.clear();
}