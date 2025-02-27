/** \file      OpenNetStreamDefine.h
 *  \copyright HangZhou Ezviz All Right Reserved.
 *  \brief     开放SDK数据结构定义
 *
 *  \author    panlong, peter
 *  \date      2014/12/15
 *
 *  \note      history:
 *  \note      V0.0.1 2014/12/15 peter 增加头文件注释 
 */

#ifndef _H_OPENNETSTREAMDEFINE_H_
#define _H_OPENNETSTREAMDEFINE_H_

/** \typedef OPENSDK_RESULT
 *  \brief   opensdk返回值，0表示成功，非0表示失败
 */
typedef int OPENSDK_RESULT; 

/** \enum  MessageType 
 *  \brief 消息类型定义
 */
enum MessageType
{
	INS_PLAY_EXCEPTION            = 0,   ///< 【0】播放异常，通常是设备断线或网络异常造成
	INS_PLAY_RECONNECT            = 1,   ///< 【1】重连，实时流播放时内部会自动重连
	INS_PLAY_RECONNECT_EXCEPTION  = 2,   ///< 【2】重连异常
	INS_PLAY_START                = 3,   ///< 【3】播放开始
	INS_PLAY_STOP                 = 4,   ///< 【4】播放终止
	INS_PLAY_ARCHIVE_END          = 5,   ///< 【5】播放结束，回放结束时会有此消息
    INS_DOWNLOAD_START            = 13,  ///< 【13】下载开始
    INS_DOWNLOAD_STOP             = 14,  ///< 【14】下载结束
    INS_DOWNLOAD_EXCEPTION        = 15,  ///< 【15】下载异常，比如接收超时，数据错误，写文件失败
	INS_VOICETALK_START           = 16,  ///< 【16】语音对讲开始
	INS_VOICETALK_STOP            = 17,  ///< 【17】语音对讲停止
	INS_VOICETALK_EXCEPTION       = 18,  ///< 【18】语音对讲异常 
	INS_PTZ_EXCEPTION             = 19,  ///< 【19】云台控制异常
	INS_RECORD_FILE               = 20,  ///< 【20】查询的录像文件(录像搜索结果)
	INS_RECORD_SEARCH_END         = 21,  ///< 【21】录像查询结束（暂不使用）
	INS_RECORD_SEARCH_FAILED      = 22,  ///< 【22】录像查询失败
	INS_DEFENSE_SUCCESS			  = 23,  ///< 【23】布防成功
	INS_DEFENSE_FAILED			  = 24,  ///< 【24】布防失败
	INS_PLAY_ARCHIVE_EXCEPTION    = 28,	 ///< 【28】回放异常结束，可能是接收数据超时
	INS_PTZCTRL_SUCCESS			  = 46,  ///< 【46】云台控制命令发送成功
	INS_PTZCTRL_FAILED			  = 47,	 ///< 【47】云台控制失败

	INS_PLAYREAL_STAT			  = 91,	      ///< 【91】预览取流统计
	//INS_PLAYBACK_STAT             = 92,	  ///< 【92】回放取流统计
	//INS_PRE_CONNECTION_STAT       = 97,     ///< 【97】预链接统计
	//INS_PLAY_SWITCH_STAT		  = 99,	      ///< 【99】切换取流统计	
	//INS_NAT_INFO_REPORT		  = 101,      ///< 【101】NAT类型信息上报

    INS_USER_STOP_DOWNLOAD        = 108,  ///< 【108】主动停止下载
    INS_VTDU_STREAM_EXCEPTION     = 115,  ///< 【115】取流过程中设备与流媒体服务之间的链接异常断开
	INS_STREAM_STATUS_INFO        = 116,    //取流结果信息
    INS_PUNCH_INFO                = 117,    //预链接结果消息
	INS_VIDEO_COVER_STAT	      = 200,    //录像封面状态
	INS_DEV_TIMEOUT			      = 201,    //流媒体15秒没收到设备流超时
	INS_DEV_DISCONNECT			  = 202,    //取流过程中设备主动断开tcp连接
	INS_SEEK_TIMEOUT              = 203,    //设备支持seek 2.0 下发设备命令收到设备返回指令超时
	INS_MSG_UNKNOW                = 999,	//未知类型,其他异常
};

/** \struct STREAM_TIME
 *  \brief  开放SDK获取OSD时间格式定义
 */
typedef struct _STREAM_TIME
{
	unsigned int iYear;					 ///< 年
	unsigned int iMonth;				 ///< 月
	unsigned int iDay;					 ///< 日
	unsigned int iHour;					 ///< 时
	unsigned int iMinute;				 ///< 分
	unsigned int iSecond;				 ///< 秒
}STREAM_TIME, *LP_STREAM_TIME;

/** \struct NetStreamCallbackMessage
 *  \brief 经过NetStream处理所反馈的信息                                                                    
 */
typedef struct _NetStreamCallBackMessage
{
public:
	_NetStreamCallBackMessage(void):
	  iErrorCode(0), pMessageInfo(0){}
	int iErrorCode;			///< 消息回调反馈的错误码
	char *pMessageInfo;					///< 消息回调反馈的信息
} NSCBMsg, *LP_NSCBMsg;

/** \enum  AlarmType 
 *  \brief 开放SDK告警类型定义
 */
enum AlarmType
{
	ALARM_TYPE_ALL 					= -1,           ///< 全部
	BODY_SENSOR_EVENT				= 10000,		///< 人体感应事件
	EMERGENCY_BUTTON_EVENT 			= 10001,		///< 紧急遥控按钮事件
	MOTION_DETECT_ALARM 			= 10002,		///< 移动侦测告警
	BABY_CRY_ALARM 					= 10003,		///< 婴儿啼哭告警
	MAGNETIC_ALARM 					= 10004,		///< 门磁告警
	SMOKE_DETECTOR_ALARM 			= 10005,		///< 烟感告警
	COMBUSTIBLE_GAS_ALARM 			= 10006,		///< 可燃气体告警
	FLOOD_IN_ALARM 					= 10008,		///< 水浸告警
	EMERGENCY_BUTTON_ALARM 			= 10009,		///< 紧急按钮告警
	BODY_SENSOR_ALARM 				= 10010,		///< 人体感应告警
	SHELTER_ALARM					= 10011, 		///< 遮挡告警
	VIDEO_LOSS_ALARM				= 10012,		///< 视频丢失
	LINE_DETECTION_ALARM			= 10013,		///< 越界侦测
	FIELD_DETECTION_ALARM			= 10014,		///< 区域入侵
	FACE_DETECTION_ALARM			= 10015,		///< 人脸检测事件
	DOOR_BELL_ALARM					= 10016,		///< 智能门铃告警
	DEVOFFLINE_ALARM				= 10017,		///< 摄像机失去关联告警
	CURTAIN_ALARM					= 10018,		///< 幕帘告警
	MOVE_MAGNETOMETER_ALARM			= 10019,		///< 单体门磁告警
	SCENE_CHANGE_DETECTION_ALARM	= 10020,		///< 场景变更侦测
	DEFOCUS_ALARM					= 10021,		///< 虚焦侦测
	AUDIO_EXCEPTION_ALARM			= 10022,		///< 音频异常侦测
	LEFT_DETECTION_ALARM			= 10023,		///< 物品遗留侦测
	TAKE_DETECTION_ALARM			= 10024,		///< 物品拿取侦测
	PARKING_DETECTION_ALARM			= 10025,		///< 非法停车侦测
	HIGH_DENSITY_DETECTION_ALARM	= 10026,		///< 人员聚集侦测
	LOITER_DETECTION_ALARM			= 10027,		///< 徘徊检测侦测
	RUN_DETECTION_ALARM				= 10028,		///< 快速移动侦测
	ENTER_AREA_DETECTION_ALARM		= 10029,		///< 进入区域侦测
	EXIT_AREA_DETECTION_ALARM		= 10030,		///< 离开区域侦测
	MAG_GIM_ALARM					= 10031,		///< 磁干扰告警
	UNDER_VOLTAGE_ALARM				= 10032,		///< 电池欠压告警
	INTRUSION_ALARM					= 10033,		///< 闯入告警
	IO_00_ALARM						= 10100,		///< IO告警
	IO_01_ALARM						= 10101,		///< IO-1告警
	IO_02_ALARM						= 10102,		///< IO-2告警
	IO_03_ALARM						= 10103,		///< IO-3告警
	IO_04_ALARM						= 10104,		///< IO-4告警
	IO_05_ALARM						= 10105,		///< IO-5告警
	IO_06_ALARM						= 10106,		///< IO-6告警
	IO_07_ALARM						= 10107,		///< IO-7告警
	IO_08_ALARM						= 10108,		///< IO-8告警
	IO_09_ALARM						= 10109,		///< IO-9告警
	IO_10_ALARM						= 10110,		///< IO-10告警
	IO_11_ALARM						= 10111,		///< IO-11告警
	IO_12_ALARM						= 10112,		///< IO-12告警
	IO_13_ALARM						= 10113,		///< IO-13告警
	IO_14_ALARM						= 10114,		///< IO-14告警
	IO_15_ALARM						= 10115,		///< IO-15告警
	IO_16_ALARM						= 10116         ///< IO-16告警
};

/** \enum  AlarmNotifyType
 *  \brief 开放SDK告警通知类型定义
 */
typedef enum _AlarmNotifyType
{
	ALARM_NOTIFY_CONNECT_EXCEPTION = 100,	///< SDK同萤石平台连接异常
	ALARM_NOTIFY_RECONNECT_SUCCESS,			///< 重连成功
	ALARM_NOTIFY_RECONNECT_FAILED			///< 重连失败
} AlarmNotifyType;

/** \enum PTZCommand
 *  \brief 云台控制命令
 */
enum PTZCommand
{
	UP = 0,					///< 上
	DOWN,					///< 下
	LEFT,					///< 左
	RIGHT,					///< 右
	UPLEFT,					///< 上左
	DOWNLEFT,				///< 下左
	UPRIGHT,				///< 上右
	DOWNRIGHT,				///< 下右
	ZOOMIN,
	ZOOMOUT,
	FOCUSNEAR,
	FOCUSFAR,
	IRISSTARTUP,
	IRISSTOPDOWN,
	LIGHT,
	WIPER,
	AUTO
};

/** \enum PTZAction
 *  \brief 云台操作命令
 *  一般情况下，鼠标按下代表开始，鼠标松开代表停止
 */
enum PTZAction
{
	START = 0,						///< 开始			 
	STOP							///< 停止
};

/** \enum  DefenceType 
 *  \brief 布撤防告警类型
 */
enum DefenceType 
{
	PIR = 0,						///< 红外
	ATHOME,                         ///< 在家，A1设备
	OUTDOOR,					    ///< 外出
	BABYCRY,						///< 婴儿啼哭
	MOTIONDETECT,					///< 移动侦测
	GLOBAL							///< 全部
};

/** \enum  DefenceStatus 
 *  \brief 布撤防状态
 */
enum DefenceStatus
{
	UN_DEFENCE = 0,					///< 撤防
	DEFENCE,						///< 布防
	UN_SUPPORT,						///< 不支持
	FORCE_DEFENCE 					///< 强制布防，A1设备
};

enum DefenceActor
{
	D = 0,							///< 设备
	V,								///< 视频通道
	I 								///< IO通道
};

/** \enum  DataType 
 *  \brief 音视频流数据类型
 */
enum DataType {
	NET_DVR_SYSHEAD       = 1,    ///< 流头
	NET_DVR_STREAMDATA    = 2,	  ///< 流数据
	NET_DVR_RECV_END      = 3,    ///< 结束标记  
    NET_DVR_RECV_201      = 4,    ///< (云存储倍速回放)服务器返回的快放模式切换标记，当收到此标记时，快放模式从全帧快放切换成抽帧快放(网络质量不足以满足全帧快放时)  
    NET_DVR_RECV_202      = 5,    ///< (服务器返回的降低快放倍速消息,云存储服务器发来这个数据后开始降低快放倍速(云存储倍速回放是由云存储服务对码流做了倍速效果的处理,客户端直接播放码流即可，不需自己实现倍速播放)
	NET_DVR_RECV_AUDIO	  = 6,
	NET_DVR_SEND_AUDIO    = 7,
	NET_DVR_COVER_HEAD    = 8,
	NET_DVR_COVER_DATA	  = 9,	
	NET_DVR_AUDIO_HADE    = 10,
	NET_DVR_PLAY_SEEK     = 11    //seek2.0 收到该数据类型，需要清除以前缓存流数据
};

/** \enum  LoginParamKey 
 *  \brief 登录相关参数类型
 */
enum LoginParamKey {
	LOGIN_AREAID              = 1,      ///< 区域号, 目前只有海外使用
	LOGIN_VER                 = 2,		///< 登录页面版本信息
    LOGIN_AREADOMAIN          = 3,      ///< 登录后,重定向的域名
    LOGIN_TOKEN_EXPIRETIME    = 4,      ///< 登录后,Token失效时间
    LOGIN_ACCESS_TOKEN        = 5,      ///< 登录后,获取AccessToken
	LOGIN_LOGO			      = 6,		///< 自定义 登录logo 
	LOGIN_PRIVACY_POLICY	  = 7,		///< 自定义 隐私说明
	LOGIN_USER_TERMS		  = 8,  	///< 自定义 用户准则
};

/** \enum  ConfigKey 
 *  \brief 配置类型
 */
enum ConfigKey {
    CONFIG_DATA_UTF8           = 1,			///< 数据输出使用UTF8
    CONFIG_OPEN_STREAMTRANS    = 2,			///< 码流数据输出经过转封装处理, 用于录像存储
    CONFIG_CLOSE_P2P           = 3,			///< P2P开关, 用于关闭P2P
    CONFIG_LOG_LEVEL           = 4,			///< 配置日志等级,参见 #OpenSDK_LogLevel
	CONFIG_P2P_MAXNUM          = 6,			///< 设置预链接支持的最大路数
    CONFIG_CLOSE_REPORT        = 9,			///< 关闭取流上报, value = 1;
    CONFIG_TALK_CHANNEL        = 10,		///< 开启通道对讲, 1:开启， 0：关闭
	CONFIG_FAST_STREAM         = 13,		///< 快速出流开关 0: 关闭   1: 打开
    CONFIG_CLOSE_BLONG_SERIAL  = 14,		///< 是否关联设备 0: 关联   1: 不关联
    CONFIG_ClOSE_KEY_AUTH      = 15,		///< 关闭密钥验证 0: 不关闭 1: 关闭
	CONFIG_SLAVE_DATA		   = 16,		///< 保存取到的数据 0: 关闭 1: 打开 
	CONFIG_VTDU_STREAM		   = 17,		///< 强制流媒体取流 0: 关闭 1: 打开
	CONFIG_SLAVE_AUDIO		   = 18,		///< 保存对讲数据 0: 关闭 1: 打开
	CONFIG_P2P_LIMIT		   = 19,		///< p2p打洞限制端口数 0 关闭 1: 打开
	CONFIG_SET_PLAYBACK_STREAM_TYPE = 20,	///< 设置回放或下载主子码流
	CONFIG_SEARCH_FROM_CENTER = 21,			///< 设置本地录像查询位置，当前针对CVR设备 0: 关闭 1: 打开
	CONFIG_SET_PLAYBACK_RECORD_TYPE=22,		///< 设置回放录像类型 0：不使用 5：自动浓缩录像 6：定时浓缩录像 7：手动浓缩录像
	CONFIG_SET_PLAYBACK_RECORD_FINTERVAL=23,///< 设置回放录像帧间隔
};

typedef struct ST_EZ_RECORD_INFO
{
	char    szStartTime[32];		///< 录像开始时间
	char    szStopTime[32];			///< 录像结束时间
	int 	iRecType;			    ///< 录像来源,  1:本地录像.2:云录像
	char	szServerIp[64];			///< 服务Ip地址 云录像时使用
	int		iServerPort;			///< 服务Ip地址 云录像时使用
	int     iFileType;				///< 类型  云录像时使用
	int     iVideoType;				///< 录像类型  云录像时使用
	int		istorageVersion;		///< 云录像版本 云录像时使用
	char    szKeyChecksum[128];		///< 云录像MD5加密后的密钥 云录像时使用
	int		iRecordType;			///< 录像类型 本地录像使用 主要用于 压缩录像
	int		ifinterval;				///< 浓缩录像 帧间隔，单位秒 压缩录像时使用
	int 	iBusType;				///< 服务类型 云录制2.0使用
	int     iSpaceId;				///< 录像控件 云录制2.0使用
}EZ_RECORD_INFO;


/** \enum  OpenSDK_LogLevel 
 *  \brief 日志级别
 */
enum EZOPENSDK_LogLevel {
	EZOPENSDK_ALL		= 0,	///< 【0】打印全部日志
	EZOPENSDK_TRACE,			///< 【1】
	EZOPENSDK_DEBUG,			///< 【2】调试日志
	EZOPENSDK_INFO,				///< 【3】基本信息级别 
	EZOPENSDK_WARN,				///< 【4】警告日志
	EZOPENSDK_ERROR,			///< 【5】基本错误级别
    EZOPENSDK_OFF		= 7,    ///< 【7】无日志级别 

};

/** \enum  EZOPENSDK_AUDIO_CODE 
 *  \brief 定义语音编码类型
 */
enum EZOPENSDK_AUDIO_CODE {
    EZOPENSDK_AUDIO_CODE_G722_1      = 0,   ///< G722_1
    EZOPENSDK_AUDIO_CODE_G711_MU     = 1,   ///< G711_MU
    EZOPENSDK_AUDIO_CODE_G711_A      = 2,   ///< G711_A
    EZOPENSDK_AUDIO_CODE_G723        = 3,   ///< G723
    EZOPENSDK_AUDIO_CODE_MP1L2       = 4,   ///< MP1L2
    EZOPENSDK_AUDIO_CODE_MP2L2       = 5,   ///< MP2L2
    EZOPENSDK_AUDIO_CODE_G726        = 6,   ///< G726
    EZOPENSDK_AUDIO_CODE_AAC         = 7,   ///< AAC
    EZOPENSDK_AUDIO_CODE_G726_A      = 8,   ///< G726
    EZOPENSDK_AUDIO_CODE_G726_16     = 9,   ///< G726_16
    EZOPENSDK_AUDIO_CODE_G729        = 10,  ///< G729
    EZOPENSDK_AUDIO_CODE_ADPCM       = 11,  ///< ADPCM
    EZOPENSDK_AUDIO_CODE_AMR_NB      = 12,  ///< AMR_NB
    EZOPENSDK_AUDIO_CODE_RAW_DATA8   = 13,  ///< 采样率为8k的原始数据
    EZOPENSDK_AUDIO_CODE_RAW_UDATA16 = 14,  ///< 采样率16K的原始数据,即L16
    EZOPENSDK_AUDIO_CODE_MP2L2_32    = 15,	///< 
    EZOPENSDK_AUDIO_CODE_MP2L2_64    = 16,	///< 
    EZOPENSDK_AUDIO_CODE_AAC_32      = 17,	///< 
    EZOPENSDK_AUDIO_CODE_AAC_64      = 18,	///< 
    EZOPENSDK_AUDIO_CODE_OPUS_8      = 19,  ///< OPUS 8K
    EZOPENSDK_AUDIO_CODE_OPUS_16     = 20,  ///< OPUS 16K
    EZOPENSDK_AUDIO_CODE_OPUS_48     = 21,  ///< OPUS 48K
    EZOPENSDK_AUDIO_CODE_G729_A      = 22,  ///< G729_A
    EZOPENSDK_AUDIO_CODE_G729_B      = 23,  ///< G729_B
    EZOPENSDK_AUDIO_CODE_PCM         = 24,  ///< PCM
    EZOPENSDK_AUDIO_CODE_MP3         = 25,  ///< MP3
    EZOPENSDK_AUDIO_CODE_AC3         = 26,  ///< AC3
    EZOPENSDK_AUDIO_CODE_RAW         = 99,  ///< RAW
};

/** \enum  EZOPENSDK_RECORD_TYPE  
 *  \brief 定义录像文件类型
 */
enum EZOPENSDK_RECORD_TYPE{
    EZOPENSDK_RECORD_AUTO          = 0,   ///< 【0】设备和本地同时查询 
    EZOPENSDK_RECORD_CLOUD         = 1,   ///< 【1】查询云录像 
    EZOPENSDK_RECORD_DEVICE        = 2,   ///< 【2】查询设备本地录像 
};

/** \enum  EZOPENSDK_PLAYBACK_SPEED_TYPE  
 *  \brief 定义倍速回放速度类型
 */
enum EZOPENSDK_PLAYBACK_SPEED_TYPE
{
    EZOPENSDK_PLAYBACK_SPEED_NONE = 1,     ///< 【1】正常速率 
	EZOPENSDK_PLAYBACK_SPEED_X2,           ///< 【2】2倍速 
	EZOPENSDK_PLAYBACK_SPEED_S2,           ///< 【3】1/2倍速 
    EZOPENSDK_PLAYBACK_SPEED_X4,           ///< 【4】4倍速 
	EZOPENSDK_PLAYBACK_SPEED_S4,           ///< 【5】1/4倍速 
    EZOPENSDK_PLAYBACK_SPEED_X8,           ///< 【6】8倍速 
	EZOPENSDK_PLAYBACK_SPEED_S8,           ///< 【7】1/8倍速 
    EZOPENSDK_PLAYBACK_SPEED_X16,          ///< 【8】16倍速 
	EZOPENSDK_PLAYBACK_SPEED_S16			///< 【9】1/16倍速 
};


/**
* \brief 带宽检测类型
*/
typedef enum
{
	OPENSDK_BWCHECK_UPLOAD = 0X01,   ///< 上行
	OPENSDK_DOWNLOAD = 0X2,          ///< 下行
}OpenSDK_BWCHECK_CHECKTYPE;

/**
* \brief 带宽检测结果信息
*/
typedef struct
{
	float fDownloadSpeed;    ///< Mbps
	float fUploadSpeed;      ///< Mbps
	int iLatency;            ///< ms       //暂不支持忽略
	int iLoss;               ///< percent  //暂不支持忽略
}OpenSDK_BWCheckRes;


/**
* \brief 客户端功能区分
*/
typedef enum
{
	OPENSDK_MINOR_PERMISSION = 1, ///< 小权限
	OPENSDK_Private_Cloud = 2,    ///< 私有云
}OpenSDK_ClientType;

#endif