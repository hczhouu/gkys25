#ifndef FULLSCREENDIALOG_H
#define FULLSCREENDIALOG_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QFrame>
#include <QPushButton>
#include <QGridLayout>

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

public slots:
    void onExitFullScreen();
    void onVideoCountChange();

    void onActionOneVideo();
    void onActionFourVideo();
    void onActionNineVideo();
    void onActionSixteenVideo();

private:
    Ui::FullScreenDialog *ui;
    int m_currIndex;
    QJsonArray* m_devArray;
    QString m_sessionId;

    QVBoxLayout m_vboxMain;
    QHBoxLayout m_hboxMenu;
    QGridLayout m_gridLayout;
    QPushButton m_btnExitFull;
    QPushButton m_btnVideoCount;
    QFrame* m_frameVideos;

    void initControls();
    void clearGridLayout();
};

#endif // FULLSCREENDIALOG_H
