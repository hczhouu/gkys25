#include "VideoFrame.h"
#include <QResizeEvent>
#include <QEvent>
#include <QDebug>
#include <thread>
#include <mutex>
#include <QDate>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

std::atomic<bool> exitFlags;
std::atomic<int>  alarmCount;

VideoFrame::VideoFrame()
{
    setStyleSheet("QFrame{border:1px solid gray}");
    connect(this, &VideoFrame::playException, this, &VideoFrame::slotPlayException);
    connect(this, &VideoFrame::playAlarmSound, this, &VideoFrame::slotPlayAlarmSound, Qt::UniqueConnection);

    exitFlags.store(false);
    alarmCount.store(0);
    m_player = std::make_shared<QMediaPlayer>();
    m_playerList = std::make_shared<QMediaPlaylist>();
    m_player->setVolume(100);
    m_playerList->addMedia(QMediaContent(QUrl("qrc:/res/alarm.mp3")));
    m_player->setPlaylist(m_playerList.get());
    connect(m_player.get(), &QMediaPlayer::stateChanged, this, &VideoFrame::stateChanged);
}


VideoFrame::~VideoFrame()
{
    exitFlags.store(true);
}


void VideoFrame::setSize(int iWidth, int iHeight)
{
    setMinimumSize(iWidth, iHeight);
    m_videoFrame.setParent(this);
    m_videoFrame.setMinimumSize(iWidth - 1 * 2, iHeight - 1 * 2);
    m_videoFrame.move(1, 1);

    m_vboxTips.setMargin(0);
    m_vboxTips.setSpacing(0);
    m_vboxTips.setAlignment(Qt::AlignHCenter);

    QFont font;
    font.setPixelSize(20);
    font.setFamily(u8"微软雅黑");
    m_textTips.setFont(font);
    m_textTips.setAlignment(Qt::AlignCenter);
    m_textTips.setStyleSheet("background:transparent;border:none;color:red;");

    m_vboxTips.addWidget(&m_textTips);
    m_videoFrame.setLayout(&m_vboxTips);
}

void VideoFrame::resizeEvent(QResizeEvent *event)
{
    int iTempWidth = event->size().width();
    int iTempHeight = event->size().height();
    m_videoFrame.resize(iTempWidth - 1 * 2, iTempHeight - 1 * 2);
}


bool VideoFrame::event(QEvent* event)
{
    return QWidget::event(event);
}


void VideoFrame::mouseDoubleClickEvent(QMouseEvent *event)
{

}


void CALLBACK VideoFrame::messageHandler(const char* szSessionId, unsigned int iMsgType,
                                         unsigned int iErrorCode,const char *pMessageInfo, void *pUser)
{
    VideoFrame* pframe = reinterpret_cast<VideoFrame*>(pUser);
    if (pframe == nullptr)
    {
        return;
    }

    emit pframe->playException(pframe, iMsgType);
}


void VideoFrame::startPlay(const QString& devSerialNum, int channelNo)
{
    m_devSerialNum = devSerialNum;
    m_channelNo = channelNo;
    std::thread(&VideoFrame::realPlayVideo,
                this, devSerialNum, channelNo, m_videoFrame.winId(), &m_sessionId).detach();
}


void VideoFrame::stopPlay()
{
    std::thread(&VideoFrame::stopRealPlay,
                this, m_sessionId).detach();
}


void VideoFrame::updateBackground()
{
    m_videoFrame.setStyleSheet("background:black");
}


void VideoFrame::realPlayVideo(const QString& devSerialNum, int channelNo, WId hwnd, QString* sessionId)
{
    char* sessionBuf  =  nullptr;
    int sessionLen = 0;
    if(OpenSDK_AllocSessionEx(messageHandler, this, &sessionBuf, &sessionLen) != OPEN_SDK_NOERROR)
    {
        qDebug() << "OpenSDK_AllocSessionEx failed";
        return;
    }

    *sessionId = static_cast<char*>(sessionBuf);
    if(OpenSDK_StartPlayWithStreamType(sessionId->toStdString().data(), (HWND)hwnd,
       devSerialNum.toStdString().data(), channelNo, NULL, 1) != OPEN_SDK_NOERROR)
    {
        qDebug() << "OpenSDK_StartPlayWithStreamType failed";
        return;
    }

    getAlarmList();

    if (OpenSDK_OpenSound(sessionBuf) != OPEN_SDK_NOERROR)
    {
        qDebug() << "OpenSDK_OpenSound failed";
        return;
    }
}


void VideoFrame::stopRealPlay(const QString& sessionId)
{
    if (OpenSDK_CloseSound(sessionId.toStdString().data()) != OPEN_SDK_NOERROR)
    {
        qDebug() << "OpenSDK_CloseSound failed";
    }


    if(OpenSDK_StopRealPlayEx(sessionId.toStdString().data()) != OPEN_SDK_NOERROR)
    {
        qDebug() << "OpenSDK_StopRealPlayEx failed";
    }

    if(OpenSDK_FreeSession(sessionId.toStdString().data()) != OPEN_SDK_NOERROR)
    {
        qDebug() << "OpenSDK_FreeSession failed";
    }
}


void VideoFrame::slotPlayException(VideoFrame* pFrame, int msgType)
{
    pFrame->setPlayStatus(msgType);
}


void VideoFrame::setPlayStatus(int type)
{
    switch (type) {
    case INS_PLAY_EXCEPTION:
    {
        m_textTips.setText(u8"播放异常,正在重试...");
        OpenSDK_GetLastErrorCode();
    }

        break;
    case INS_PLAY_START:
    {
        m_textTips.setText(u8"开始播放...");
    }

        break;
    case INS_PLAY_STOP:
    {
        m_textTips.setText(u8"停止播放...");
    }

        break;
    default:
        break;
    }
}


void CALLBACK  VideoFrame::pushMessageHandler(const char* szDesc, const char* szContent,  const char* szDetail, void* pUser)
{
    qDebug() << szDesc << szContent << szDetail << pUser;
}


void VideoFrame::getAlarmList()
{
    std::thread(&VideoFrame::getAlarmInfo, this,
                m_devSerialNum.toStdString(), m_channelNo).detach();
}



void VideoFrame::getAlarmInfo(const std::string& devSerialNum, const int channelNo)
{
    while (!exitFlags.load())
    {
        void* pBuf = NULL;
        int length = 0;
        QDate currDate = QDate::currentDate();
        QString startTime = currDate.toString("yyyy-MM-dd 00:00:00");
        QString stopTime  = currDate.toString("yyyy-MM-dd 23:59:59");
        if (OpenSDK_Data_GetAlarmListEx(devSerialNum.data(), channelNo,
                                        startTime.toStdString().data(), stopTime.toStdString().data(),
                                        ALARM_TYPE_ALL, 0,  0, 100, &pBuf, &length) != OPEN_SDK_NOERROR)
        {
            continue;
        }

        QByteArray jsonData =  QByteArray(static_cast<char*>(pBuf), length);
        qDebug() << "alarm list >>>>>>>>>>>>>>>>> : " << jsonData.data();

        QJsonParseError jsonError;
        QJsonDocument doucment = QJsonDocument::fromJson(jsonData, &jsonError);  // 转化为 JSON 文档
        if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError))
        {
            OpenSDK_Data_Free(pBuf);
            continue;
        }

        QJsonObject object = doucment.object();
        QJsonObject jsonResult = object.value("result").toObject();
        QJsonArray arrData =  jsonResult.value("data").toArray();
        if (arrData.empty())
        {
            OpenSDK_Data_Free(pBuf);
            continue;
        }

        if (arrData.empty())
        {
            OpenSDK_Data_Free(pBuf);
            std::this_thread::sleep_for(std::chrono::seconds(5));
            continue;
        }

        foreach (const QJsonValue& item, arrData)
        {
            QString alarmId = item.toObject().value("alarmId").toString();
            OpenSDK_Data_SetAlarmRead(OpenSDK_GetLoginResponseParams(LOGIN_ACCESS_TOKEN),
                                      alarmId.toStdString().data());

            if (item.toObject().value("alarmType").toInt() == FIELD_DETECTION_ALARM ||
                item.toObject().value("alarmType").toInt() == MOTION_DETECT_ALARM )
            {
                emit playAlarmSound();
            }
        }

        OpenSDK_Data_Free(pBuf);
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }
}


void VideoFrame::slotPlayAlarmSound()
{
    if (m_player->state() != QMediaPlayer::PlayingState)
    {
        m_player->play();
    }
}


void VideoFrame::stateChanged(QMediaPlayer::State newState)
{
    if (newState == QMediaPlayer::StoppedState)
    {
        if (alarmCount.load() > 2)
        {
            alarmCount.store(0);
            return;
        }

        alarmCount.fetch_add(1);
        m_player->play();
    }
}
