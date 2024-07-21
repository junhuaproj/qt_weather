#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "predictwnd.h"
#include "citydialog.h"
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
,locale(QLocale::Chinese,QLocale::China)
{
    ui->setupUi(this);
    QLocale currLocal=QLocale::system();

    QLocale::setDefault(locale);

    memset(&weather,0,sizeof(struct Weather));
    WeatherParser parser(&weather);
    QString path="./weather.json";
    //styledDelegate=new QWeatherStyledItemDelegate(ui->listWidget_predict);
    //ui->listWidget_predict->setItemDelegate(styledDelegate);
    parser.parser(path);
    update();
    timer.setInterval(200);
    connect(&timer,SIGNAL(timeout()),this,SLOT(onTimer()));
    timer.start();
    connect(ui->pushButton_predict,SIGNAL(clicked(bool)),this,SLOT(onPredict(bool)));
    connect(ui->pushButton_city,SIGNAL(clicked(bool)),this,SLOT(onCity(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    del_weather(weather);
}
void MainWindow::onNetFinish()
{
    reply->deleteLater();
    reply=NULL;
}
void MainWindow::onNetReadyRead()
{
    QByteArray bytes=reply->readAll();
    QJsonDocument doc=QJsonDocument::fromJson(bytes);
    if(!doc.isObject())return;
    WeatherParser parser(&weather);
    parser.parser(doc);
    update();
}

void MainWindow::requestWeather(const QString& code)
{
    QDateTime curDate=QDateTime::currentDateTime();
    QString url=QString("http://www.nmc.cn/rest/weather?stationid=%1&_=%2")
                      .arg(code).arg(curDate.toMSecsSinceEpoch());
    reply=networkManager.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(onNetFinish()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onNetReadyRead()));
}

void MainWindow::onCity(bool)
{
    CityDialog* dlg=new CityDialog(this);
    if(QDialog::Accepted==dlg->exec())
    {
        requestWeather(dlg->getCode());
    }
    delete dlg;
}

void MainWindow::onTimer()
{
    QDate date=QDate::currentDate();

    QString out=date.toString(tr("yyyy/MM"));
    ui->label_yymm->setText(out);

    out=date.toString(tr("dd"));
    ui->label_day->setText(out);
    out=locale.toString(date,tr("ddd"));
    ui->label_week->setText(out);

    QTime time=QTime::currentTime();
    out=time.toString(tr("hh:mm:ss"));
    ui->label_time->setText(out);
}

void MainWindow::update()
{
    QString out;
    str_weather_station(weather.real.station,out);
    ui->label_Station->setText(out);
    updateReal();
    updatePredict();
}
void MainWindow::updateReal()
{
    struct Weather_Real* real=&weather.real;
    //ui->listWidget_Real->addItem(real->publish_time);
    QPixmap pixmap;
    QString iconPath=QString(tr("/home/wang/qtproj/weather-02/icon/%1.png")).arg(real->weather.img);
    //pixmap.load("C:\\app\\qtapp\\weather-02\\icon\\0.png");
    if(pixmap.load(iconPath))
        ui->label_img->setPixmap(iconPath);
    //QString out;
    //str_weather_station(real->station,out);
    //ui->listWidget_Real->addItem(out);

    ui->label_temp->setText(QString("%1℃").arg(real->weather.temperature));
    ui->label_weather->setText(real->weather.info);
    ui->label_feelst->setText(QString("feelst\n%1℃").arg(real->weather.feelst));
    ui->label_humidity->setText(QString("humidity\n%1").arg(real->weather.humidity));
    ui->label_wind->setText(QString("%1\n%2").arg(real->wind.direct).arg(real->wind.power));
    ui->label_rain->setText(QString(tr("rain\n%1")).arg(real->weather.rain));

    //out=QString("%1").arg(real->weather.info);
    //ui->listWidget_Real->addItem(out);
}
void MainWindow::onPredict(bool)
{
    PredictWnd* wnd=new PredictWnd(&locale,&weather.predict);
    wnd->show();
}
void MainWindow::updatePredict()
{
    Weather_Predict* predict=&weather.predict;
    //ui->listWidget_predict->addItem(predict->publish_time);
    QString out;
    str_weather_station(predict->station,out);
    //ui->listWidget_predict->addItem(out);
    /*QListWidgetItem* item;
    for(int i=0;i<predict->detail_cnt;i++)
    {
        item=new QListWidgetItem();
        item->setData(PREDICT_DAY_DATA,(uint64_t)(predict->detail+i));
        ui->listWidget_predict->addItem(item);
    }*/
}
