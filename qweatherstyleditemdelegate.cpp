#include "qweatherstyleditemdelegate.h"
#include <QPainter>
#include "weather.h"

QWeatherStyledItemDelegate::QWeatherStyledItemDelegate(QObject *parent)
    :QStyledItemDelegate(parent)
{

}
void QWeatherStyledItemDelegate::paint(QPainter *painter, const QStyleOptionViewItem &option,
                                       const QModelIndex &index) const
{
    QRect rect = option.rect; // 目标矩形
    QFont font(tr("宋体"),11,600);
    painter->save();
    painter->setFont(font);
    rect.adjust(2,2,-2,-2);
    if(option.state & QStyle::State_Selected)
    {
        painter->setBrush(QColor(0x08, 0x62, 0xAD));
        painter->drawRoundedRect(rect, 2, 2);
        painter->setBrush(Qt::NoBrush);
    }
    QVariant var=index.data(PREDICT_DAY_DATA);
    Weather_Predict_Day* pDay=(Weather_Predict_Day*)qvariant_cast<uint64_t>(var);

    if(pDay)
    {
        QRect rcText=rect;
        QString text;
        QDate date=QDate::fromString(pDay->date,weather_date_format);
        const int cell_width=75;
        rcText.setWidth(cell_width);
        //day日期
        text=date.toString("MM/dd\nddd");
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,text);
        //day weather
        rcText.moveLeft(rcText.left()+cell_width);
        text=QString("%1").arg(pDay->day.weather.info);
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,text);
        //daywind
        rcText.moveLeft(rcText.left()+cell_width);
        text=QString("%1\n%2").arg(pDay->day.wind.direct,pDay->day.wind.power);
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,text);

        //daytemperature
        rcText.moveLeft(rcText.left()+cell_width);
        text=QString("%1℃").arg(pDay->day.weather.temperature);
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,text);
        //night
        //temperature
        rcText.moveLeft(rcText.left()+cell_width);
        text=QString("%1℃").arg(pDay->night.weather.temperature);
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,text);
        //weather
        rcText.moveLeft(rcText.left()+cell_width);
        //text=QString("%1℃").arg(pDay->night.weather.temperature);
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,pDay->night.weather.info);
        //wind
        rcText.moveLeft(rcText.left()+cell_width);
        text=QString("%1\n%2").arg(pDay->night.wind.direct,pDay->night.wind.power);
        painter->drawText(rcText,Qt::AlignLeft|Qt::AlignVCenter,text);
    }
    painter->restore();
}
//决定单元格的推荐大小
QSize QWeatherStyledItemDelegate::sizeHint(const QStyleOptionViewItem &option, const QModelIndex &index) const
{
    QSize size=QStyledItemDelegate::sizeHint(option,index);
    size.setHeight(85);
    return size;
}
