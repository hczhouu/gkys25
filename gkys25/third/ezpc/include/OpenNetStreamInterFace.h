/** \file      OpenNetStreamInterFace.h
 *  \copyright HangZhou Ezviz All Right Reserved.
 *  \brief     开放SDK接口定义
 *
 *  \author    panlong, peter, zoujinwei
 *  \date      2014/12/15
 *
 *  \note      history:
 *  \note      V0.0.1 2014/12/15 peter 修改文件注释 
 *  \note      V5.1.12 2022/12/08 zoujinwei  
 */

#ifndef _H_OPENNETSTREAMINTERFACE_H_
#define _H_OPENNETSTREAMINTERFACE_H_

#include "OpenNetStreamDefine.h"
#include "OpenNetStreamError.h"

#if defined (_WIN32) || defined(_WIN64)
#include <Windows.h>
#ifdef OPENNETSTREAM_EXPORTS
#  define OPENSDK_API __declspec(dllexport)
#else
#  define OPENSDK_API __declspec(dllimport)
#endif
#elif defined (OS_POSIX) || defined (__APPLE__) || defined(ANDROID) || defined (__linux__)
#include <cstdio>
#  define OPENSDK_API
#  define CALLBACK

#if defined(__linux__)
typedef unsigned int HWND;
#else
typedef void* HWND;
#endif

#else
#  error os not support!
#endif

#ifdef __cplusplus
extern "C" {
#endif

/** 
 *  \ingroup alloc
 *  \brief 信息回调
 *  \param szSessionId  申请的会话ID
 *  \param iMsgType     消息号 #MessageType
 *	\param iErrorCode   错误码 OpenNetStreamError.h
 *  \param pMessageInfo 信息
 *  \param pUser		用户自定义数据 
 *
 *  \note  
 *	涉及的功能接口包括预览、回放、回放查询、布撤防、云台控制、语音对讲
 *  比方回放查询接口，当iMsgType == INS_RECORD_FILE时，pMessageInfo的格式如下
 *  \code {.json}
 *  {
 *		"FileSize":10, 
 *		"FileList": [
 *			{
 *				"StartTime":"",
 *				"EndTime":""
 *			}
 *		]
 *	}
 *  \endcode
 *  
 *  \sa OpenSDK_AllocSession()
 */
typedef void (CALLBACK *OpenSDK_MessageHandler)(const char* szSessionId, unsigned int iMsgType, unsigned int iErrorCode,const char *pMessageInfo, void *pUser);

/** \defgroup init SDK初始化
 *  @{
 */

/** 
 *  \brief 初始化库, 支持配置平台地址
 *  \param[in] szAuthAddr 认证域名:开放平台认证中心地址，默认地址为：https://openauth.ys7.com .对于开发者而言，请填写此默认地址即可. 
 *  \param[in] szPlatform 平台域名:开放平台后台地址，默认地址为：https://open.ys7.com .对于开发者而言，请填写此默认地址即可.
 *  \param[in] szAppId    向平台申请的AppKey
 *  \param[in] bOverseas  区分国外国内 false:国内 true:国外
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_FiniLib()
 *
 *  \note
 *  该接口必须在调用其他接口之前调用  非小权限调用此接口初始化
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_InitLib(const char* szAuthAddr, const char* szPlatform, const char* szAppId, const bool bOverseas);


/** 
 *  \brief 初始化库, 支持配置平台地址
 *  \param[in] szAuthAddr 认证域名:开放平台认证中心地址，默认地址为：https://openauth.ys7.com .对于开发者而言，请填写此默认地址即可. 
 *  \param[in] szPlatform 平台域名:开放平台后台地址，默认地址为：https://open.ys7.com .对于开发者而言，请填写此默认地址即可.
 *  \param[in] szAppId    向平台申请的AppKey
 *  \param[in] bOverseas  区分国外国内 false:国内 true:国外
 *  \param[in] iCientType 客户端功能区分 小权限需将iCientType=1 设置为1  详见：OpenSDK_ClientType 
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_FiniLib()
 *
 *  \note
 *  该接口必须在调用其他接口之前调用  使用小权限调用此接口初始化
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_InitClientLib(const char* szAuthAddr, const char* szPlatform, const char* szAppId, const bool bOverseas, const int iCientType);


/** 
 *  \brief  设置非设备类token
 *  \return 0表示成功，-1表示失败
 * 
 *  \note
 *  小权限调用初始化(OpenSDK_InitClientLib)之后须调用此接口
 * 
 *  \sa 
 */

OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetNonDeviceTkToken(const char* szNonDeviceTkToken);


/** 
 *  \brief  设置设备类TkToken 格式如下
 *  \return 0表示成功，-1表示失败
 * 
 *  \note
 *  小权限调用初始化(OpenSDK_InitClientLib)之后须调用此接口
 * 
 *  \sa 
 */

/*
设备类tkToken按照以下格式提供 (需要用json序列化成字符串传入该接口)
                                  字段对应类型         释义： 
[  
    {                            
        "deviceSerial":            "string",             设备序列号
        "channelNo":  ,             string               通道号
        "deviceTkToken":           "string",             设备类token       
        "deviceVideoTkToken":      "string",             设备类通道级
        "previewTkToken":          "string",             预览token
        "playTackTkToken":         "string",             回放token
        "talkTkToken":             "string"              对讲token
    },
    {                            
        "deviceSerial":            "string",             设备序列号
        "channelNo": ,              string               通道号
        "deviceTkToken":           "string",             设备类token       
        "deviceVideoTkToken":      "string",             设备类通道级
        "previewTkToken":          "string",             预览token
        "playTackTkToken":         "string",             回放token
        "talkTkToken":             "string"              对讲token
    }
]
*/

OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetDeviceTkTokens(const char* szDeviceTkTokens);


/** 
 *  \brief 初始化库, 默认国内版本使用的接口
 *  \param[in] szAppId    向平台申请的AppKey
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_FiniLib()
 *
 *  \note
 *  该接口必须在调用其他接口之前调用
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Init(const char* szAppId);

/** 
 *  \brief  反初始化库
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_InitLib()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_FiniLib();

/** 
 *  \brief  全局配置信息, 根据ConfigKey进行配置
 *  \param[in] iKey    配置类型, 参见 #ConfigKey
 *  \param[in] iValue  配置数值,如果key=CONFIG_DATA_UTF8, value=1时表示数据输出为Utf8格式
 *                               如果key=CONFIG_OPEN_STREAMTRANS, value=1时表示视频码流经过转封装库输出,用于录像
 *                               如果key=CONFIG_CLOSE_P2P, value=1时表示关闭P2P功能
 *                               如果key=CONFIG_LOG_LEVEL, value为日志等级, 参见 #OpenSDK_LogLevel
 *                               如果key=CONFIG_P2P_MAXNUM, value = 25, 设置支持的最大预链接路数，默认25路
 *                               如果key=CONFIG_CLOSE_REPORT, value=1表示关闭数据上报
 *  \return
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetConfigInfo(ConfigKey iKey, const int iValue);


/** 
 *  \brief  设置预链接信息回调
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetMessageCallback(OpenSDK_MessageHandler pHandle);

/** 
 *  \brief  设置认证地址, 通过调用此接口重新设置认证服务器地址.
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetAuthAddr(const char* szAuthAddr);

/** 
 *  \brief  设置登录认证地址, 通过调用此接口设置登录认证服务器地址.
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetLoginAuthAddr(const char* szAuthAddr);

/** 
 *  \brief  设置平台地址,海外平台地址重定向后, 通过调用此接口重新设置平台地址.
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetPlatformAddr(const char* szPlatform);

/** 
 *  \brief  设置AppKID
 *  \return 0表示成功，-1表示失败
 *  \sa 
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetAppID(const char* szAppId);

/** 
 *  \brief  设置AccessToken 使用小权限TkToken时,不能再使用该接口刷新token需调用该接口OpenSDK_SetDeviceTkTokens
 *  \return 0表示成功，-1表示失败
 *  \sa 
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetAccessToken(const char* szAccessToken);

/** 
 *  \brief  设置客户端版本（接入用户）
 *  \return 0表示成功，-1表示失败
 *  \sa 
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetClientVer(const char* szClientVer);

/** @} */ // end of init

/** \defgroup alloc 分配会话
 *  @{
 */

/** 
 *  \brief 申请一个会话Id
 *  \param[in]  pHandle     设置回调函数， #OpenSDK_MessageHandler
 *  \param[in]  pUser       用户自定义数据，会在pHandle中原样抛出
 *  \param[out] pSession    用于接收分配的会话ID
 *  \param[out] iSessionLen pSession的长度
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_FreeSession()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_AllocSessionEx(OpenSDK_MessageHandler pHandle, void* pUser, char** pSession, int* iSessionLen);

/** 
 *  \brief 销毁SDK操作句柄
 *  \param[in] szSessionId 会话Id，通过OpenSDK_AllocSession()创建
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_AllocSession()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_FreeSession(const char* szSessionId);

/** 
 *  \brief  针对Session设置配置信息, 根据ConfigKey进行配置, 取流之前调用
 *  \param[in] iKey    ConfigKey, 配置类型
 *  \param[in] iValue  配置数值, 如果key=CONFIG_OPEN_STREAMTRANS, value=1时表示视频码流经过转封装库输出,用于录像
 *  \return
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetSessionConfig(const char* szSessionId, ConfigKey iKey, const int iValue);

/** @} */ // end of alloc

/** \defgroup play 预览（直播）接口
 *  @{
 */

/** 
 *  \brief 数据回调格式
 *  \param enType 数据类型，参见 #DataType
 *  \param pData  数据内容
 *  \param iLen   数据长度
 *  \param pUser  用户自定义数据
 */
typedef void (CALLBACK *OpenSDK_DataCallBack)(DataType enType, char* const pData, int iLen, void* pUser, const char* szSessionId);

/** 
 *  \brief 设置数据回调
 *  \param[in] szSessionId   会话ID
 *  \param[in] pDataCallBack 回调函数
 *  \param[in] pUser         用户自定义数据，会通过pDataCallBack原样抛出
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetDataCallBack(const char* szSessionId, OpenSDK_DataCallBack pDataCallBack, void* pUser);

/** 
 *  \brief 设置对讲音频数据回调
 *  \param[in] szSessionId   会话ID
 *  \param[in] pDataCallBack 回调函数
 *  \param[in] pUser         用户自定义数据，会通过pDataCallBack原样抛出
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetAudioDataCallBack(const char* szSessionId, OpenSDK_DataCallBack pDataCallBack, void* pUser);

/** 
 *  \brief 设置或者切换清晰度, 根据OpenSDK_Data_GetDevDetailInfo接口返回信息videoQualityInfos来判断是否支持对应的清晰度类型
 *         不支持国标设备
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] iVideoLevel   视频质量，输入范围在0-3之间, 0-流畅; 1-均衡; 2-高清; 3-超清
 *  \return 0表示成功，非0表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetVideoLevel(const char* szDevSerial, const int iChannelNo, const int iVideoLevel);

/** 
 *  \brief 针对摄像头进行预览
           异步接口，返回值只是表示操作成功，不代表播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话ID
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey	 视频加密密钥,如果视频未加密, 可以设置为NULL 
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopRealPlayEx
 *	\sa OpenSDK_SetDataCallBack
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartRealPlayEx(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const int iChannelNo, const char* szSafeKey);

/** 
 *  \brief 针对摄像头进行预览(扩展接口),不适用于国标设备，支持边缘设备预览
           异步接口，返回值只是表示操作成功，不代表播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话ID
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   取流的设备序列号"主设备-子设备"
 *  \param[in] szSuperSerial 主设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥,如果视频未加密, 可以设置为NULL 
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopRealPlayEx
 *	\sa OpenSDK_SetDataCallBack
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartRealPlayExtend(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const char* szSuperSerial, const int iChannelNo, const char* szSafeKey);


/** 
 *  \brief 针对摄像头进行预览, 指定主\子码流
           异步接口，返回值只是表示操作成功，不代表播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话ID
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey	 视频加密密钥,如果视频未加密, 可以设置为NULL 
 *  \param[in] iStreamType	 主子码流 1-主, 2-子, -1-默认码流类型
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopRealPlayEx
 *	\sa OpenSDK_SetDataCallBack
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartPlayWithStreamType(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const int iStreamType);

/** 
 *  \brief 针对摄像头进行预览
           异步接口，返回值只是表示操作成功，不代表播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话ID
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey	 视频加密密钥,如果视频未加密, 可以设置为NULL 
 *  \param[in] szToken	 	 取流token 必须设置
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopRealPlayEx
 *	\sa OpenSDK_SetDataCallBack
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartRealPlayNew(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szToken);

/** 
 *  \brief 停止预览（直播）播放
           异步接口，返回值只是表示操作成功，不代表停止播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_STOP, 表示成功
 *  \param[in] szSessionId 会话ID
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopRealPlayEx
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopRealPlayEx(const char* szSessionId);

/** @} */ // end of play

/** \defgroup searchRecord 查录像接口
 *  @{
 */

/** 
 *  \brief 按录像源进行搜索，支持海外本地录像和国内海外云存储录像搜索
           同步接口
           结果根据消息回调函数msgid判断, 若msgid=INS_RECORD_SEARCH_END, 表示成功, 若msgid= INS_RECORD_SEARCH_FAILED, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号(支持长序列号设备)
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szStartTime   开始时间  时间格式为：yyyy-MM-dd hh:mm:ss
 *  \param[in] szStopTime    停止时间  时间格式为：yyyy-MM-dd hh:mm:ss
 *  \param[in] iRecordType   要搜索的录像源,1 表示本地录像 2 表示云存储录像
 *  \return 0表示成功 非0表示失败
 *  \note
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartSearchExtend(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime,const int iSearchType);

/** 
 *  \brief 云录制录像搜索，支持国内搜索
           同步接口
           结果根据消息回调函数msgid判断, 若msgid=INS_RECORD_SEARCH_END, 表示成功, 若msgid= INS_RECORD_SEARCH_FAILED, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号(支持长序列号设备)
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szStartTime   开始时间  时间格式为：yyyy-MM-dd hh:mm:ss
 *  \param[in] szStopTime    停止时间  时间格式为：yyyy-MM-dd hh:mm:ss
 *  \param[in] iSpaceId      云录像存储的空间id
 *  \return 0表示成功 非0表示失败
 *  \note
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartSearchCloudRecord(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime,const int iSpaceId);

/** 
 *  \brief 本地录像搜索,支持指定录像类型，支持返回录像大小
           同步接口
           结果根据消息回调函数msgid判断, 若msgid=INS_RECORD_SEARCH_END, 表示成功, 若msgid= INS_RECORD_SEARCH_FAILED, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szStartTime   开始时间  时间格式为：yyyy-MM-dd hh:mm:ss 起始结束时间必须为同一天
 *  \param[in] szStopTime    停止时间  时间格式为：yyyy-MM-dd hh:mm:ss 起始结束时间必须为同一天
 *  \param[in] iRecordType   要搜索的录像类型,0 表现所有录像 1 表示定时录像 2 表示事件录像 3 智能-车 4 智能-人行 5 自动浓缩录像 6 定时浓缩录像 7 手动浓缩录像
 *  \return 0表示成功 非0表示失败
 *  \note
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartSearchLocalExtend(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime,const int iSearchType);

/**
 *  \brief 云存储录像搜索,海外使用接口
           同步接口
           结果根据消息回调函数msgid判断, 若msgid=INS_RECORD_SEARCH_END, 表示成功, 若msgid= INS_RECORD_SEARCH_FAILED, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号(支持长序列号设备)
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szStartTime   开始时间   时间格式为：yyyy-MM-dd hh:mm:ss
 *  \param[in] szStopTime    结束时间   时间格式为：yyyy-MM-dd hh:mm:ss
 *  \return 0表示成功 非0表示失败
 *  \note
 *  
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartSearchCouldExtend(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime);

/**
*  \brief 设备录像搜索,国内环境使用接口 增加CVR录像搜索
同步接口
结果根据消息回调函数msgid判断, 若msgid=INS_RECORD_SEARCH_END, 表示成功, 若msgid= INS_RECORD_SEARCH_FAILED, 表示失败
*  \param[in] szSessionId   会话Id
*  \param[in] szDevSerial   设备序列号(支持长序列号设备)
*  \param[in] iChannelNo    设备通道号
*  \param[in] szStartTime   开始时间   时间格式为：yyyy-MM-dd hh:mm:ss
*  \param[in] szStopTime    结束时间   时间格式为：yyyy-MM-dd hh:mm:ss
*  \return 0表示成功 非0表示失败
*  \note
*
*/

OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartSearchLocalDomestic(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime);

/**
* \brief 获取本地录像封面
* \param[in] szSessionId 会话Id
* \param[in] szDevSerial 设备序列号
* \param[in] iChannelNo 通道号
* \param[in] szStartTime 开始时间
* \param[in] szStopTime 结束时间
* \param[in] seq 文件索引
* \return 0表示成功，-1表示失败
* \sa
*/
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_GetLocalRecordCover(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime, const int seq);

/**
* \brief 初始化本地录像封面链接
* \param[in] szSessionId 会话Id
* \param[in] szDevSerial 设备序列号
* \param[in] iChannelNo 通道号
* \return 0表示成功，-1表示失败
* \sa
*/
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_LocalRecordCoverConnect(const char* szSessionId, const char* szDevSerial, const int iChannelNo);

/**
* \brief 断开本地录像封面链接
* \param[in] szSessionId 会话Id
* \return 0表示成功，-1表示失败
* \sa
*/
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_LocalRecordCoverDisconnect(const char* szSessionId);

/**
* \brief 获取本地录像封面
* \param[in] szSessionId 会话Id
* \param[in] szDevSerial 设备序列号
* \param[in] iChannelNo 通道号
* \param[in] szStartTime 开始时间
* \param[in] szStopTime 结束时间
* \param[in] seq 文件索引
* \return 0表示成功，-1表示失败
* \sa
*/
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_GetLocalRecordCoverEx(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szStopTime, const int seq);


/** @} */ // end of searchRecord

/** \defgroup playback 录像回放接口
 *  @{
 */

/** 
*  \brief 开始回放
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败,可调用此接口下载本地录像,
          只取流状态下不支持连续跨片段取流
 *  \param[in] szSessionId   会话Id
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopPlayBack()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartPlayBackEx(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime);


/** 
*  \brief 开始回放，不适用于国标设备
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败,可调用此接口下载本地录像,
          只取流状态下不支持连续跨片段取流
 *  \param[in] szSessionId   会话Id
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   取流设备序列号"主设备-子设备"
 *  \param[in] szSuperSerial 主设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopPlayBackEx()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartPlayBackExtend(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const char* szSuperSerial,const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime);

/** 
*  \brief 开始回放，免查询，不适用于国标设备
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败,可调用此接口下载本地录像,
          只取流状态下不支持连续跨片段取流
 *  \param[in] szSessionId   会话Id
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   取流设备序列号"主设备-子设备"
 *  \param[in] szSuperSerial 主设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] stVideoInfo   录像信息
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopPlayBackEx()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartPlayBackNoCheck(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const char* szSuperSerial,const int iChannelNo, const char* szSafeKey,const EZ_RECORD_INFO& stRecInfo);

/** 
*  \brief 开始回放，免查询 使用于多片段连续播放 
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败,可调用此接口下载本地录像,
          只取流状态下不支持连续跨片段取流
 *  \param[in] szSessionId   会话Id
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevSerial   取流设备序列号
 *  \param[in] szSuperSerial 主设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] stVideoInfo   录像信息
 *  \param[in] szToken	 	 取流token 必须设置
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopPlayBackEx()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartPlayBackNew(const char* szSessionId, HWND hPlayWnd, const char* szDevSerial, const char* szSuperSerial,const int iChannelNo, const char* szSafeKey,const EZ_RECORD_INFO& stRecInfo, const char* szToken);

/** 
 *  \brief 恢复回放
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_PlayBackPause()
 *  \note
 *  该接口只能在OpenSDK_StartPlayBack()调用之后才能调用
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_PlayBackResume(const char* szSessionId);

/** 
 *  \brief 暂停回放
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_PlayBackResume()
 *  \note 
 *  该接口只能在OpenSDK_StartPlayBack() 调用之后才能调用
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_PlayBackPause(const char* szSessionId);


/** 
 *  \brief 回放seek
 *  \param[in] szSessionId 会话Id
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_PlayBackSeek()
 *  \note 
 *  该接口只能在OpenSDK_StartPlayBack() 调用之后才能调用
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_PlayBackSeek(const char* szSessionId,const char* szStartTime, const char* szStopTime);


/** 
 *  \brief 回放倍速设置,支持只取流状态下设置取流倍速.(倍速能力集由上级判断)
 *  \param[in] szSessionId 会话Id
 *  \param[in] iScale 回放倍速,1(正常倍速),2(2倍速),3(1/2倍速),4(4倍速),5(1/4倍速),6(8倍速),7(1/8倍速),8(16倍速),9(1/16倍速),10(32倍速)
       云存储回放只支持1,2,4,8,16,32倍速切换 #EZOPENSDK_PLAYBACK_SPEED_TYPE
 *  \param[in] szOsdTime,OSD 时间,当前播放时间,云存储回放切换倍速时需要传入该参数(只取流状态下,外部需要获取播放库的OSD时间),暂不支持跨片段设置取流倍速功能。
 *  \param[in] mode, 目前仅用于云存储倍速回放的时候,用户指定是否抽帧, 0表示4倍速全帧，8倍速以上抽帧, 1 表示全部抽帧  2表示全帧
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_SetPlayBackScale()
 *  \note
 *  该接口只能在OpenSDK_StartPlayBackEx()调用之后才能调用,在只取流状态下,外部实现倍速回放需要设置播放速度（匹配取流速度）
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetPlayBackScale(const char* szSessionId,int iScale,const char* szOsdTime = NULL, int mode = 0);

/** 
 *  \brief 停止回放
		   异步接口，返回值只是表示操作成功，不代表停止播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_STOP, 表示成功
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_StartPlayBack()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopPlayBackEx(const char* szSessionId);

/** @} */ // end of playback

/** \defgroup 录像下载接口
 *  @{
 */

/** 
*  \brief 云存储录像下载 不支持国标设备
          异步接口,本地录像请调用回放接口进行下载,不支持跨片段连续下载
          如果接口返回非0, 表示失败.
          如果接口返回0, 结果根据消息回调函数msgid判断, 若msgid= INS_DOWNLOAD_STOP, 表示下载成功，若msgid= INS_DOWNLOAD_EXCEPTION, 表示下载异常
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StartDownloadCloudFile()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartDownloadCloudFile(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime);

/** 
 *  \brief 停止云存储录像下载，不支持国标设备
		   异步接口,如果接口返回0, 结果根据消息回调函数msgid判断, 若msgid = INS_USER_STOP_DOWNLOAD, 表示成功
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_StopDownloadCloudFile()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopDownloadCloudFile(const char* szSessionId);


/** 
*  \brief 开始下载(设备录像流媒体下载)，不支持国标设备
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopDownload()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartDownload(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime);

/** 
 *  \brief 停止设备录像下载下载，不支持国标设备
		   异步接口，返回值只是表示操作成功，不代表停止播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_STOP, 表示成功
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_StartDownload()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopDownload(const char* szSessionId);


/** @} */ // end of download

/** 
 *  \brief 获取单个设备p2p相关信息
 *  \param[in] szDeviceSerial 设备序列号
 *  \param[out] pBuf          设备信息的JSON字符串
 *  \param[out] iLength       获取到的数据大小
 *  \return 0表示成功，非0表示失败
 *
 *  \note 
 *  设备信息的JSON格式如下
 *  \code {.json}
 *	{
 *      "result": 
 8      {
 *          "data": 
 *          {
 *              "serverInfos": [
 *              {
 *                  "ip": "61.153.2.189",
 *                  "port": 6000
 *              }],
 *              "defaultKey": "YTIxekxtUmxkbWxqWlE9PS4xMDQuODQzNGFjNTkwMTUzZGQxODQ5YTQ5ZGUxNmNjMDUxYjAyNTg5MTI3OGYxNTE5MjJjNTU2YjAyODBhNmY3YWI3MC4xNTMyNDEzNzI5NDk0Lnc4Z0JHa3dlUER4QnlCa2xqbGg4cElBZU9pSUt4dHQvZTlncEtVNGNGZ0t6ZjdGK05JWXZWblFiSVB5bldGbTd4NThpS0YveUpjM2Q3UUM4NmNzaXdnPT0=",
 *              "defaultKeyVer": "104",
 *              "ezDeviceCapability":"{\"V3Sec\":\"1\",\"DirectPlayback_EndFlag\":\"1\",\"support_smart_wifi\":\"0\",\"ptz_left_right\":\"0\",\"V3Download\":\"1\",\"sc\":\"1\",\"V3Playback\":\"1\",\"support_wifi_userId\":\"0\",\"support_hiddns_config\":\"1\",\"ptz_top_bottom\":\"0\",\"V3\":\"0\",\"support_channel_number\":1,\"V3Talk\":\"0\"}"
 *          },
 *          "code": "200",
 *          "msg": "操作成功!"
 *      }
 *	}
 *  \endcode
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetP2PDeviceInfo(const char* szAccessToken, const char* szDeviceSerial, void** pBuf, int* iLength);

/** 

/** 
*  \brief p2p下载
 *         异步接口，返回值只是表示操作成功,
 *         如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *         调用这个接口之前可通过OpenSDK_Data_GetP2PDeviceInfo返回的 ezDeviceCapability的V3Download这个字段来判断是否支持p2p下载
 *  \param[in] iDownloadType 下载类型
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间 
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StartP2PDownload()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartP2PDownload(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime);

/** 
 *  \brief 停止下载
           异步接口，返回值只是表示操作成功
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopP2PDownload()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopP2PDownload(const char* szSessionId);

/** 
*  \brief 开始下载(设备录像下载)，不支持国标设备
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopDownloadEx()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartDownloadEx(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime);

/** 
*  \brief 开始下载(设备录像流媒体下载)，不支持国标设备
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szSafeKey     视频加密密钥
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \param[in] szToken	 	 取流token 必须设置
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopDownload()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartDownloadNew(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szSafeKey, const char* szStartTime, const char* szStopTime, const char* szToken);

/** 
 *  \brief 停止设备录像下载下载，不支持国标设备
		   异步接口，返回值只是表示操作成功，不代表停止播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_STOP, 表示成功
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_StartDownloadEx()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopDownloadEx(const char* szSessionId);

/** \defgroup info 视频信息获取
 *  @{
 */

/** 
 *  \brief 获取OSD时间
 *  \param[in]  szSessionId 会话Id
 *  \param[out] pTime       OSD时间，格式见 #STREAM_TIME 
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_GetOSDTime(const char* szSessionId, STREAM_TIME *pTime);

/** @} */ // end of info

/** \defgroup sound 音频相关接口
 *  @{
 */

/** 
 *  \brief 打开声音，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_CloseSound()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_OpenSound(const char* szSessionId);

/** 
 *  \brief 关闭声音，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_OpenSound()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_CloseSound(const char* szSessionId);

/**
 *  \brief 获取视频播放音量，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \return 小于0表示失败，其他值表示音量大小(0-100之间)
 *  \sa OpenSDK_SetVolume()
 */
OPENSDK_API unsigned short CALLBACK OpenSDK_GetVolume(const char* szSessionId);

/** 
 *  \brief 设置视频播放音量，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \param[in] uVolume     音量大小，0-100之间
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_GetVolume()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetVolume(const char* szSessionId, unsigned short uVolume);

/** @} */ // end of sound

/** \defgroup voice_talk 语音对讲
 *  @{
 */

/** 
 *  \brief  语音对讲获取声卡信息
 *  \param[in] szSessionId 会话Id
 *  \param[in] pBuf 声卡列表的JSON字符串
 *  \param[in] iLength 声卡列表的JSON字符串长度
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_GetVoiceTalkCapture()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_GetVoiceTalkCapture(const char* szSessionId, void** pBuf, int* iLength);

/** 
 *  \brief  语音对讲获取播放器信息
 *  \param[in] szSessionId 会话Id
 *  \param[in] pBuf 声卡列表的JSON字符串
 *  \param[in] iLength 声卡列表的JSON字符串长度
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_GetVoiceTalkPlay()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_GetVoiceTalkPlay(const char* szSessionId, void** pBuf, int* iLength);

/** 
 *  \brief  语音对讲选择声卡
 *  \param[in] szSessionId 会话Id
 *  \param[in] iCapture 声卡编号
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_SetVoiceTalkCapture()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetVoiceTalkCapture(const char* szSessionId,int iCapture);

/** 
 *  \brief  语音对讲选择播放器
 *  \param[in] szSessionId 会话Id
 *  \param[in] iCapture 声卡编号
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_SetVoiceTalkPlay()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetVoiceTalkPlay(const char* szSessionId,int iPlay);

/** 
 *  \brief 开启语音对讲, 不支持多个设备同时对接，不支持国标设备
           异步接口，返回值只是表示操作成功，不代表对讲开启成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_VOICETALK_START, 表示成功, 若msgid=INS_VOICETALK_STOP, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_StopVoiceTalk()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartVoiceTalkEx(const char* szSessionId, const char* szDevSerial, const int iChannelNo);

/** 
 *  \brief 开启语音对讲, 不支持多个设备同时对接
           异步接口，返回值只是表示操作成功，不代表对讲开启成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_VOICETALK_START, 表示成功, 若msgid=INS_VOICETALK_STOP, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] iChannelNo    设备通道号
 *  \param[in] szToken	 	 取流token 必须设置,根据设备对讲和通道对讲获取不同权限的token
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_StopVoiceTalk()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartVoiceTalkNew(const char* szSessionId, const char* szDevSerial, const int iChannelNo, const char* szToken);

/** 
 *  \brief 结束语音对讲，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_StartVoiceTalkEx()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopVoiceTalk(const char* szSessionId);

/**
 *  \brief 获取对讲播放音量，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \return 小于0表示失败，其他值表示音量大小(0-100之间)
 *  \sa OpenSDK_SetVolume()
 */
OPENSDK_API unsigned short CALLBACK OpenSDK_GetVoiceTalkExVolume(const char* szSessionId);

/** 
 *  \brief 设置对讲播放音量，不支持国标设备
 *  \param[in] szSessionId 会话Id
 *  \param[in] uVolume     音量大小，0-100之间
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_GetVolume()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetVoiceTalkExVolume(const char* szSessionId, unsigned short uVolume);

/** @} */ // end of voice_talk

/** \defgroup audio_transport 音频传输
 *  @{
 */

/** 
 *  \brief 开启音频传输
           异步接口，返回值只是表示操作成功，不代表对讲开启成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_VOICETALK_START, 表示成功, 若msgid=INS_VOICETALK_STOP, 表示失败
 *  \param[in] szSessionId     会话Id
 *  \param[in] szDevSerial     设备序列号
 *  \param[in] iChannelNo      设备通道号
 *  \param[out] pAudioCodeType 设备音频编码类型   #EZOPENSDK_AUDIO_CODE
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_StopAudioTransport()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartAudioTransport(const char* szSessionId, const char* szDevSerial, const int iChannelNo, int* pAudioCodeType);


/** 
 *  \brief 结束音频传输
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_StartAudioTransport()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopAudioTransport(const char* szSessionId);

/** 
 *  \brief 发送音频数据
 *  \param[in] szSessionId   会话ID
 *  \param[in] pAudioBuf     对讲数据
 *  \param[in] iDataSize     对讲数据大小
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SendAudioData(const char* szSessionId, void* pAudioBuf, unsigned int iDataSize);

/** @} */ // end of audio_transport

/** \defgroup midpage 中间页(账号登录接口，WIN64不支持)
 *  @{
 */

/** 
 *  \brief  设置登入传入参数（不再支持，不建议使用该接口） 
 *  \param[in] iKey        传入参数类型, LOGIN_AREAID 或者 LOGIN_VER
 *  \param[in] szValue     Key对应参数参数内容
 *  \sa 
 */
OPENSDK_API void CALLBACK OpenSDK_SetLoginParams(LoginParamKey iKey, const char* szValue);

/** 
 *  \brief  获取登入后返回参数（不再支持，不建议使用该接口） 
 *  \param[in] iKey        传入参数类型, LOGIN_AREADOMAIN 或者 LOGIN_TOKEN_EXPIRETIME 或者 LOGIN_ACCESS_TOKEN
 *  \param[in] szValue     Key对应参数参数内容
 *  \sa OpenSDK_Mid_Login之后调用
 */
OPENSDK_API const char* CALLBACK OpenSDK_GetLoginResponseParams(LoginParamKey iKey);


/** 
 *  \brief  注销接口, 同OpenSDK_Mid_Login接口配对使用，不支持国标设备（不再支持，不建议使用该接口） 
 *  \sa OpenSDK_Mid_Login之后调用
 */
OPENSDK_API void CALLBACK OpenSDK_Logout();

/** 
 *  \brief 登陆接口，不支持国标设备（不再支持，不建议使用该接口） 
 *  \param[out] szAccessToken   认证Token
 *  \param[out] iAccessTokenLen 认证Token长度
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Data_Free()，用于释放szAccessToken申请的内存
 */
OPENSDK_API int CALLBACK OpenSDK_Mid_Login( char** szAccessToken, int* iAccessTokenLen );

/** @} */ // end of midpage

/** \defgroup data 数据接口
 *  @{
 */

/** 
 *  \brief 获取摄像头列表
 *  \param[in]  iPageStart    分页起始页，从0开始
 *  \param[in]  iPageSize     分页大小, 限制最大1000,默认100, 建议不超过100
 *  \param[out] pBuf          摄像头列表的JSON字符串
 *  \param[out] iLength       获取到的数据大小
 *  \return 0表示成功，-1表示失败 
 *
 *  \note 
 *  摄像头列表的JSON格式如下
 *  \code {.json}
 *{
 *  "page" : {
 *     "page" : 0,                                             //页码
 *     "size" : 20,                                            //页大小
 *     "total" : 1                                             //账号下所有设备数量
 *  },
 *  "data" : [
 *     {
 *        "addTime" : 1512652274000.0,                          //设备被用户添加时间，精确到毫秒
 *        "alarmSoundMode" : 0,                                 //告警声音模式
 *  	  "cameraNum" : 2,                                      //视频数
 *        "category" : "UNKNOWN",                               //设备大类
 *        "defence" : 0,                                        //布A1设备布撤防状态，0:睡眠 8:在家 16:外出, 非A1设备，0-撤防 1-布防
 *        "detectorNum" : 0,                                    //探测器数
 *        "detectorInfo" : null,                                //探测器信息
 *        "deviceCover" : "https://i.ys7.com/image/DVR/1.jpeg", //设备封面
 *        "deviceName" : "DS-7104N-SN/C(129103497)",            //设备名称
 *        "deviceSerial" : "129103497",                         //设备序列号
 *        "deviceType" : "DS-7104N-SN/C",                       //设备类型
 *        "deviceVersion" : "V3.0.21 build 170417",             //设备版本号
 *        "isEncrypt" : 0,                                      //是否加密，0：不加密，1：加密
 *        "status" : 2,                                         //在线状态，1-在线，2-不在线
 *        "supportExtShort" : "1|1|1|1|0|0|0|1|1|0|0|-1|0...",  //设备能力集
 *        "cameraInfo" : [                                      //视频信息
 *           {
 *              "cameraCover" : "https://i.ys7.com/...",        //通道封面
 *              "cameraName" : "视频1@DS-7104N-SN/C(129103497)",//视频名camera
 *              "cameraNo" : 1,                                 //视频号
 *              "deviceSerial" : "129103497",                   //设备序列号
 *              "isShared" : "0",                               //分享状态，1：分享所有者，0：未分享，2：分享接受者（表示此摄像头是别人分享给我的）
 *              "videoLevel" : 0,                               //视频质量，0-流畅，1-均衡，2-高清，3-超清
 *              "videoQualityInfos" : [
 *                 {
 *                    "streamType" : 2,
 *                    "videoLevel" : 0,
 *                    "videoQualityName" : "流畅"
 *                 },
 *                 {
 *                    "streamType" : 1,
 *                    "videoLevel" : 2,
 *                    "videoQualityName" : "高清"
 *                 }
 *              ]
 *           },
 *           {
 *              "cameraCover" : "https://i.ys7.com/assets/imgs/public/homeDevice.jpeg",
 *              "cameraName" : "视频2@DS-7104N-SN/C(129103497)",
 *              "cameraNo" : 2,
 *              "deviceSerial" : "129103497",
 *              "isShared" : "0",
 *              "videoLevel" : 0,
 *              "videoQualityInfos" : [
 *                 {
 *                    "streamType" : 2,
 *                    "videoLevel" : 0,
 *                    "videoQualityName" : "流畅"
 *                 },
 *                 {
 *                    "streamType" : 1,
 *                    "videoLevel" : 2,
 *                    "videoQualityName" : "高清"
 *                 }
 *              ]
 *           }
 *        ]
 *     }
 *  ]
 *}
*  \endcode
*/
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetDevListEx(int iPageStart, int iPageSize, void** pBuf, int* iLength);

/** 
 *  \brief 获取他人分享的摄像头列表
 *  \param[in]  iPageStart    分页起始页，从0开始
 *  \param[in]  iPageSize     分页大小, 限制最大1000,默认100, 建议不超过100
 *  \param[out] pBuf          摄像头列表的JSON字符串
 *  \param[out] iLength       获取到的数据大小
 *  \return 0表示成功，-1表示失败 
 *
 *  \note 
 *  摄像头列表的JSON格式如下
 *  \code {.json}
 *{
 *  "page" : {
 *     "page" : 0,                                             //页码
 *     "size" : 20,                                            //页大小
 *     "total" : 1                                             //账号下所有设备数量
 *  },
 *  "data" : [
 *     {
 *        "addTime" : 1512652274000.0,                          //设备被用户添加时间，精确到毫秒
 *        "alarmSoundMode" : 0,                                 //告警声音模式
 *  	  "cameraNum" : 2,                                      //视频数
 *        "category" : "UNKNOWN",                               //设备大类
 *        "defence" : 0,                                        //布A1设备布撤防状态，0:睡眠 8:在家 16:外出, 非A1设备，0-撤防 1-布防
 *        "detectorNum" : 0,                                    //探测器数
 *        "detectorInfo" : null,                                //探测器信息
 *        "deviceCover" : "https://i.ys7.com/image/DVR/1.jpeg", //设备封面
 *        "deviceName" : "DS-7104N-SN/C(129103497)",            //设备名称
 *        "deviceSerial" : "129103497",                         //设备序列号
 *        "deviceType" : "DS-7104N-SN/C",                       //设备类型
 *        "deviceVersion" : "V3.0.21 build 170417",             //设备版本号
 *        "isEncrypt" : 0,                                      //是否加密，0：不加密，1：加密
 *        "status" : 2,                                         //在线状态，1-在线，2-不在线
 *        "supportExtShort" : "1|1|1|1|0|0|0|1|1|0|0|-1|0...",  //设备能力集
 *        "cameraInfo" : [                                      //视频信息
 *           {
 *              "cameraCover" : "https://i.ys7.com/...",        //通道封面
 *              "cameraName" : "视频1@DS-7104N-SN/C(129103497)",//视频名camera
 *              "cameraNo" : 1,                                 //视频号
 *              "deviceSerial" : "129103497",                   //设备序列号
 *              "isShared" : "0",                               //分享状态，1：分享所有者，0：未分享，2：分享接受者（表示此摄像头是别人分享给我的）
 *              "videoLevel" : 0,                               //视频质量，0-流畅，1-均衡，2-高清，3-超清
                "permission": 3,                                //客户端用于分享的设备权限控制字段 ,  authority=(REAL(1), REPLAY(1 << 1), ALARM(1 << 2), TALK(1 << 3), VIDEO_QUALITY(1 << 4), CAPTURE(1 << 5), VIDEO(1 << 6), SHARE(1 << 7), PTZ(1 << 8), LEAVE(1 << 9), ALL(-1)); 如果authority&permission = permission:表示可授予该权限
 *              "videoQualityInfos" : [
 *                 {
 *                    "streamType" : 2,
 *                    "videoLevel" : 0,
 *                    "videoQualityName" : "流畅"
 *                 },
 *                 {
 *                    "streamType" : 1,
 *                    "videoLevel" : 2,
 *                    "videoQualityName" : "高清"
 *                 }
 *              ]
 *           },
 *           {
 *              "cameraCover" : "https://i.ys7.com/assets/imgs/public/homeDevice.jpeg",
 *              "cameraName" : "视频2@DS-7104N-SN/C(129103497)",
 *              "cameraNo" : 2,
 *              "deviceSerial" : "129103497",
 *              "isShared" : "0",
 *              "videoLevel" : 0,
 *              "videoQualityInfos" : [
 *                 {
 *                    "streamType" : 2,
 *                    "videoLevel" : 0,
 *                    "videoQualityName" : "流畅"
 *                 },
 *                 {
 *                    "streamType" : 1,
 *                    "videoLevel" : 2,
 *                    "videoQualityName" : "高清"
 *                 }
 *              ]
 *           }
 *        ]
 *     }
 *  ]
 *}
*  \endcode
*/
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetSharedDevList(int iPageStart, int iPageSize, void** pBuf, int* iLength);

/** 
 *  \brief 获取设备详细信息, 包括预览能力级, PTZ, 对讲能力级等
 *  \param[in]  szDevSerial     设备序列号
 *  \param[in]  iChannelNo      设备通道号
 *  \param[in]  bUpdate         是否重新获取设备详细信息, true:重新从萤石平台拿数据, 否则拿缓存信息. true的时机为用户重新登录或者需要刷新设备信息.
 *	\param[out] pDevDetailInfo	监控点信息，需要调用OpenSDK_FreeData接口释放
 *	\param[out] iLength			监控点信息的长度
 *
 *  \note 
 *  JSON格式如下：
 *  \code {.json}
 *  {
 *		"devSerial":"426931084",	        // 设备序列号
 *		"channelNo":1,				        // 通道号
 *		"videoLevel":2,				        // 视频质量 2-高清 1-标清 0-流畅
 *		"type":1,					        // 1 网络录像机 2 模拟录像机
 *		"capability":"2-2-1",		        // 预览能力级, 2-2-1 三挡分别对应流畅,均衡,高清, 1:主码流, 2:子码流, 0:不支持
 *		"isEncrypt":0,				        // 是否开启安全模式:1-开启,0-关闭, 即视频图像是否加密
 *      "support_talk":1,                   // 是否支持对讲:1-支持, 0-不支持
 *      "support_ptz":0,                    // 是否支持云台控制:1-支持, 0-不支持
 *      "ptz_top_bottom":1,                 // 是否支持云台上下:1-支持, 0-不支持
 *      "ptz_left_right":1                  // 是否支持云台左右:1-支持, 0-不支持
 *      "ptz_preset":1,                     // 是否支持云台预置点:1-支持, 0-不支持
 *      "support_quickplay_way":1           // 远程回放方式（4/8/16倍速）,0-不支持, 1-远程抽帧快速回放, 2-流控
 *      "download":1,                       // 是否支持流媒体录像下载:1-支持, 0-不支持
 *      "shared": "0",                      // 分享类型
 *      "supportExtShort" : "1|1|1|1|0|0|0|1|1|0|0|-1|0...",  //设备能力集
 *      "support_seek_playback":1,						// 是否支持设备录像播放seek操作：1-支持，0-不支持 
 *      "support_device_playback_small_speed":1,    // 是否支持2倍数和0.5倍数回放功能, 1-支持, 0-不支持
 *      "videoQualityInfos": [              // 设备支持的预览能力级详情
 *          {
 *              "videoQualityName":"流畅",  // 清晰度名称
 *              "videoLevel":0,             // 清晰度数值
 *              "streamType":2              // 主子码流类型
 *          },
 *          {
 *              "videoQualityName":"均衡",
 *              "videoLevel":1,
 *              "streamType":2
 *          },
 *          {
 *              "videoQualityName":"高清",
 *              "videoLevel":2,
 *              "streamType":1
 *          }
 *      ]
 *	}
 *  \endcode
 *  \return 0表示成功，其他值表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetDevDetailInfo(const char* szDevSerial, const int iChannelNo, const bool bUpdate, void** pDevDetailInfo, int* iLength);

/** 
 *  \brief 获取单个设备信息,不支持国标设备
 *  \param[in] szAccessToken 设备序列号
 *  \param[in] szDeviceSerial 设备序列号
 *  \param[out] pBuf          设备信息的JSON字符串
 *  \param[out] iLength       获取到的数据大小
 *  \return 0表示成功，非0表示失败
 *
 *  \note 
 *  设备信息的JSON格式如下
 *  \code {.json}
 *	{
 *		"result": {
 *			"data": [
 *				{
 *					"deviceId": "",						// 设备在平台的唯一标识
 *					"deviceSerial": "421877673",		// 设备序列号
 *					"cameraId": "",						// 监控点ID
 *					"cameraNo": 1,						// 设备通道号
 *					"cameraName": "",					// 监控点名称
 *					"status": 1,						// 监控点在线状态, 0-离线; 1-在线
 *					"isShared": "0",					// 设备分享状态, 0-未共享;1-共享所有者;2-共享接受者
 *					"picUrl": "",						// 监控点图片url
 *					"isEncrypt": 0						// 加密状态, 0-未加密;1-加密
 *					"defence" : 1,						// 布撤防状态, 0-未布防; 1-布防
 *					"videoLevel" : 0					// 视频清晰度, 0-流畅; 1-均衡; 2-高清
 *				}
 *			],
 *			"code": "200",
 *			"msg": "操作成功!"
 *		}
 *	}
 *  \endcode
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetDeviceInfo(const char* szAccessToken, const char* szDeviceSerial, void** pBuf, int* iLength);

/** 
 *  \brief 获取告警列表,不支持国标设备  不支持64位不支持小权限
 *  \param[in]  szAccessToken 认证Token
 *  \param[in]  szDevSerial   设备序列号
 *  \param[in]  iChannelNo    通道号
 *  \param[in]  szStartTime   开始时间
 *  \param[in]  szEndTime     结束时间
 *  \param[in]  iAlarmType    告警类型，参见 #AlarmType
 *  \param[in]  iStatus       告警状态，0表示未读，1表示已读，2表示所有
 *  \param[in]  iPageStart    分页起始页，从0开始
 *  \param[in]  iPageSize     分页大小
 *  \param[out] pBuf		  告警信息列表
 *  \param[out] iLength       告警信息列表长度
 *  \return 0表示成功，-1表示失败
 *  
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 *
 *  \note 
 *  告警信息列表JSON格式如下：
 *  \code {.json}
 {
     "result": {
         "page": {                                          // 分页信息
             "total": 694,                                  // 告警总数量
             "page": 0,                                     // 分页号
             "size": 5                                      // 分页大小
         },
         "data": [                                          // 告警信息
             {
             "alarmId": "96610038558312441",                // 消息ID
             "alarmName": "test设备",                       // 告警源名称
             "alarmType": 10000,                            // 告警类型
             "alarmStart": "2016-08-30 15:33:18",           // 告警开始时间
             "cameraNo": 1,                                 // 通道号 
             "isEncrypt": 0,                                // 是否加密，0-不加密 1-加密
             "isChecked": 0,                                // 是否已读，0-未读 1-已读
             "recState":1,                                  // 存储类型,0-无存储，1-萤石，2-百度,4-sd卡存储，5-萤石和sd，6-百度和sd
             "preTime": 10,                                 // 预录时间，单位秒
             "delayTime": 30,                               // 延迟录像时间，单位秒
             "deviceSerial": "427734168",                   // 设备序列号
             "alarmPicUrl": "https://test.ys7.com/Ezviz_14?isEncrypted=0&isCloudStored=0", //告警图片地址, 如果加密,调用OpenSDK_DecryptPicture解析
             "relationAlarms": null,                        // 关联的告警消息
             "customerType": null,                          // 透传设备参数类型
             "customerInfo": null                           // 透传设备参数内容
             },
             ...
         ],
         "code": "200",
         "msg": "操作成功!"
     }
 }
 *  \endcode
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetAlarmListEx(const char* szDevSerial, const int iChannelNo, const char* szStartTime, const char* szEndTime, AlarmType iAlarmType, int iStatus, int iPageStart, int iPageSize, void** pBuf, int* iLength);

/** 
 *  \brief 解密告警图片(建议加密的图片才调用，非加密图片直接下载，可以通过url里面isEncrypted=1来区分),,不支持国标设备 不支持64位不支持小权限
 *  \param[in] szAccessToken 认证Token
 *  \param[in] szPicURL  图片URL,https://wuhancloudpictest.ys7.com:8083/...?isEncrypted=1&isCloudStored=0
 *  \param[in] szSerail  告警图片对应的设备序列号
 *  \param[in] szSafeKey 解密密钥，默认是设备验证码
 *  \param[out] pPicBuf	 解密后图片内容（需要调用OpenSDK_Data_Free释放内存）
 *	\param[out] iPicLen	 pPicBuf的长度
 *  \return 0表示成功，-1表示失败, 失败时调用GetLastErrorCode(), 考虑错误: OPEN_SDK_PERMANENT_KEY_INVALID,OPEN_SDK_PIC_CONTENT_INVALID
 */
OPENSDK_API int CALLBACK OpenSDK_DecryptPicture(const char* szAccessToken, const char* szPicURL, const char* szSerail, const char* szSafeKey, void** pPicBuf, int* iPicLen);

/** 
 *  \brief 设置告警已读,不支持国标设备 不支持64位不支持小权限
 *  \param[in] szAlarmId 告警ID
 *  \return 0表示成功，非0表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_SetAlarmRead(const char* szAccessToken, const char* szAlarmId);

/** 
 *  \brief 销毁SDK分配的内存
 *  \param[in] pBuf SDK分配的内存
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_Free(void* pBuf);

/** @} */ // end of data

/** \defgroup util_tool 通用工具接口
*  @{
*/

/** 
 *  \brief Http请求接口 只支持post接口 后续不再维护，建议不再使用
 *  \param[in] szUri			请求地址
 *  \param[in] szHeaderParam	头部参数
 *  \param[in] szBody			Body数据
 *  \param[in] pBuf				返回报文的内容
 *  \param[in] iLength			返回报文的长度
 *  \return 0表示成功，其他值表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_HttpSendWithWait(const char* szUri, const char* szHeaderParam, const char* szBody, char** pBuf, int* iLength);

/** 
 *  \brief Http请求接口 只支持post接口 后续不再维护，建议不再使用
 *  \param[in] szUri			请求地址
 *  \param[in] szHeaderParam	头部参数
 *  \param[in] szBody			Body数据
 *  \param[in] handers			例如：contentType信息 application/json 输入空为默认：application/x-www-from-urlencoded 格式 "a=aaaa;b=bbbb" 没有就传空
 *  \param[in] pBuf				返回报文的内容
 *  \param[in] iLength			返回报文的长度
 *  \return 0表示成功，其他值表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_HttpSendWithWaitEx(const char* szUri, const char* szHeaderParam, const char* szBody, const char* handers, char** pBuf, int* iLength);

/** @} */ // end of util_tool


/** \defgroup push 消息推送 4200客户端使用 不支持64位
 *  
 * @{
 */
/** 
 *  \brief 告警消息推送回调函数
 *  \param szDesc	   推送描述信息
 *  \param szContent   推送内容
 *	\param szDetail	   完整的推送信息
 *  \param pUser       用户自定义数据
 *  \sa OpenSDK_Push_SetAlarmCallBack()
 *	\sa OpenSDK_Push_SetDeviceStatusCallBack()
 *  \sa OpenSDK_Push_SetTransparentChannelCallBack()
 *  \note 
 *  告警推送内容(szContent) JSON格式如下：
 *  \code {.json}
 *	{
 *		"msgType": "1",							// 消息类型
 *		"alarmTime": "2012-12-25 15:25:42",		// 告警时间
 *		"deviceSeril": "421877673",			    // 设备序列号
 *		"channelID": "1",						// 通道ID
 *		"alarmType": "11",						// 告警类型
 *		"picUrl": "http://www.openys7.com/",	// 图片短地址URL
 *		"videoUrl": "http://www.openys7.com/",// 视频URL
 *		"custominfo":"sn=507669173^cn=30",		// 自定义内容，涉及告警防区
 *		"custominfoType":"CS-A1-32W_01",	    // 自定义内容的类型
 *      "alarmPicUrl":""                        // 图片URL,有效期1天, 通过调用OpenSDK_DecryptPicture接口来解密
 *      "isEncrypt":1                           // 图片是否加密, 1:加密, 0:非加密
 *	}
 *  \endcode
 *  \note 
 *  消息推送内容JSON格式如下：
 *  \code {.json}
 *	{
 *		"msgType": "3",							// 消息类型, 设备状态触发为3
 *		"devMsgType": "1",						// 推送类型号,下线状态为1
 *		"msgTime": "2016-01-27 16:09:39",		// 消息的时间
 *		"deviceSeril": "421877673",				// 设备序列号
 *		"channelID": "1"						// 通道ID
 *	}
 *  \endcode
 *  \note 
 *  通明通道消息推送内容JSON格式如下：
 *  \code {.json}
 *	{
 *		"msgType": "com.openys7.open.transparent",	// 消息类型，透传数据类型
 *		"msgSeq": "123456789",							// 消息ID
 *		"createDate": 1456070400000.0,					// 消息时间, 时间戳(日期)
 *		"data": "hello",								// 消息内容，即透传的数据
 *		"sn": "421877673",								// 设备序列号
 *	}
 *  \endcode
 */

//该接口4200客户端使用 不支持64位不支持小权限

typedef void (CALLBACK *OpenSDK_Push_MessageHandler)(const char* szDesc, const char* szContent,  const char* szDetail, void* pUser);

//该接口4200客户端使用 不支持64位不支持小权限
typedef void (CALLBACK *devSDK_EventHandler)(int nEventType, void* pUser);
/** 
 *  \brief 设置告警推送回调,该接口4200客户端使用 不支持国标设备  不支持64位不支持小权限
 *  \param[in] pHandle			告警推送函数
 *  \param[in] pUser			用户自定义数据，回调函数会原样抛出
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Push_AlarmHandler
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Push_SetAlarmCallBack(OpenSDK_Push_MessageHandler pHandle, void* pUser);

/** 
 *  \brief 设置设备下线状态推送,该接口4200客户端使用 不支持国标设备  不支持64位不支持小权限
 *  \param[in] pHandle			设备状态推送函数
 *  \param[in] pUser			用户自定义数据，回调函数会原样抛出
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Push_MessageHandler
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Push_SetDeviceStatusCallBack(OpenSDK_Push_MessageHandler pHandle, void* pUser);

/** 
 *  \brief 设置设备透明通道推送,该接口4200客户端使用 不支持国标设备  不支持64位不支持小权限
 *  \param[in] pHandle			告警推送函数
 *  \param[in] pUser			用户自定义数据，回调函数会原样抛出
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Push_MessageHandler
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Push_SetTransparentChannelCallBack(OpenSDK_Push_MessageHandler pHandle, void* pUser);

/** 
 *  \brief // 上层需要设置回调  上层收到后  然后调用停止接口 再调用开始接口  该接口4200客户端使用 不支持国标设备  不支持64位不支持小权限
 *  \param[in] pHandle			告警推送函数
 *  \param[in] pUser			用户自定义数据，回调函数会原样抛出
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Push_MessageHandler
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Push_SetEventCallback(devSDK_EventHandler pEventHandle, void* pUser);

/** 
 *  \brief 开启接收推送 以前接口不再支持 需用该更新接口，长连接最新版本 该接口4200客户端使用 不支持国标设备  不支持64位不支持小权限
 *  \param[in] szAccessToken 认证token
 *  \return 0表示成功，-1表示失败, 失败时调用GetLastErrorCode()
 *  \sa OpenSDK_Push_OpenRecv()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Push_OpenRecv(const char* szAccessToken);

/** 
 *  \brief 关闭接收推送,不支持国标设备 注意：该接口4200客户端使用 不支持国标设备  不支持64位不支持小权限
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Push_CloseRecv()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Push_CloseRecv();

/** @} */ // end of push


/** \defgroup errorinfo 错误码信息接口
 *  @{
 */
/** 
 *  \brief 获取错误码接口
 *  \return 错误码
 *  \sa OpenSDK_GetLastErrorDesc
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_GetLastErrorCode();

/** 
 *  \brief 错误码描述
 *  \return 错误码描述字段
 *  \sa OpenSDK_GetLastErrorCode
 */
OPENSDK_API const char* CALLBACK OpenSDK_GetLastErrorDesc();

/** @} */ // end of errorinfo

/** \defgroup version 查询SDK版本信息接口
 *  @{
 */

/** 
 *  \brief SDK版本号
 *  \return SDK版本号字符
 *  \sa OpenSDK_GetSdkVersion
 */
OPENSDK_API const char* CALLBACK OpenSDK_GetSdkVersion();

/** @} */ // end of version

/** 
 *  \brief 云录像截屏，StartRealPlay成功（回调函数中收到MsgId == INS_PLAY_START）情况下使用,截图保存在内存
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_CapturePictureFromCloudRecord(const char* szSessionId);

/** 
 *  \brief 云录像截屏上传 OpenSDK_CapturePictureFromCloudRecord 成功后使用
 *  \param[in] szSessionId 会话Id
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_CapturePictureUpdata(const char* szSessionId);

/**
 * \brief       消息回调
 *  \param[in]  szSessionId 会话Id
 * \param [in]  msg_type    消息类型, 参见 OpenSDK_BWCHECK_MSG_TYPE
 * \param [in]  msg         消息内容, 参见 OpenSDK_BWCheckRes
 * \param [in]  userdata    用户自定义数据
 */

typedef void(CALLBACK *OpenSDK_BWCheckMsg)(const int iSession, int iMsgtype, void* pMsg, void* pUser);

/** 
 *  \brief // 带宽检测消息回调
 *  \param[in] pCheckMsg			消息回调
 *  \param[in] pUser			用户自定义数据，回调函数会原样抛出
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_SetBandwidthDetectionMsgCallback
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_SetBandWidthCheckMsgCallback(OpenSDK_BWCheckMsg pCheckMsg, void* pUser);

/** 
 *  \brief 打开带宽检测
 *  \param[in] szAccessToken             认证Token
 *  \param[in] szDevSerial               设备序列号（选填，如果账号下有设备则建议填写，测速会更精准）
 *  \param[in] iCheckTime                最大带宽测试时长、取值区间【10s~30s】
 *  \param[in] iCheckType                带宽检测类型 参见：OpenSDK_BWCHECK_CHECKTYPE
 *  \param[in] pUser                     用户自定义数据
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_BandWidthCheckOpen(const char* szAccessToken,  const int iCheckTime, const int iCheckType);

/** 
 *  \brief 关闭带宽检测
 *  \return 0表示成功，-1表示失败
 *  \sa OpenSDK_Push_OpenRecv()
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_BandWidthCheckClose();


#ifdef __cplusplus
}
#endif

//兼容老版本接口，需要更改老接口函数名，更改[OriginalFunName]_Old
#include "backward/OpenNetStreamInterfaceBackward.h"

#endif