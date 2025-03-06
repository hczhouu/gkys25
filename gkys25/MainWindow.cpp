#include "MainWindow.h"
#include "ui_MainWindow.h"
#include <QtDebug>
#include <thread>
#include <string>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <iostream>
#include <QTextCodec>
#include <QFile>
#include <QTextStream>
#include "CustomHeaderView.h"
#include "TextItemDelegate.h"
#include "FullScreenDialog.h"
#include "json/json.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1200, 800);
    setMenuBar(nullptr);
    setStatusBar(nullptr);
    setStyleSheet("background:#0F1C2E");
    setWindowTitle(u8"九林酒店监控预警系统");
    m_isMaxWnd = false;
    initControls();
    //setWindowFlags(Qt::FramelessWindowHint);
    connect(this, &MainWindow::deviceListChange, this, &MainWindow::slotDeviceListChange, Qt::UniqueConnection);
    std::thread(&MainWindow::sendHttpRequest, this).detach();
}

MainWindow::~MainWindow()
{
    OpenSDK_FiniLib();
    delete ui;
}


void MainWindow::initControls()
{
    m_vboxMain.setMargin(0);
    m_vboxMain.setSpacing(0);

    initTableView();

    m_btnMinWindow.setMinimumSize(40, 40);
    m_btnMinWindow.setStyleSheet("background:transparent;border:none");
    m_btnMinWindow.setIcon(QIcon(":/res/min.png"));
    m_btnMinWindow.setIconSize(QSize(25,25));
    connect(&m_btnMinWindow, &QPushButton::clicked, this, &MainWindow::onMinClick);

    m_btnMaxWinddow.setMinimumSize(40, 40);
    m_btnMaxWinddow.setStyleSheet("background:transparent;border:none");
    m_btnMaxWinddow.setIcon(QIcon(":/res/max.png"));
    m_btnMaxWinddow.setIconSize(QSize(25,25));
    connect(&m_btnMaxWinddow, &QPushButton::clicked, this, &MainWindow::onMaxClick);

    m_btnClose.setMinimumSize(40, 40);
    m_btnClose.setStyleSheet("background:transparent;border:none");
    m_btnClose.setIcon(QIcon(":/res/close.png"));
    m_btnClose.setIconSize(QSize(25,25));
    connect(&m_btnClose, &QPushButton::clicked, this, &MainWindow::onCloseClick);

    m_hboxTitle.setMargin(0);
    m_hboxTitle.setSpacing(10);
    m_hboxTitle.setAlignment(Qt::AlignVCenter);
    m_hboxTitle.addStretch(1);

    m_hboxTitle.addWidget(&m_btnMinWindow);
    m_hboxTitle.addWidget(&m_btnMaxWinddow);
    m_hboxTitle.addWidget(&m_btnClose);

    //m_vboxMain.addLayout(&m_hboxTitle);
    m_vboxMain.addWidget(&m_tableView);


    QWidget* widget = new QWidget();
    setCentralWidget(widget);
    widget->setLayout(&m_vboxMain);
}


void MainWindow::initTableView()
{
    m_tableView.setMinimumSize(width(), height());
    m_tableView.setStyleSheet("QTableView{background:#117195;border:none;color:white}"
                              "QTableView::item{background:blue}");

    CustomHeaderView* headerView = new CustomHeaderView(Qt::Horizontal, &m_tableView);
    headerView->setObjectName("hHeader");
    m_tableView.setHorizontalHeader(headerView);

    m_tableView.horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);
    m_tableView.horizontalHeader()->setFixedHeight(40);
    m_tableView.setShowGrid(false);
    m_tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    m_tableView.setSelectionMode(QAbstractItemView::SingleSelection);

    //隐藏垂直表头
    m_tableView.verticalHeader()->setVisible(false);
    m_tableView.horizontalHeader()->setStyleSheet("QHeaderView::section {background-color: qlineargradient(x1:0, y1:0, x2:0, y2:1,\
    stop:0 transparent, stop: 0.5 transparent,stop: 0.6 transparent, stop:1 transparent);color: white;"
    "border:none;}"
    "QHeaderView{border:none;background:#117195;color:white}");

    m_tableModel = new DeviceTableModel();
    m_tableView.setModel(m_tableModel);
    m_tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
    //设置列宽
    m_tableView.setColumnWidth(0, 150);
    m_tableView.setColumnWidth(1, 400);
    m_tableView.setColumnWidth(2, 200);
    m_tableView.setColumnWidth(3, 200);
    m_tableView.setColumnWidth(4, 130);

    TextItemDelegate* devIndexItemDelegate = new TextItemDelegate(&m_tableView);
    connect(devIndexItemDelegate, &TextItemDelegate::itemDbClick, this, &MainWindow::onItemDbClick);
    TextItemDelegate* devNameItemDelegate = new TextItemDelegate(&m_tableView);
    connect(devNameItemDelegate, &TextItemDelegate::itemDbClick, this, &MainWindow::onItemDbClick);
    TextItemDelegate* devSerialItemDelegate = new TextItemDelegate(&m_tableView);
    connect(devSerialItemDelegate, &TextItemDelegate::itemDbClick, this, &MainWindow::onItemDbClick);
    TextItemDelegate* chanNumItemDelegate = new TextItemDelegate(&m_tableView);
    connect(chanNumItemDelegate, &TextItemDelegate::itemDbClick, this, &MainWindow::onItemDbClick);
    TextItemDelegate* videoLevelItemDelegate = new TextItemDelegate(&m_tableView);
    connect(videoLevelItemDelegate, &TextItemDelegate::itemDbClick, this, &MainWindow::onItemDbClick);

    m_tableView.setItemDelegateForColumn(0, devIndexItemDelegate);
    m_tableView.setItemDelegateForColumn(1, devNameItemDelegate);
    m_tableView.setItemDelegateForColumn(2, devSerialItemDelegate);
    m_tableView.setItemDelegateForColumn(3, chanNumItemDelegate);
    m_tableView.setItemDelegateForColumn(4, videoLevelItemDelegate);
    m_tableView.horizontalHeader()->setStretchLastSection(true);

    m_tableView.verticalHeader()->setMinimumSectionSize(35);
    for (int i = 0; i < m_tableModel->rowCount(); i++)
    {
        m_tableView.verticalHeader()->resizeSection(i, 35);
    }
}


void MainWindow::closeEvent(QCloseEvent *event)
{
    QApplication::quit();
}


void MainWindow::onItemDbClick(int index)
{
    FullScreenDialog* fullscreenDlg = new FullScreenDialog(this);
    fullscreenDlg->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    fullscreenDlg->setModal(true);
    fullscreenDlg->setDevList(index, m_sessionId,  &m_devList);
    fullscreenDlg->exec();
}


void CALLBACK MainWindow::messageHandler(const char* szSessionId, unsigned int iMsgType,
                                         unsigned int iErrorCode,const char *pMessageInfo, void *pUser)
{
    //qDebug() << " szSessionId >>>>>>>>>>>>>>>>>>>>>>>>>>>>>: " <<   pMessageInfo;
}


void MainWindow::sendHttpRequest()
{
    curl_global_init(CURL_GLOBAL_ALL);
    std::string accessToken  = "";
    std::string url = "https://open.ys7.com/api/lapp/token/get";
    std::string postData = "appKey=1a2dff9fcc3d4111b1531e73bb9930e7&appSecret=3322f3b4eeb6d988f0a5a1c6a0afe2e3";
    CURL* pCurl = curl_easy_init();
    curl_easy_setopt(pCurl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYPEER, 0L);
    curl_easy_setopt(pCurl, CURLOPT_SSL_VERIFYHOST, 0L);
    curl_easy_setopt(pCurl, CURLOPT_TRANSFER_ENCODING, 1L);
    curl_easy_setopt(pCurl, CURLOPT_WRITEFUNCTION, writeDataCallback);
    curl_easy_setopt(pCurl, CURLOPT_WRITEDATA, &accessToken);
    curl_easy_setopt(pCurl, CURLOPT_CONNECTTIMEOUT, 20L);
    curl_easy_setopt(pCurl, CURLOPT_FOLLOWLOCATION, 1L);
    curl_easy_setopt(pCurl, CURLOPT_POST, 1L);
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDS, postData.data());
    curl_easy_setopt(pCurl, CURLOPT_POSTFIELDSIZE, postData.size());
    curl_easy_setopt(pCurl, CURLOPT_NOPROXY, "*");
    curl_easy_perform(pCurl);
    curl_easy_cleanup(pCurl);
    curl_global_cleanup();

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(QByteArray::fromStdString(accessToken), &jsonError);
    if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
        return;
    }

    QJsonObject object = doucment.object();
    QJsonObject objectData = object.value("data").toObject();
    accessToken = objectData.value("accessToken").toString().toStdString();
    qDebug() << accessToken.data();
    getDeviceList(accessToken);
}


size_t MainWindow::writeDataCallback(char* buffer, size_t size,
                         size_t nitems, void* outstream)
{
    if (nullptr == buffer || size == 0 ||
           nullptr == outstream)
           return size * nitems;

   std::string* pstrMsg = reinterpret_cast<std::string*>(outstream);
   pstrMsg->append(buffer, size * nitems);
   return size * nitems;
}


void MainWindow::getDeviceList(const std::string& accessToken)
{
    //获取设备列表
    std::string authAddr = "https://openauth.ys7.com";
    std::string platForm = "https://open.ys7.com";
    std::string appKey = "1a2dff9fcc3d4111b1531e73bb9930e7";
    //初始化SDK
    if ( OpenSDK_InitLib(authAddr.data(), platForm.data(), appKey.data(), false) != OPEN_SDK_NOERROR)
    {
        return;
    }

    //关闭日志信息
    OpenSDK_SetConfigInfo(CONFIG_LOG_LEVEL, EZOPENSDK_OFF);
    OpenSDK_SetConfigInfo(CONFIG_DATA_UTF8, 1);
    OpenSDK_SetConfigInfo(CONFIG_OPEN_STREAMTRANS, 1);
    OpenSDK_SetConfigInfo(CONFIG_FAST_STREAM, 1);
    OpenSDK_SetConfigInfo(CONFIG_P2P_MAXNUM, 16);

    //设置token
    if (OpenSDK_SetAccessToken(accessToken.data()) != OPEN_SDK_NOERROR)
    {
        OpenSDK_FiniLib();
        return;
    }

    //获取设备列表
    void* devList = NULL;
    int devListLen = 0;
    std::string devSerial = "FU0782242";
    if(OpenSDK_Data_GetDeviceInfo(accessToken.data(), devSerial.data(), &devList, &devListLen) != OPEN_SDK_NOERROR)
    {
        OpenSDK_FiniLib();
        return;
    }

    QJsonParseError jsonError;
    QByteArray jsonData =  QByteArray(static_cast<char*>(devList), devListLen);
    QJsonDocument docDataList = QJsonDocument::fromJson(jsonData, &jsonError);  // 转化为 JSON 文档
    if (docDataList.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
        OpenSDK_Data_Free(devList);
       return;
    }

    QJsonObject objectDataList = docDataList.object();
    QJsonObject devInfo = objectDataList.value("result").toObject();
    m_devList = devInfo.value("data").toArray();
    if (m_devList.empty())
    {
        OpenSDK_Data_Free(devList);
       return;
    }

    OpenSDK_Data_Free(devList);
    emit deviceListChange(m_devList);
}

void MainWindow::slotDeviceListChange(const QJsonArray& devList)
{
    m_devList = devList;
    m_tableModel->updateModelData(devList);
}


void MainWindow::onCloseClick()
{
    close();
}


void MainWindow::onMinClick()
{
    showMinimized();
}


void MainWindow::onMaxClick()
{
    if (!m_isMaxWnd)
    {
        showMaximized();
        m_isMaxWnd = true;
    } else {
        showNormal();
        m_isMaxWnd = false;
    }

}

