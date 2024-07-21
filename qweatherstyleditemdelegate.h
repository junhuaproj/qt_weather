#ifndef QWEATHERSTYLEDITEMDELEGATE_H
#define QWEATHERSTYLEDITEMDELEGATE_H

#include <QObject>
#include <QStyledItemDelegate>

#define PREDICT_DAY_DATA    (Qt::UserRole+1)

class QWeatherStyledItemDelegate : public QStyledItemDelegate
{
    Q_OBJECT
public:
    QWeatherStyledItemDelegate(QObject *parent = nullptr);
    virtual void paint(QPainter *painter, const QStyleOptionViewItem &option, const QModelIndex &index) const;
    //决定单元格的推荐大小
    virtual QSize sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const;
};

#endif // QWEATHERSTYLEDITEMDELEGATE_H
