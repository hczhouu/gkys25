#ifndef VIDEOFRAME_H
#define VIDEOFRAME_H

#include <QWidget>
#include <QFrame>
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
    void startPlay(const QString& sessionId, const QString& devSerialNum, int channelNo);
    void stopPlay();
    void updateBackground();

signals:
    void frameDbClick(int itemIndex, int channelIndex);

protected:
    void resizeEvent(QResizeEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    bool event(QEvent* event) override;

private:
    QFrame m_videoFrame;
    QString m_sessionId;
    QString m_devSerialNum;
    int m_channelNo;

};

#endif // VIDEOFRAME_H
