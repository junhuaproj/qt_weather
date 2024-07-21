#include "citydialog.h"
#include "ui_citydialog.h"
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

CityDialog::CityDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::CityDialog)
{
    ui->setupUi(this);
    connect(ui->listWidget_province,SIGNAL(currentRowChanged(int)),this,SLOT(onProvinceCurrChanged(int)));
    connect(ui->listWidget_city,SIGNAL(currentRowChanged(int)),this,SLOT(onCityCurrChanged(int)));
    request=request_type_none;
    requestProvince();
}

CityDialog::~CityDialog()
{
    delete ui;
    clearList(cities);
    clearList(provinces);
}
const QString& CityDialog::getCode()const
{
    return selectCityCode;
}
void CityDialog::onCityCurrChanged(int curr)
{
    if(curr<0)return;
    QListWidgetItem* item=(QListWidgetItem*)ui->listWidget_city->item(curr);
    Province* pro=(Province*)qvariant_cast<uint64_t>(item->data(Qt::UserRole));
    selectCityCode=pro->code;
}
void CityDialog::onProvinceCurrChanged(int curr)
{
    if(curr<0)return;
    QListWidgetItem* item=(QListWidgetItem*)ui->listWidget_province->item(curr);
    Province* pro=(Province*)qvariant_cast<uint64_t>(item->data(Qt::UserRole));
    requestCity(pro->code);
}
void CityDialog::requestProvince()
{
    QString url="http://www.nmc.cn/rest/province/all";
    reply=networkManager.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(onFinished()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    request=request_type_province;
}
void CityDialog::clearList(QList<Province*>& list)
{
    while(!list.isEmpty())
    {
        delete list.back();
        list.pop_back();
    }
}
void CityDialog::onFinished()
{
    reply->deleteLater();
    reply=NULL;
}
void CityDialog::onReadyRead()
{
    QByteArray bytes= reply->readAll();
    QJsonDocument doc=QJsonDocument::fromJson(bytes);
    if(!doc.isArray())return;
    QJsonArray arr= doc.array();
    int count=arr.size();

    QListWidgetItem* item;
    if(request==request_type_province)
    {
        Province* pro;
        clearList(provinces);
        ui->listWidget_province->clear();

        for(int i=0;i<count;i++)
        {
            QJsonObject obj=arr.at(i).toObject();
            pro=new Province();
            pro->code=obj.value("code").toString();
            pro->name=obj.value("name").toString();
            pro->url=obj.value("url").toString();
            provinces.append(pro);
            item=new QListWidgetItem(pro->name);
            item->setData(Qt::UserRole,(qlonglong)pro);
            ui->listWidget_province->addItem(item);
        }
    }
    else if(request==request_type_city)
    {
        clearList(cities);
        Province* pro;
        ui->listWidget_city->clear();

        for(int i=0;i<count;i++)
        {
            QJsonObject obj=arr.at(i).toObject();
            pro=new Province();
            pro->code=obj.value("code").toString();
            pro->name=obj.value("city").toString();
            pro->url=obj.value("url").toString();
            cities.append(pro);
            item=new QListWidgetItem(pro->name);
            item->setData(Qt::UserRole,(qlonglong)pro);
            ui->listWidget_city->addItem(item);
        }
    }
}
void CityDialog::requestCity(const QString& code)
{
    QString url=QString("http://www.nmc.cn/rest/province/%1").arg(code);
    reply=networkManager.get(QNetworkRequest(url));
    connect(reply,SIGNAL(finished()),this,SLOT(onFinished()));
    connect(reply,SIGNAL(readyRead()),this,SLOT(onReadyRead()));
    request=request_type_city;
}
