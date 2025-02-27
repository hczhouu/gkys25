#include "CustomHeaderView.h"
#include <QPainter>

CustomHeaderView::CustomHeaderView(Qt::Orientation orientation, QWidget *parent)
       : QHeaderView(orientation, parent)
{
}



void CustomHeaderView::paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const
{
       painter->save();
       QHeaderView::paintSection(painter, rect, logicalIndex);
       painter->restore();

       QString strText;

       switch (logicalIndex) {
       case 0:
       {
           strText  = u8"序号";
       }
           break;
       case 1:
       {
           strText  = u8"设备名称";
       }
           break;
       case 2:
       {
           strText  = u8"序列号";
       }
           break;
       case 3:
       {
           strText  = u8"通道号";
       }
           break;
       case 4:
       {
           strText  = u8"视频级别";
       }
           break;
       default:
           break;
       }

       //画文本内容
       painter->save();
       QFont font;
       font.setPixelSize(18);
       painter->setFont(font);
       QRect txtRec(rect.left(), rect.top(), rect.width(), rect.height());
       painter->drawText(txtRec, Qt::AlignCenter, strText);
       painter->restore();
}


void CustomHeaderView::mousePressEvent(QMouseEvent *event)
{
   //表头可控制，鼠标第一个列，鼠标在矩形范围内
   QHeaderView::mousePressEvent(event);
}

void CustomHeaderView::mouseMoveEvent(QMouseEvent *event)
{
   QHeaderView::mouseMoveEvent(event);
}


QSize CustomHeaderView::sizeHint() const
{
    QSize sizeBase = QHeaderView::sizeHint();
    sizeBase.setHeight(35);
    return sizeBase;
}
