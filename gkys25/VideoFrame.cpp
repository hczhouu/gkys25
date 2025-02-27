#include "VideoFrame.h"
#include <QResizeEvent>
#include <QEvent>
#include <QDebug>

VideoFrame::VideoFrame()
{
    setStyleSheet("QFrame{border:1px solid gray}");
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
    if (event->button() == Qt::LeftButton)
    {
        //emit frameDbClick(m_itemIndex, m_channelIndex);
    }
}


void VideoFrame::startPlay(const QString& sessionId, const QString& devSerialNum, int channelNo)
{
    m_sessionId = sessionId;
    m_devSerialNum = devSerialNum;
    m_channelNo = channelNo;

    qDebug() << ">>>>>>>>>>>>>>>>>> : " << this <<m_sessionId << m_devSerialNum << m_channelNo;

    if(OpenSDK_StartRealPlayEx(m_sessionId.toStdString().data(), (HWND)this->winId(),
       m_devSerialNum.toStdString().data(), m_channelNo, NULL) != OPEN_SDK_NOERROR)
    {
        qDebug() << "play failed";
    }
}


void VideoFrame::stopPlay()
{
    if(OpenSDK_StopRealPlayEx(m_sessionId.toStdString().data()) != OPEN_SDK_NOERROR)
    {
        qDebug() << "stop failed";
    }
}


void VideoFrame::updateBackground()
{
    m_videoFrame.setStyleSheet("background:black");
}

