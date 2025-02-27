#include "FullScreenDialog.h"
#include "ui_FullScreenDialog.h"
#include <QDebug>
#include <QMenu>
#include <QAction>
#include <QtMath>
#include "VideoFrame.h"


FullScreenDialog::FullScreenDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FullScreenDialog)
{
    ui->setupUi(this);
    resize(1920, 1080);
    setAttribute(Qt::WA_DeleteOnClose);
    initControls();
}


FullScreenDialog::~FullScreenDialog()
{
    qDebug() << "FullScreenDialog::~FullScreenDialog";
    delete ui;
}


void FullScreenDialog::setDevList(int index, const QString& sessionId, QJsonArray* devArray)
{
    m_devArray = devArray;
    m_currIndex = index;
    m_sessionId = sessionId;

    QJsonObject item = devArray->at(index).toObject();
    qDebug() << item.value("cameraNo").toInt();
}


void FullScreenDialog::initControls()
{
    m_vboxMain.setMargin(0);
    m_vboxMain.setSpacing(0);

    m_gridLayout.setMargin(0);
    m_gridLayout.setSpacing(0);

    m_hboxMenu.setMargin(0);
    m_hboxMenu.setSpacing(0);
    m_hboxMenu.addStretch(1);

    m_btnVideoCount.setMinimumSize(40, 40);
    m_btnVideoCount.setCursor(Qt::PointingHandCursor);
    m_btnVideoCount.setStyleSheet("background:transparent;border:none");
    m_btnVideoCount.setIcon(QIcon(":/res/videocount.png"));
    m_btnVideoCount.setIconSize(QSize(20, 20));
    connect(&m_btnVideoCount, &QPushButton::clicked, this, &FullScreenDialog::onVideoCountChange);

    m_btnExitFull.setMinimumSize(40, 40);
    m_btnExitFull.setCursor(Qt::PointingHandCursor);
    m_btnExitFull.setStyleSheet("background:transparent;border:none");
    m_btnExitFull.setIcon(QIcon(":/res/exit_fullscreen.png"));
    m_btnExitFull.setIconSize(QSize(20, 20));
    connect(&m_btnExitFull, &QPushButton::clicked, this, &FullScreenDialog::onExitFullScreen);

    m_hboxMenu.addWidget(&m_btnVideoCount);
    m_hboxMenu.addWidget(&m_btnExitFull);

    m_frameVideos = new QFrame(this);
    m_frameVideos->setMinimumSize(width(), height() - 40);
    m_frameVideos->setStyleSheet("background:black;border:none");
    m_frameVideos->setLayout(&m_gridLayout);

    m_vboxMain.addWidget(m_frameVideos);
    m_vboxMain.addLayout(&m_hboxMenu);
    setLayout(&m_vboxMain);
}

void FullScreenDialog::onExitFullScreen()
{
    done(1);
}


void FullScreenDialog::onVideoCountChange()
{
    QMenu* pMenu = new QMenu(this);
    pMenu->setStyleSheet("QMenu{background-color:#0F1C2E;border:none}"
                         "QMenu::item{padding:11px 35px;color:white;font:15px}"
                         "QMenu::item:hover{background:#0F1C2E}"
                         "QMenu::item:selected{background:#374357}");

    QAction* pOneVideo = new QAction("1画面", this);
    connect(pOneVideo, &QAction::triggered, this, &FullScreenDialog::onActionOneVideo);
    QAction* pFourVideo = new QAction("4画面", this);
    connect(pFourVideo, &QAction::triggered, this, &FullScreenDialog::onActionFourVideo);
    QAction* pNineVideo = new QAction("9画面", this);
    connect(pNineVideo, &QAction::triggered, this, &FullScreenDialog::onActionNineVideo);
    QAction* pSixteenVideo = new QAction("16画面", this);
    connect(pSixteenVideo, &QAction::triggered, this, &FullScreenDialog::onActionSixteenVideo);

    pMenu->addAction(pOneVideo);
    pMenu->addAction(pFourVideo);
    pMenu->addAction(pNineVideo);
    pMenu->addAction(pSixteenVideo);
    pMenu->exec(QPoint(cursor().pos().x() - 50 , cursor().pos().y() - 170));
    QList<QAction*> list = pMenu->actions();
    foreach (QAction* pAction, list) delete pAction;
    delete pMenu;
}


void FullScreenDialog::onActionOneVideo()
{
    createVideos(1);
}


void FullScreenDialog::onActionFourVideo()
{
    createVideos(4);
}


void FullScreenDialog::onActionNineVideo()
{
    createVideos(9);
}


void FullScreenDialog::onActionSixteenVideo()
{
    createVideos(16);
}


void FullScreenDialog::createVideos(int videoCount)
{
//    m_currVideoCount = videoCount;
//    int streamType = 0;
//    if (videoCount == 1)
//    {
//        m_currVideoIndex = 0;;
//        QString strPageNum = QString("%1/%2").arg(QString::number(m_currVideoIndex + 1)).
//                arg(QString::number(m_listChanInfo.size()));
//        m_labPageNum->setText(strPageNum);
//    } else {
//        if (m_listChanInfo.size() > videoCount)
//        {
//            m_currVideoIndex = videoCount - 1;
//        } else {
//            m_currVideoIndex = m_listChanInfo.size() - 1;

//        }

//        QString strPageNum = QString("%1/%2").arg(QString::number(qCeil((double)m_currVideoIndex / m_currVideoCount)),
//                                                 QString::number(qCeil((double)m_listChanInfo.size() / m_currVideoCount)));
//        m_labPageNum->setText(strPageNum);
//        streamType = 1;
//    }

    int iLineNum = static_cast<int>(qSqrt(videoCount)) ;
    int iWidth = width();
    int iHeight = height() - 40;
    int iPlayWndWidth  = iWidth / iLineNum;
    int iPlayWndHeight = iHeight / iLineNum;
    //m_playFrame.clear();
    clearGridLayout();
    //创建播放框架
    for (int i = 0; i < videoCount; i++)
    {
        VideoFrame* playFrameWnd = new VideoFrame();
        playFrameWnd->setSize(iPlayWndWidth, iPlayWndHeight);
        if (m_devArray->size() >= i + 1 )
        {
            QJsonObject item = m_devArray->at(i).toObject();
            QString serialNum = item.value("deviceSerial").toString();
            int channelNo = item.value("cameraNo").toInt();
            playFrameWnd->startPlay(m_sessionId, serialNum, channelNo);
        }

        m_gridLayout.addWidget(playFrameWnd,  (i / iLineNum) * iPlayWndHeight,
                                 (i % iLineNum) * iPlayWndWidth, iPlayWndHeight, iPlayWndWidth);
        //connect(playFrameWnd, &PlayFrameWnd::frameDbClick, this, &PreviewForm::onFrameDoubleClick);
        //m_playFrame.push_back(playFrameWnd);
    }
}


void FullScreenDialog::clearGridLayout()
{
    if (m_gridLayout.rowCount() == 0 ||
            m_gridLayout.columnCount() == 0)
    {
        return;
    }

    QLayoutItem *child;
    while ((child = m_gridLayout.takeAt(0)) != nullptr) {
        if(child->widget()) {
            child->widget()->setParent(nullptr);
            delete child->widget();
        }

        delete child;
    }
}
