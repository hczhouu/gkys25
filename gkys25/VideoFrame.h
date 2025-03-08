#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QWidget>
#include <QFrame>
#include <QVBoxLayout>
#include <QLabel>
#include <QString>
#include <atomic>
#include <QMediaPlayer>
#include <QMediaPlaylist>
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
    void playAlarmSound();

public slots:
    void slotPlayException(VideoFrame* pFrame, int msgType);
    void slotPlayAlarmSound();
    void stateChanged(QMediaPlayer::State newState);
    void slotEnablePlaySound(bool enablePlay);

protected:
    void resizeEvent(QResizeEvent *event) override;
    bool event(QEvent* event) override;

private:
    QFrame m_videoFrame;
    QString m_sessionId;
    QString m_devSerialNum;
    int m_channelNo;

    QHBoxLayout m_vboxTips;
    QLabel m_textTips;
    std::shared_ptr<QMediaPlayer> m_player;
    std::shared_ptr<QMediaPlaylist> m_playerList;
    std::atomic<bool> m_enablePlaySound;

    void getAlarmList();
    void getAlarmInfo(const std::string& devSerialNum, const int channelNo);
    void realPlayVideo(const QString& devSerialNum, int channelNo, WId hwnd, QString* sessionId);
    void stopRealPlay(const QString& sessionId);
    static void CALLBACK messageHandler(const char* szSessionId, unsigned int iMsgType,
                                             unsigned int iErrorCode,const char *pMessageInfo, void *pUser);
    static void CALLBACK  pushMessageHandler(const char* szDesc, const char* szContent,  const char* szDetail, void* pUser);

};

#endif // VIDEOFRAME_H
