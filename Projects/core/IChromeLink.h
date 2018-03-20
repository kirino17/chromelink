#pragma once
#include <Windows.h>
#include <sstream>
#include <string>
#include <Shlwapi.h>
#include <vector>
#include <list>
#include "cpprest\containerstream.h"
#include "cpprest\filestream.h"
#include "cpprest\http_client.h"
#include "cpprest\json.h"
#include "cpprest\producerconsumerstream.h"
#include "cpprest\details\web_utilities.h"
#include "cpprest\uri.h"
#include "cpprest\streams.h"
#include "cpprest\ws_client.h"
#include "cpprest\ws_msg.h"
#include "cpprest\asyncrt_utils.h"
#include <tchar.h>
#include <map>
#include <mutex>
#include <iostream>
#include <fstream>
#include <thread>
#include <malloc.h>
#include <Shellapi.h>
#include <thread>
#include <malloc.h>


#ifndef OVERRIDE
#define OVERRIDE override
#endif

/*
stable (1.2)

The 1.2 version of the protocol is the latest stable release of the protocol, tagged at Chrome 54.
It includes a smaller subset of the complete protocol compatibilities.

https://chromedevtools.github.io/devtools-protocol/1-2

*/

#pragma comment(lib,"Shlwapi.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"User32.lib")


using namespace ::pplx;
using namespace utility;
using namespace web;
using namespace concurrency::streams;
using namespace web::http;
using namespace web::http::client;
using namespace web::json;
using namespace web::websockets::client;

namespace Komari {

#ifdef _UNICODE 
	typedef std::wstringstream IStream;
	typedef std::wstring IString;
#else
	typedef std::stringstream IStream;
	typedef std::string IString;
#endif

	/*
		类型: IChromeTabInfo
		说明: 标签栏信息类型结构
	*/
	struct IChromeTabInfo {
		//标签描述
		IString strDescription;

		//Web调试地址
		IString strDevToolsFrontendUrl;

		//标签ID
		IString strId;

		//标签标题
		IString strTitle;

		//标签类型
		IString strType;

		//地址
		IString strUrl;

		//WS调试地址
		IString strWebSocketDebuggerUrl;
	};

	//所有打开的标签页信息
	typedef std::vector<IChromeTabInfo> IChromeCurTabs;

	//关联对象
	typedef std::map<DWORD, pplx::task_completion_event<json::value>> IChromeResponse;

	//ExecutionContextId 列表
	struct IChromeExecutionContextId {
		DWORD dwExecutionContextId;
		IString strFrameId;
	};
	typedef std::vector<IChromeExecutionContextId> IChromeV8ExecutionContextIdVectors;

	//框架列表
	struct IChromeFrameId {
		IString strFrameId;
		IString strLoaderId;
		IString strMimeType;
		IString strName;
		IString strParentId;
		IString strUrl;
	};
	typedef std::vector<IChromeFrameId> IChromeFrameIdVectors;

	//脚本编译信息
	struct IChromeExceptionDetails {
		DWORD dwExceptionId;
		IString strText;
		DWORD dwLineNumber;
		DWORD dwColumnNumber;
		IString strScriptId;
		IString strUrl;
		DWORD dwexecutionContextId;
	};
	struct IChromeCompileResult {
		IString strScriptId;
		IChromeExceptionDetails objExceptionDetails;
		IChromeCompileResult()
		{
			objExceptionDetails.dwExceptionId = 0;
			objExceptionDetails.dwLineNumber = 0;
			objExceptionDetails.dwColumnNumber = 0;
			objExceptionDetails.dwexecutionContextId = 0;
		}
	};

	//表达式执行结果
	struct IChromeEvalResult {
		IString strDescription;
		IString strType;
		IString strSubType;
		IString strClassName;
		IString strValue;
		BOOL bValue;
		double flValue;
		int nValue;
		BOOL bException;
		IChromeEvalResult()
		{
			bException = FALSE;	//是否存在错误
			bValue = FALSE;
			flValue = 0.0f;
			nValue = 0;
		}
	};

	struct IChromeTouchPoint {
		double x;	//事件的X坐标相对于主框架的视口。
		double y;//事件的Y坐标相对于主框架的视口。 0表示视口的顶部，Y随着进入视口底部而增加。
		double rediusX; //触摸区域的X半径（默认值：1）。
		double rediusY; //触摸区域的Y半径（默认值：1）。
		double rotationAngle; //旋转角度（默认值：0.0）。
		double force;//Force (default: 1.0).
		double id;//用于跟踪事件之间的触摸源的标识符在事件中必须是唯一的。
	};

	typedef std::vector<IChromeTouchPoint> IChromeTouchPoints;

	enum IChromeReadyStatus {
		ICR_SIGNAL,
		ICR_WAITING
	};

	//COOKIE信息
	struct IChromeCookie {
		IString strName;
		IString strValue;
		IString strDomain;
		IString strPath;
		BOOL bHttpOnly;
		BOOL bSecure;
		__int64 expires;
	};
	typedef std::vector<IChromeCookie> IChromeCookieVectors;

	//资源信息
	struct IChromeResource {
		IString strUrl;
		IString strType;
		IString strMimeType;
		DWORD dwContentSize;
		IString strFrameId;
	};
	typedef std::vector<IChromeResource> IChromeResourceVectors;

	//JavaScript对话框信息
	enum IChromeJavaScriptDialogType {
		IC_JSDT_ALERT,
		IC_JSDT_CONFIRM,
		IC_JSDT_PROMPT,
		IC_JSDT_BEFOREUNLOAD
	};

	struct IChromeJavaScriptDialogInfomation {
		IString strURL;
		IString strMessage;
		IChromeJavaScriptDialogType type;
		IString strDefaultPrompt;
		BOOL bDialogLifed;
	};

	/////////////////////////////////////////////////////////////////////////////////////////////
	//建立智能指针
	class __declspec(novtable) IChromeAutoSink {
	public:
		void virtual AddRef(void) = 0;		//增加引用计数
		bool virtual Release(void) = 0;		//释放引用计数
		bool virtual HasOneRef(void) = 0;		//是否只有一个引用
	};
	//引用计数类
	class IChromeRefCount : public IChromeAutoSink {
	public:
		IChromeRefCount() :ref_count_(0) {};

	public:
		virtual void AddRef(void) OVERRIDE { InterlockedIncrement(&ref_count_); }
		virtual bool Release(void) OVERRIDE { volatile long res = InterlockedDecrement(&ref_count_); if (res == 0) return true; else return false; }
		virtual bool HasOneRef(void) OVERRIDE { if (ref_count_ == 1) return true; else return false; }

	private:
		volatile long ref_count_;
	};

	//增加引用计数宏
#define IMPLEMENT_REFCOUNTING(ClassName) \
public: \
	void AddRef() { ref_.AddRef(); } \
	bool Release() {\
if (ref_.Release()) {\
	delete static_cast<const ClassName*>(this); \
	return true; \
} \
else return false; \
	}\
	bool HasOneRef() { return ref_.HasOneRef(); } \
private: \
	IChromeRefCount ref_;
	/////////////////////////////////////////////////////////////////////////////////////////////

	//Chrome访问接口类
	class IChromeLink : public IChromeAutoSink {
	public:
		explicit IChromeLink(LPCTSTR szWebSocketDebuggerUrl);
		~IChromeLink();

	protected:
		IMPLEMENT_REFCOUNTING(IChromeLink);

	public:
		/////////////////////////静态函数/////////////////////////

		/*
				函数: GetTabs
				说明: 获取浏览器中所有已打开的标签页信息
				szTraceAddr : 浏览器调试地址
				Tabs : 返回获得的标签页列表
				返回: 如果返回真则说明调用是成功的。
		*/
		BOOL static GetTabs(LPCTSTR szTraceAddr, IChromeCurTabs &Tabs);

		/*
				函数: NewTab
				说明: 打开一个新的标签页
				szTraceAddr : 浏览器调试地址
				szUrl : 设置页访问的地址
				Tab : 返回已打开的标签页信息
				返回: 如果返回真则说明调用是成功的。
		*/
		BOOL static NewTab(LPCTSTR szTraceAddr, LPCTSTR szUrl, IChromeTabInfo &Tab);

		/*
				函数: CloseTab
				说明: 关闭一个标签页。
				szTraceAddr : 浏览器调试地址
				szId : 标签的ID
				返回: 如果返回真则说明调用是成功的。
		*/
		BOOL static CloseTab(LPCTSTR szTraceAddr, LPCTSTR szId);

		/*
				函数: ActivateTab
				说明: 激活一个标签页。
				szTraceAddr : 浏览器调试地址
				szId : 标签的ID
				返回: 如果返回真则说明调用是成功的。
		*/
		BOOL static ActivateTab(LPCTSTR szTraceAddr, LPCTSTR szId);

		/*
				函数: GetTabCount
				说明: 获取标签页数量。
				返回: 如果成功将返回数量。
		*/
		int static GetTabCount(LPCTSTR szTraceAddr);

	public:

		/*
				函数: LinkStart
				说明: 连接至指定的标签页。
				返回: 如果连接成功将返回真。
		*/
		BOOL LinkStart(void);

		/*
				函数: Close
				说明: 关闭连接
		*/
		void Close(void);

	private:
		//ws连接对象
		websocket_client m_wsKeepLink;

		//用于关联Request和Response的互斥对象
		std::mutex m_hUnionMutex;
		IChromeResponse m_mapRequestQueues;

		//增量ID
		volatile DWORD m_dwAutoIdentify;

		//连接状态
		BOOL m_bLinkStatus;
#define ASSERT_KEEP_LINKEDR(x)	if(this->m_bLinkStatus == FALSE) return x;
#define ASSERT_KEEP_LINKED	if(this->m_bLinkStatus == FALSE) return;

		//V8 ExecutionContextId
		IChromeV8ExecutionContextIdVectors m_vecExecutionContextId;

		//响应事件
		pplx::task_completion_event<BOOL> m_taskReady;

		//子框架列表
		IChromeFrameIdVectors m_vecChildFrameIdLists;

		//主框架
		IChromeFrameId m_objMainFrameId;

		//就绪作业对象
		HANDLE m_hEventReady;

		//继续状态
		BOOL m_bReadyState;

		//初始化状态
		BOOL m_bInitializedSucess;

		//控制台信息
		std::vector<std::wstring> m_consoleInfos;

	private:
		//调试地址
		IString m_strTraceAddr;

		//JavaScript对话框信息
		IChromeJavaScriptDialogInfomation m_javascriptDialogInfomation;

	private:
		//内部消息调度线程
		void CALLBACK OnEventDispatchs(IChromeLink *pChromeLink);

		//调度事件消息
		void SendEventDispatchs(const json::value &objRoot);

		//获取一个增量ID
		DWORD GetAutoRand(void);

		//获取网页结构
		pplx::task<json::value> GetWebSheet(void);

		//解析网页结构
		void OnParserWebTree(const json::value &objRoot);

		//解析网页框架
		void EnumWebFrame(const json::value &objRoot);

		//获取框架
		void GetWebFrame(const json::value &objRoot);

		//获取V8上下文执行环境
		pplx::task<BOOL> GetExecutionContextId(void);
	private:
		///////////////////////////域接口类///////////////////////

		/*
					函数: OnLinkSucess
					说明: 在成功连接到浏览器后进行初始化设置。
					返回: 初始化成功则返回真。
		*/
		BOOL OnLinkSucess(void);

		/*
					函数: OpenPageEvents
					说明: 开启接收"Page"域的事件信息
					返回: 成功返回Response。
		*/
		pplx::task<json::value> OpenPageEvents(void);

		/*
					函数: OpenRuntimeEvents
					说明: 开启接收"Runtime"域的事件信息
					返回: 成功返回Response
		*/
		pplx::task<json::value> OpenRuntimeEvents(void);


		BOOL OpenNetwork(void);

	public:
		/*
					函数: GetMainFrame
					说明: 获取标签页中的主框架对象
					返回: 成功将返回主框架信息。
		*/
		const IChromeFrameId &GetMainFrame(void);


		/*
					函数: GetFrameCount
					说明: 获取标签页中的子框架数
					返回: 成功返回子框架数量。
		*/
		int GetFrameCount(void);

		/*
					函数: GetChildFrame
					说明: 获取子框架对象
					index: 下标,从0开始
					返回: 成功将返回子框架信息。
		*/
		const IChromeFrameId &GetChildFrame(int index);

		/*
					函数: GetExecutionContextCount
					说明: 获取V8上下文执行环境数量
					返回: 成功返回环境数量
		*/
		int GetExecutionContextCount(void);

		/*
					函数: GetExecutionContextId
					说明: 获取V8上下文执行环境
					返回: 成功返回上下文执行环境对象
		*/
		const IChromeExecutionContextId &GetExecutionContextId(int index);

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
		BOOL Evaluate(LPCTSTR szExpression, LPCTSTR szObjectGroup, BOOL bIncludeCommandLineAPI, BOOL bSilent, DWORD dwExecutionContextId, BOOL bReturnByValue, BOOL bGeneratePreview, BOOL bUserGesture, BOOL bAwaitPromise, IChromeEvalResult &objResult);

		/*
					函数: ReleaseObjectGroup
					说明: 释放属于给定组的所有远程对象。
					szObjectGroup: 符合组名
					返回: 成功返回真,失败返回假
		*/
		BOOL ReleaseObjectGroup(LPCTSTR szObjectGroup);

		/*
					函数:  SetReady
					说明: 设置就绪状态
					enStatus: 设置就绪状态类型
		*/
		void SetReady(IChromeReadyStatus enStatus);

		/*
					函数: WaitReady
					nWaitTime: 等待的时间,默认为: INFINITE,但不推荐使用默认
					说明: 等待至就绪状态
					返回: 返回等待的结果
		*/
		IChromeReadyStatus WaitReady(int nWaitTime = INFINITE);

		/*
					函数: Navigate
					说明: 跳转到指定页面
					szUrl: 欲访问的地址
					szReferrer: 来路设置
					返回: 如果返回真则操作是成功的
		*/
		BOOL Navigate(LPCTSTR szUrl, LPCTSTR szReferrer);

		/*
					函数: ReLoad
					说明: 重新加载页面
					bIgnoreCache: 是否忽略缓存进行重新加载
					szScriptToEvaluateOnLoad: 如果设置，脚本将被重新加载后注入被检查页面的所有帧。
					返回: 如果返回真则操作是成功的
		*/
		BOOL ReLoad(BOOL bIgnoreCache, LPCTSTR szScriptToEvaluateOnLoad);

		/*
					函数: GetResourceContent
					说明: 返回给定资源的内容
					strRetContent: 返回获得的资源内容
					bBase64Encoded: 返回的内容是否已使用base64编码
					返回: 如果返回真则操作是成功的
		*/
		BOOL GetResourceContent(LPCTSTR szFrameId, LPCTSTR szUrl, IString &strRetContent, BOOL &bBase64Encoded);

		/*
					函数: IsLinked
					说明: 是否仍然保持着连接
					返回: 如果依然连接则返回真,已断开则返回假
		*/
		BOOL IsLinked(void) { return this->m_bLinkStatus; }

		/*
					函数: UpdateFrame
					说明: 更新所记录的框架信息
					返回: 如果返回真则操作是成功的
		*/
		BOOL UpdateFrame(void);

		/*
					函数: SetDocumentContent
					说明: 设置指定框架中的HTML代码
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetDocumentContent(LPCTSTR szFrameId, LPCTSTR szHtml);

		/*
					函数: CaptureScreenshot
					说明: 拍摄页面快照
					szFormat: 快照的存储格式,可选值: jpeg,png
					nQuality: 压缩质量范围(0-100),仅限jpeg
					bFromSurface: 从表面捕获屏幕截图，而不是视图。 默认为true.
					返回: 如果返回真则操作是成功的
		*/
		BOOL CaptureScreenshot(LPCTSTR szFormat, int nQuality, BOOL bFromSurface, IString &strData);

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
					dontSetVisibleSize: 不改变可视区大小
					szScreenOrientationType: 屏幕方向
					nScreenOrientationAngle: 屏幕角度
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetDeviceMetricsOverride(int nWidth,
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
			int nScreenOrientationAngle);

		/*
					函数: ClearDeviceMetricsOverride
					说明: 清除覆盖设备指标。
					返回: 如果返回真则操作是成功的
		*/
		BOOL ClearDeviceMetricsOverride(void);

		/*
					函数: ForceViewport
					说明: 覆盖页面的可见区域。 更改从页面隐藏，即可观察的滚动位置和页面比例不改变。 实际上，该命令将页面的指定区域移动到框架的左上角。
					x: 区域左上角的X坐标（CSS像素）。
					y: 区域左上角的Y坐标（CSS像素）。
					scale: 缩放以应用于该区域（相对于1.0的页面比例）。
					返回: 如果返回真则操作是成功的
		*/
		BOOL ForceViewport(double x, double y, double scale);

		/*
					函数: ResetViewport
					说明: 将页面的可见区域重置为原始视口，撤消forceViewport命令的任何效果。
					返回: 如果返回真则操作是成功的
		*/
		BOOL ResetViewport(void);

		/*
					函数: ResetPageScaleFactor
					说明: 请求页面比例因子被重置为初始值。
					返回: 如果返回真则操作是成功的
		*/
		BOOL ResetPageScaleFactor(void);

		/*
					函数: SetPageScaleFactor
					说明: 设置特定的页面比例因子
					flPageScaleFactor: 页面比例因子。
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetPageScaleFactor(double flPageScaleFactor);

		/*
					函数: SetVisibleSize
					说明: 调整页面的框架/视口大小。 请注意，这不会影响框架的容器（例如浏览器窗口）。 可用于生成指定大小的屏幕截图。 Android不支持
					nWidth: 宽度（DIP）。
					nHeight: 高度（DIP）。
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetVisibleSize(int nWidth, int nHeight);

		/*
					函数: SetGeolocationOverride
					说明: 覆盖地理位置位置或错误。 省略任何参数模拟位置不可用。
					flLatitude: 模拟纬度
					flLongitude: 模拟经度
					flAccuracy: 模拟精度
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetGeolocationOverride(double flLatitude, double flLongitude, double flAccuracy);

		/*
					函数: ClearGeolocationOverride
					说明: 清除覆盖的地理位置位置和错误
					返回: 如果返回真则操作是成功的
		*/
		BOOL ClearGeolocationOverride(void);

		/*
					函数: SetTouchEmulationEnabled
					说明: 切换基于鼠标事件的触摸事件仿真。
					bEnabled: 触摸事件仿真是否应启用
					szConfiguration: 触摸/手势事件配置。 默认：当前平台。 允许的值：mobile, desktop
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetTouchEmulationEnabled(BOOL bEnabled, LPCTSTR configuration);

		/*
					函数: SetEmulatedMedia
					说明: 为CSS媒体查询模拟给定媒体。
					szMedia: 要模拟的媒体类型 空字符串禁用覆盖。
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetEmulatedMedia(LPCTSTR szMedia);

		/*
					函数: SetCPUThrottlingRate
					说明: 启用CPU限制来模拟缓慢的CPU。
					rate: 节流率为减速因子（1为无油门，2为2倍减速等）。
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetCPUThrottlingRate(double rate);

		/*
					函数: CanEmulate
					说明: 告诉是否支持仿真。
					返回: 如果返回真则说明已开启仿真支持
		*/
		BOOL CanEmulate(void);

		/*
					函数: SetUserAgentOverride
					说明: 允许使用给定的字符串覆盖用户代理。
					szUserAgent: 用户代理使用。
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetUserAgentOverride(LPCTSTR szUserAgent);

		/*
					函数: SetBlockedURLs
					说明: 设置阻止加载的URL
					返回: 如果返回真则操作是成功的
		*/
		BOOL SetBlockedURLs(std::vector<IString> &Urls);

		/*
					函数: CanClearBrowserCache
					说明: 告诉是否支持清除浏览器缓存。
					返回: 如果返回真则支持清除浏览器缓存
		*/
		BOOL CanClearBrowserCache(void);

		/*
					函数: ClearBrowserCache
					说明: 清除浏览器缓存
					返回: 如果返回真则操作是成功的
		*/
		BOOL ClearBrowserCache(void);

		/*
					函数: CanClearBrowserCookies
					说明: 告诉是否支持清除浏览器cookie。
					返回: 如果返回真则支持清除浏览器cookie
		*/
		BOOL CanClearBrowserCookies(void);

		/*
					函数: ClearBrowserCookies
					说明: 清除浏览器中的所有cookie
					返回: 如果返回真则操作是成功的
		*/
		BOOL ClearBrowserCookies(void);

		/*
					函数: GetCookies
					说明: 返回当前网址的所有浏览器Cookie。 根据后端支持，将在Cookie字段中返回详细的Cookie信息。
					szUrl: 将提取适用的Cookie的URL列表
					Result: 存放取得的Cookie列表
					返回: 如果返回真则操作是成功的
		*/
		BOOL GetCookies(LPCTSTR szUrl, IChromeCookieVectors &Result);

		/*
					函数:GetAllCookies
					说明: 回所有浏览器Cookie。 根据后端支持，将在Cookie字段中返回详细的Cookie信息。
					Result: 存放取得的Cookie列表
					返回: 如果返回真则操作是成功的
		*/
		BOOL GetAllCookies(IChromeCookieVectors &Result);

		/*
					函数: DeleteCookie
					说明: 删除具有给定名称，域和路径的浏览器cookie。
					szCookieName: 名称
					szUrl: 匹配domain和path的URL。
					返回: 如果返回真则操作是成功的
		*/
		BOOL DeleteCookie(LPCTSTR szCookieName, LPCTSTR szUrl);

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
		BOOL SetCookie(LPCTSTR szUrl, LPCTSTR szName, LPCTSTR szValue, LPCTSTR szDomain, LPCTSTR szPath, BOOL bSecure,__int64 expires, BOOL bHttpOnly);

		/*
					函数: GetResourceList
					说明: 获取页面资源列表
					szFrameId: 要获取资源的框架
					objVectors: 返回的资源列表
					返回: 如果返回真则操作是成功的
		*/
		BOOL GetResourceList(IChromeResourceVectors &objVectors);
		void EnumResourceCallback(IChromeResourceVectors &objVectors,json::value &root);

		/*
					函数: IsLoading
					说明: 检查浏览器是否正在载入页面中
					返回: 如果返回真则说明还在载入网页中
		*/
		BOOL IsLoading(void) { return m_bReadyState; }

		/*
					函数: SetIgnoreInputEvents
					说明: 忽略输入事件（在审核页面时很有用）。
					bIgnore: 是否忽略
					返回: 如果返回真则操作是成功的
		*/
		//BOOL SetIgnoreInputEvents(BOOL bIgnore);

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
		BOOL DispatchKeyEvent(LPCTSTR szType,
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
			BOOL bIsKeypad, BOOL bIsSysttemKey);

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
		BOOL DispatchMouseEvent(LPCTSTR szType,
			int x,
			int y,
			int modifiers,
			LPCTSTR szButton,
			int clickCount,
			DWORD deltaX,
			DWORD deltaY);

		/*
					函数: DispatchTouchEvent
					说明: 在页面上调度触摸事件。
					szType: 触摸事件的类型。 允许的值：touchStart，touchEnd，touchMove。
					pTouchPoints: 触摸点列表
					nModifiers: 状态键
		*/
		BOOL DispatchTouchEvent(LPCTSTR szType, IChromeTouchPoints *pTouchPoints, int nModifiers);

		/*
					函数: EmulateTouchFromMouseEvent
					说明: 从鼠标事件参数模拟发送一个触摸事件到页面
		*/
		BOOL EmulateTouchFromMouseEvent(LPCTSTR szType, int x, int y, LPCTSTR szButton, double deltaX, double deltaY, int modifiers, int clickCount);

		/*
					函数:  SynthesizePinchGesture
					说明: 通过发送适当的触摸事件,在一段事件内合成捏合手势
		*/
		BOOL SynthesizePinchGesture(double x, double y, double scaleFactor, int relativeSpeed, LPCTSTR gestureSourceType);

		/*
					函数: SynthesizeScrollGesture
					说明: 通过发送适当的触摸事件,在一段事件内合成滚动手势
		*/
		BOOL SynthesizeScrollGesture(double x, double y,
			double xDistance, double yDistance,
			double xOverscroll, double yOverscroll,
			BOOL preventFling, int speed, LPCTSTR gestureSourceType, int repeatCount, int repeatDelayMs);

		/*
					函数: SynthesizeTapGesture
					说明: 通过发送适当的触摸事件,在一段事件内合成点击(Tep)手势
		*/
		BOOL SynthesizeTapGesture(double x, double y, int duration, int tapCount, LPCTSTR gestureSourceType);


		/*
				函数:IsJavaScriptDialogOpening
				说明: 检查当前页面是否已弹出alert,confirm,prompt,beforeunload等提示框
				返回: 如果返回真则表示有对话框存在
		*/
		BOOL IsJavaScriptDialogOpening(void);

		/*
				函数: HandleJavaScriptDialog
				accept: 接收或接触对话
				promptText: 输入文本
				说明: 接受或解除JavaScript启动的对话框（alert，confirm，prompt或onbeforeunload）。
		*/
		BOOL HandleJavaScriptDialog(BOOL accept, LPCTSTR promptText);

		/*
				函数: GetJavaScriptDialogInfomation
				Info: 返回获取到的javascript对话框信息
				说明: 获取JavaScript对话框信息
		*/
		void GetJavaScriptDialogInfomation(IChromeJavaScriptDialogInfomation &Info);

		/*
					函数: GetConsoleLogs
					logs: 返回控制台信息
					获取控制日志信息
		*/
		void GetConsoleLogs(std::wstring & logs);
	};
}