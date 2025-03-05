#include "VideoFrame.h"
#include <QResizeEvent>
#include <QEvent>
#include <QDebug>
#include <thread>
#include <mutex>


VideoFrame::VideoFrame()
{
    setStyleSheet("QFrame{border:1px solid gray}");
    connect(this, &VideoFrame::playException, this, &VideoFrame::slotPlayException);
}


VideoFrame::~VideoFrame()
{
    qDebug() << "VideoFrame::~VideoFrame";
}


void VideoFrame::setSize(int iWidth, int iHeight)
{
    setMinimumSize(iWidth, iHeight);
    m_videoFrame.setParent(this);
    m_videoFrame.setMinimumSize(iWidth - 1*2, iHeight - 1*2);
    m_videoFrame.move(1, 1);

    m_vboxTips.setMargin(0);
    m_vboxTips.setSpacing(0);
    m_vboxTips.setAlignment(Qt::AlignHCenter);

    QFont font;
    font.setPixelSize(15);
    font.setFamily(u8"微软雅黑");
    m_textTips.setFont(font);
    m_textTips.setAlignment(Qt::AlignCenter);
    m_textTips.setStyleSheet("background:transparent;border:none;color:red;font:15px");

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
    }
}


void VideoFrame::stopRealPlay(const QString& sessionId)
{
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
        qDebug() << "OpenSDK_GetLastErrorDesc >>>>> : " << OpenSDK_GetLastErrorDesc();
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
