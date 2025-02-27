#include "TextItemDelegate.h"
#include <QPainter>
#include <QLineEdit>
#include <QMouseEvent>
#include <QDebug>

TextItemDelegate::TextItemDelegate(QObject *parent) : QStyledItemDelegate(parent)
{

}


TextItemDelegate::~TextItemDelegate()
{

}


void TextItemDelegate::paint(QPainter *painter,const QStyleOptionViewItem &option,
                             const QModelIndex &index) const
{

    QRect retc = option.rect;
    QStyleOptionViewItem viewoption(option);
    initStyleOption(&viewoption, index);
    if(option.state.testFlag(QStyle::State_HasFocus))
    {
        viewoption.state = viewoption.state ^ QStyle::State_HasFocus;
    }

    QString textContent = index.model()->data(index, Qt::DisplayRole).toString();
    //画文本内容
    painter->save();
    QFont font;
    font.setPixelSize(16);
    font.setBold(true);
    painter->setFont(font);
    QRect txtRec(retc.left(), retc.top(), retc.width(), retc.height());
    QRect bkRec(retc.left(), retc.top(), retc.width(), retc.height());
    if (index.row() % 2 == 0)
    {
        painter->fillRect(bkRec, QBrush(QColor("#169ca5")));
    } else {
        painter->fillRect(bkRec, QBrush(QColor("#1badb0")));
    }

    painter->drawText(txtRec, Qt::AlignCenter, textContent);
    painter->restore();
}



void TextItemDelegate::updateEditorGeometry(QWidget *editor, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    editor->setGeometry(option.rect);
}


bool TextItemDelegate::editorEvent(QEvent *event, QAbstractItemModel *model,
                 const QStyleOptionViewItem &option, const QModelIndex &index)
{
    if (event->type() == QEvent::MouseButtonDblClick)
    {
        emit itemDbClick(index.row());
    }

    return QStyledItemDelegate::editorEvent(event, model, option, index);
}


QWidget* TextItemDelegate::createEditor(QWidget *parent, const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QLineEdit* lineEdit = new QLineEdit(parent);
    //lineEdit->setFixedSize(parent->width(),parent->height());
    lineEdit->setStyleSheet("background:gray;border:none;color:white");
    QFont font;
    font.setFamily(u8"微软雅黑");
    font.setPixelSize(16);
    //font.setBold(true);
    lineEdit->setFont(font);
    lineEdit->setAlignment(Qt::AlignCenter);
    return lineEdit;
}

void TextItemDelegate::setEditorData(QWidget *editor, const QModelIndex &index) const
{
    if (QLineEdit *lineEdit = qobject_cast<QLineEdit*>(editor)) {

        lineEdit->setText(index.data(Qt::EditRole).toString());
        return;
    }

    return QStyledItemDelegate::setEditorData(editor, index);
}


void TextItemDelegate::setModelData(QWidget *editor, QAbstractItemModel *model, const QModelIndex &index) const
{
    QString value = qobject_cast<QLineEdit*>(editor)->text();
    model->setData(index, value, Qt::EditRole);
}
