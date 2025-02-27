#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QHeaderView>

class CustomHeaderView : public QHeaderView
{

public:
    explicit CustomHeaderView(Qt::Orientation orientation, QWidget *parent = 0);
    QSize sizeHint() const override;
protected:

       void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const;

       void mousePressEvent(QMouseEvent *event);

       void mouseMoveEvent(QMouseEvent *event);

};

#endif // CUSTOMHEADERVIEW_H
