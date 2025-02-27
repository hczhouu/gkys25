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


std::string accessToken = "";

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    resize(1200, 800);
    setMenuBar(nullptr);
    setStatusBar(nullptr);
    //setWindowFlags(Qt::FramelessWindowHint);
    setStyleSheet("background:#0F1C2E");
    setWindowTitle("九林酒店监控预警系统");
    initControls();

    connect(this, &MainWindow::getAccessTokenOK, this, &MainWindow::onAccessTokenOK);
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

//    m_tableView.setMinimumSize(width(), height() - 50);
//    m_tableView.setStyleSheet("background:transparent;border:none");
//    m_tableView.setContextMenuPolicy(Qt::CustomContextMenu);
//    m_tableView.setSelectionBehavior(QAbstractItemView::SelectRows);
//    m_tableView.setSelectionMode(QAbstractItemView::SingleSelection);
//    m_tableModel = new DeviceTableModel();
//    m_tableView.setModel(m_tableModel);
    initTableView();

    m_btnFullscreen.setMinimumSize(120, 40);
    m_btnFullscreen.setStyleSheet("background:red;border:none");
    connect(&m_btnFullscreen, &QPushButton::clicked, this, &MainWindow::onFullScreenClick);

    m_hboxMenu.setMargin(0);
    m_hboxMenu.setSpacing(0);
    m_hboxMenu.setAlignment(Qt::AlignVCenter);
    m_hboxMenu.addStretch(1);
    m_hboxMenu.addWidget(&m_btnFullscreen);

    m_vboxMain.addWidget(&m_tableView);
    //m_vboxMain.addLayout(&m_hboxMenu);

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
    accessToken.clear();
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
    emit getAccessTokenOK();
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


void MainWindow::onAccessTokenOK()
{
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

    //设置token
    if (OpenSDK_SetAccessToken(accessToken.data()) != OPEN_SDK_NOERROR)
    {
        OpenSDK_FiniLib();
        return;
    }

    //获取设备列表
    void* devList = NULL;
    int devListLen = 0;
    if(OpenSDK_Data_GetDevListEx(0, 1, &devList, &devListLen) != OPEN_SDK_NOERROR)
    {
        OpenSDK_FiniLib();
        return;
    }

    QString str = static_cast<char*>(devList);
    OpenSDK_Data_Free(devList);

    QJsonParseError jsonError;
    QJsonDocument doucment = QJsonDocument::fromJson(str.toUtf8(), &jsonError);  // 转化为 JSON 文档
    if (doucment.isNull() || (jsonError.error != QJsonParseError::NoError))
    {
       return;
    }

    QJsonObject object = doucment.object();
    QJsonArray arrData = object.value("data").toArray();
    if (arrData.empty())
    {
       return;
    }

    for (const QJsonValue& item : arrData)
    {
        m_devList = item.toObject().value("cameraInfo").toArray();
        m_tableModel->updateModelData(m_devList);
    }

//    for (const QJsonValue& item : arrItems)
//    {
//        QJsonObject itemObj = item.toObject();
//        QString cameraName = itemObj.value("cameraName").toString();
//        QString deviceSerial = itemObj.value("deviceSerial").toString();
//        int cameraNo = itemObj.value("cameraNo").toInt();
//        int videoLevel = itemObj.value("videoLevel").toInt();
//        qDebug() << "camName : " << deviceSerial << cameraNo << videoLevel;
//    }


//    QFile file;
//    file.setFileName("D:/3.txt");
//    if (file.open(QIODevice::WriteOnly |QIODevice::Text)){
//        QTextStream stream(&file);
//        stream<< str <<"\n";
//        file.close();
//    }


    char* sessionBuf  =  nullptr;
    int sessionLen = 0;
    OpenSDK_AllocSessionEx(messageHandler, nullptr, &sessionBuf, &sessionLen);
    m_sessionId = static_cast<char*>(sessionBuf);
    //ret = OpenSDK_FreeSession(sessionBuf);
    //qDebug() << "OpenSDK_FreeSession : " << ret;
}


void MainWindow::onFullScreenClick()
{
//    FullScreenDialog* fullscreenDlg = new FullScreenDialog(this);
//    fullscreenDlg->setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
//    fullscreenDlg->setModal(true);
//    fullscreenDlg->exec();
}


