/** \file      OpenNetStreamInterfaceBackward.h
 *  \copyright HangZhou Ezviz All Right Reserved.
 *  \brief     开放SDK向后兼容接口定义
 *
 *  \author    pikongxuan
 *  \date      2015/07/31
 *
 *  \note      history:
 *  \note      V2.4.1 2015/07/31 
 */

#ifndef _H_OPENNETSTREAMINTERFACE_BACKWARD_H_
#define _H_OPENNETSTREAMINTERFACE_BACKWARD_H_

#ifdef __GNUC__
#define DEPRECATED(func)  func __attribute__ ((deprecated))
#elif defined(_MSC_VER)
#define DEPRECATED(func) __declspec(deprecated) func
#endif


#include "OpenNetStreamBackwardWarning.h"

#ifdef __cplusplus
extern "C" {
#endif

/** \defgroup alloc 分配会话
 *  @{
 */



/** @} */ // end of alloc

/** \defgroup play_old 预览接口（老接口，不再维护）
 *  @{
 */

/** 
 *  \brief 停止预览
 *  \param[in] szSessionId 会话ID
 *	\param[out] pNSCBMsg   同步回调传出参数,如果为同步，不允许为NULL
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_StopRealPlay()
 */
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopRealPlay(const char* szSessionId, LP_NSCBMsg pNSCBMsg = NULL));

/** @} */ // end of play_old

/** \defgroup playback_old 回放接口（老接口，不再维护）
 *  @{
 */
/** 
 *  \brief 停止回放
 *  \param[in] szSessionId 会话Id
 *	\param[out] pNSCBMsg   同步回调传出参数,如果为同步，不允许为NULL
 *  \return 0表示成功，非0表示失败
 *	\sa OpenSDK_StartPlayBack()
 */
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StopPlayBack(const char* szSessionId, LP_NSCBMsg pNSCBMsg = NULL));

/** @} */ // end of playback_old


/** 
 *  \brief 针对国标摄像头进行预览
           异步接口，返回值只是表示操作成功，不代表播放成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败
 *  \param[in] szSessionId   会话ID
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevice_id   设备标识
 *  \param[in] szChannel     国标设备通道号
 *  \param[in] szPlatform_id 平台id
 *  \param[in] szBizType     国标标识
 *  \param[in] szSafeKey	 视频加密密钥,如果视频未加密, 可以设置为NULL,目前暂未使用
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopRealPlayEx
 *	\sa OpenSDK_SetDataCallBack
 */

DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartRealPlay_gbCode(const char* szSessionId, HWND hPlayWnd, const char* szDevice_id, const char* szChannel,const char* szPlatform_id,const char* szBizType,const char* szSafeKey));

/** 
 *  \brief 用于国标设备的录像搜索，目前仅支持搜索设备录像 小权限该接口不支持
           异步接口，返回值只是表示操作成功，不代表搜索成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_RECORD_SEARCH_END, 表示成功, 若msgid= INS_RECORD_SEARCH_FAILED, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial   设备序列号
 *  \param[in] szChannel  设备序列号
 *  \param[in] szPlatform_id 平台id
 *  \param[in] szPlatform_id 国标标识，目前填写gb28181
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间 
 *  \param[in] iSearchType   该参数暂未使用
 *  \return 0表示成功，-1表示失败 
 *  \note
 *  时间格式为:yyyy-MM-dd hh:mm:ss
 */
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartSearchRecord(const char* szSessionId, const char* szDevSerial,const char* szChannel, const char* szPlatform_id, const char* szBizType,const char* szStartTime, const char* szStopTime, const int iSearchType));


/** 
*  \brief 开始回放，国标设备回放接口
          异步接口，返回值只是表示操作成功，不代表播放成功
          如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_PLAY_START, 表示成功, 若msgid=INS_PLAY_EXCEPTION, 表示失败,可调用此接口下载本地录像,
          只取流状态下不支持连续跨片段取流
 *  \param[in] szSessionId   会话Id
 *  \param[in] hPlayWnd      播放窗口句柄, 如果窗口句柄为NULL表示纯粹取流，不做播放
 *  \param[in] szDevice_id   国标设备设备序列号
 *  \param[in] szChannel     国标设备通道号
 *  \param[in] szPlatform_id 平台id
 *  \param[in] szBizType     国标标识，目前填写gb28181
 *  \param[in] szStartTime   开始时间
 *  \param[in] szStopTime    停止时间
 *  \param[in] szSafeKey     暂未使用该参数,传空
 *  \return 0表示成功，非0表示失败
 *  \sa OpenSDK_StopPlayBackEx()
 *  \note 
 *  时间格式为：yyyy-MM-dd hh:mm:ss
 */
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_StartPlayBack_gbCode(const char* szSessionId, HWND hPlayWnd, const char* szDevice_id, const char* szChannel,const char* szPlatform_id,const char* szBizType, const char* szStartTime, const char* szStopTime,const char* szSafeKey));


/*
*  \brief 获取国标摄像头列表
*  \param[in]  szBizType     国标标识,目前填写gb28181
*  \param[in]  iPageStart    分页起始页，从0开始
*  \param[in]  iPageSize     分页大小, 限制最大36,默认36
*  \param[out] pBuf          摄像头列表的JSON字符串
*  \param[out] iLength       获取到的数据大小
*  \return 0表示成功,       -1表示失败 
*  \note 
*  摄像头列表的JSON格式如下
*{
*   "result": {
*                 "page": {
*                 "total": 1,
*                 "page": 0,
*                 "size": 36
*                 },
*                 "data": {
*                    "device":[
*                        {
*                              "devLogicId": "1207995620812394496",//国标设备logic_id
*                              "deviceName": "IP CAMERA",          //设备名称
*                              "deviceModel": "DS-2CD7D27DWD-IZS", //设备型号
*                              "deviceVersion": "V5.5.91",         //设备版本
*                              "platformId": "33010142992577394892", //平台id
*                              "deviceId": "33010672991327538650",   //设备唯一标识
*                              "status": 1,                          //设备在线状态,1 表示在线 2表示离线
*                              "activatedStatus": 1,                 //设备激活状态
*                              "dasId": "das_10.215.35.14_1579177660", 
*                              "netAddress": "123.157.216.218",
*                              "manufacturer": "Ezviz",
*                              "createTime": 1577067330525,          
*                              "updateTime": 1582774537577
*                        }
*                     ]
*               },
*               "code": "200",
*               "msg": "操作成功!"
*        }
*}
*/
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetDeviceList(const char* szBizType, int iPageStart, int iPageSize, void** pBuf, int* iLength));

/** 
 *  \brief 国标设备获取设备详细信息接口,该接口仅用于国标设备的信息的获取.
 *  \param[in]  szDevSerial     设备序列号
 *  \param[in]  szChannel       设备通道号
 *  \param[in]  szBizType       国标标识
 *  \param[in]  szPlatform_id   平台id
 *  \param[in]  bUpdate         是否重新获取设备详细信息, 暂未使用该参数
 *	\param[out] pDevDetailInfo	监控点信息，需要调用OpenSDK_FreeData接口释放
 *	\param[out] iLength			监控点信息的长度
 *  \note 
 *  JSON格式如下：
 *  \code {.json}
 *  {				       
 *      "devSerial":"33010709991327238715",	        // 设备序列号
 *		"deviceStatus":1,				            // 设备在线状态:1在线 2离线
 *		"devlogicId":"1204664917580410880",		    // 设备逻辑Id			           
 *      "strCameraNo": "33010709991327238715",      // 设备通道号
 *  }  
 *  \endcode
 *  \return 0表示成功，其他值表示失败
 */
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_Data_GetDeviceDetailInfo(const char* szDevSerial, const char* szChannel, const char* szBizType, const char*szPlatform_id, const bool bUpdate, void** pDevDetailInfo, int* iLength));


/** 
 *  \brief 按监控点ID进行布撤防, 针对NVR的通道进行布撤防，需要NVR和IPC支持之间协议支持,不支持国标设备
           异步接口，返回值只是表示操作成功，不代表布撤防成功
           如果接口返回-1, 结果根据消息回调函数msgid判断, 若msgid=INS_DEFENSE_SUCCESS, 表示成功, 若msgid=INS_DEFENSE_FAILED, 表示失败
 *  \param[in] szSessionId   会话Id
 *  \param[in] szDevSerial	 设备序列号
 *  \param[in] iChannelNo  	 设备通道号
 *  \param[in] enType        布撤防类型
 *  \param[in] enStatus      布撤防状态
 *  \param[in] enActor		 布撤防设备类型
 *  \return 非负表示成功，-1表示失败
 */
DEPRECATED(OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_DevDefenceEx(const char* szSessionId, const char* szDevSerial, const int iChannelNo, DefenceType enType, DefenceStatus enStatus, DefenceActor enActor));

/** @} */ // end of device

/** 
 *  \brief 截屏，StartRealPlay成功（回调函数中收到MsgId == INS_PLAY_START）情况下使用
 *  \param[in] szSessionId 会话Id
 *  \param[in] szFileName  图片保存路径，格式为JPG,需要为UTF-8格式
 *  \return 0表示成功，-1表示失败
 */
OPENSDK_API OPENSDK_RESULT CALLBACK OpenSDK_CapturePicture(const char* szSessionId, const char* szFileName);

#ifdef __cplusplus
}
#endif

#endif