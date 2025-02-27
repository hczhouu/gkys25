#ifndef TEXTITEMDELEGATE_H
#define TEXTITEMDELEGATE_H

#include <QStyledItemDelegate>

class TextItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    explicit TextItemDelegate(QObject *parent = nullptr);
    ~TextItemDelegate();

    void paint(QPainter *painter,
               const QStyleOptionViewItem &option, const QModelIndex &index) const override;

    bool editorEvent(QEvent *event, QAbstractItemModel *model,
                     const QStyleOptionViewItem &option, const QModelIndex &index) override;

    QWidget *createEditor(QWidget *parent,
                          const QStyleOptionViewItem &option,
                          const QModelIndex &index) const override;

    void setEditorData(QWidget *editor, const QModelIndex &index) const override;
    void setModelData(QWidget *editor,
                      QAbstractItemModel *model,
                      const QModelIndex &index) const override;

    void updateEditorGeometry(QWidget *editor,
                              const QStyleOptionViewItem &option,
                              const QModelIndex &index) const override;

signals:
    void itemDbClick(int index);
};

#endif // TEXTITEMDELEGATE_H
