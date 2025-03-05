#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QPushButton>
#include <QTableView>
#include "OpenNetStreamDefine.h"
#include "OpenNetStreamError.h"
#include "OpenNetStreamInterFace.h"
#include "curl/curl.h"
#include "DeviceTableModel.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

signals:
    void getAccessTokenOK();

public slots:
    void onAccessTokenOK();
    void onItemDbClick(int index);

    void onCloseClick();
    void onMinClick();
    void onMaxClick();


protected:
    void closeEvent(QCloseEvent *event) override;


private:
    Ui::MainWindow *ui;
    QVBoxLayout m_vboxMain;
    QTableView m_tableView;
    QHBoxLayout m_hboxTitle;

    QPushButton m_btnClose;
    QPushButton m_btnMaxWinddow;
    QPushButton m_btnMinWindow;
    DeviceTableModel* m_tableModel;

    QJsonArray m_devList;
    QString m_sessionId;
    bool m_isMaxWnd;

    void initControls();
    void initTableView();
    void sendHttpRequest();
    static size_t writeDataCallback(char* buffer, size_t size,
                             size_t nitems, void* outstream);

    static void CALLBACK messageHandler(const char* szSessionId, unsigned int iMsgType,
                                        unsigned int iErrorCode,const char *pMessageInfo, void *pUser);
};
#endif // MAINWINDOW_H
