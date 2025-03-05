#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include "OpenNetStreamDefine.h"
#include "OpenNetStreamError.h"
#include "OpenNetStreamInterFace.h"

class VideoFrame : public QWidget
{
    Q_OBJECT
public:
    VideoFrame();
    ~VideoFrame();
    void setSize(int iWidth, int iHeight);
    void startPlay(const QString& devSerialNum, int channelNo);
    void stopPlay();
    void updateBackground();
    void setPlayStatus(int type);

signals:
    void frameDbClick(int itemIndex, int channelIndex);
    void playException(VideoFrame* pFrame, int msgType);

public slots:
    void slotPlayException(VideoFrame* pFrame, int msgType);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    bool event(QEvent* event) override;

private:
    QFrame m_videoFrame;
    QString m_sessionId;
    QString m_devSerialNum;
    int m_channelNo;

    QHBoxLayout m_vboxTips;
    QLabel m_textTips;


    void realPlayVideo(const QString& devSerialNum, int channelNo, WId hwnd, QString* sessionId);
    void stopRealPlay(const QString& sessionId);
    static void CALLBACK messageHandler(const char* szSessionId, unsigned int iMsgType,
                                             unsigned int iErrorCode,const char *pMessageInfo, void *pUser);

};

#endif // VIDEOFRAME_H
