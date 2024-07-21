#include "predictwnd.h"
#include "ui_predictwnd.h"
#include <QLabel>
#include <QDebug>
#include <QFile>
PredictWnd::PredictWnd(QLocale* loc,Weather_Predict* pre,QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::PredictWnd)
    ,predict(pre)
    ,locale(loc)
{
    ui->setupUi(this);
    ui->tableWidget->setColumnCount(predict->detail_cnt);
    const int row_count=12;
    ui->tableWidget->setRowCount(row_count);
    ui->tableWidget->setIconSize(QSize(45,45));
    updatePredict();
    connect(ui->pushButton_r,SIGNAL(clicked(bool)),this,SLOT(onQss(bool)));
    /*for(int i=0;i<row_count;i++)
    {
        ui->tableWidget->setRowHeight(i,32);
    }*/
}

PredictWnd::~PredictWnd()
{
    delete ui;
}
QTableWidgetItem* PredictWnd::getItem(int row,int col)
{
    QTableWidgetItem* item=ui->tableWidget->item(row,col);
    if(item==NULL)
    {
        item=new QTableWidgetItem();
        item->setTextAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
        ui->tableWidget->setItem(row,col,item);
    }
    return item;
}

void PredictWnd::setItemIcon(int row,int col,const char* img)
{
    QPixmap pixmap;
    QString iconPath=QString(tr("/home/wang/qtproj/weather-02/icon/%1.png")).arg(img);
    //pixmap.load("C:\\app\\qtapp\\weather-02\\icon\\0.png");
    if(pixmap.load(iconPath))
    {
        //QIcon icon;
        //icon.addFile(tr("C:\\app\\qtapp\\weather-02\\icon\\0.png"),QSize(40,40));
        QLabel* label=dynamic_cast<QLabel*>(ui->tableWidget->cellWidget(row,col));
        if(label)
            label->setPixmap(pixmap);
        else
        {
            label=new QLabel(ui->tableWidget);
            label->setAlignment(Qt::AlignHCenter|Qt::AlignVCenter);
            label->setPixmap(pixmap);
            ui->tableWidget->setCellWidget(row,col,label);
        }
    }
    else
    {
        qDebug()<<iconPath;
    }
}

void PredictWnd::onQss(bool)
{
    QString qssPath=tr("C:\\app\\qtapp\\weather-02\\QTableWidget.qss");
    QFile f(qssPath);
    if(f.open(QFile::ReadOnly))
    {
        QByteArray bytes=f.readAll();
        f.close();
        ui->tableWidget->setStyleSheet(QString(bytes));
    }
}
void PredictWnd::updatePredict()
{
    Weather_Predict_Day* detail=predict->detail;
    QTableWidgetItem* item;
    QDate date;
    //QString iconPath;
    //ui->tableWidget->verticalHeader()->set

    for(int i=0;i<predict->detail_cnt;i++,detail++)
    {
        date=QDate::fromString(detail->date,"yyyy-MM-dd");
        item=getItem(0,i);
        item->setText(date.toString(tr("MM/dd")));

        item=getItem(1,i);
        item->setText(locale->toString(date,"ddd"));
        //icon
        //item=getItem(2,i);
        //QPixmap pixmap;
        //iconPath=QString(tr("F:\\weather\\icon\\%1.png")).arg(detail->day.weather.img);
        setItemIcon(2,i,detail->day.weather.img);
        //item->setIcon(icon);

        //
        item=getItem(3,i);
        item->setText(detail->day.weather.info);
        item=getItem(4,i);
        item->setText(detail->day.wind.direct);
        item=getItem(5,i);
        item->setText(detail->day.wind.power);
        item=getItem(6,i);
        item->setText(detail->day.weather.temperature);

        item=getItem(7,i);
        item->setText(detail->night.weather.temperature);
        //icon
        setItemIcon(8,i,detail->night.weather.img);
        //
        item=getItem(9,i);
        item->setText(detail->night.weather.info);
        item=getItem(10,i);
        item->setText(detail->night.wind.direct);
        item=getItem(11,i);
        item->setText(detail->night.wind.power);
    }
    ui->tableWidget->setRowHeight(2,45);
    ui->tableWidget->setRowHeight(8,45);
}
