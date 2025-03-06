#ifndef CUSTOMHEADERVIEW_H
#define CUSTOMHEADERVIEW_H

#include <QHeaderView>

class CustomHeaderView : public QHeaderView
{

public:
    explicit CustomHeaderView(Qt::Orientation orientation, QWidget *parent = 0);
    QSize sizeHint() const override;

protected:
       void paintSection(QPainter *painter, const QRect &rect, int logicalIndex) const override;
       void mousePressEvent(QMouseEvent *event) override;
       void mouseMoveEvent(QMouseEvent *event) override;
};

#endif // CUSTOMHEADERVIEW_H
