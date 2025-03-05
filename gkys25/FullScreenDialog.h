#ifndef FULLSCREENDIALOG_H
#define FULLSCREENDIALOG_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QLabel>
#include <QPushButton>
#include <QGridLayout>
#include "VideoFrame.h"

namespace Ui {
class FullScreenDialog;
}

class FullScreenDialog : public QDialog
{
    Q_OBJECT

public:
    explicit FullScreenDialog(QWidget *parent = nullptr);
    ~FullScreenDialog();

    void setDevList(int index, const QString& sessionId, QJsonArray* devArray);
    void createVideos(int videoCount);
    void createVideoByIndex(int index);

public slots:
    void onExitFullScreen();
    void onVideoCountChange();

    void onActionOneVideo();
    void onActionFourVideo();
    void onActionNineVideo();
    void onActionSixteenVideo();

    void onPreviousClick();
    void onNextClick();

private:
    Ui::FullScreenDialog *ui;
    int m_currIndex;
    QJsonArray* m_devArray;
    QString m_sessionId;

    QVBoxLayout m_vboxMain;
    QHBoxLayout m_hboxMenu;
    QGridLayout m_gridLayout;
    QPushButton m_btnPrevious;
    QPushButton m_btnNext;
    QLabel m_textPageNum;
    QPushButton m_btnExitFull;
    QPushButton m_btnVideoCount;
    QFrame* m_frameVideos;
    QVector<VideoFrame*> m_vecFrames;
    int m_currVideoCount;
    int m_currVideoIndex;

    void initControls();
    void clearGridLayout();
};

#endif // FULLSCREENDIALOG_H
