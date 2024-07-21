#ifndef CITYDIALOG_H
#define CITYDIALOG_H

#include <QDialog>
#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QList>
namespace Ui {
class CityDialog;
}
enum request_type{
    request_type_none,
    request_type_province,
    request_type_city
};

class Province
{
public:
    Province(){}
    QString code;
    QString name;
    QString url;
};


class CityDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CityDialog(QWidget *parent = nullptr);
    ~CityDialog();

    void requestProvince();
    void requestCity(const QString& code);
    void clearList(QList<Province*>&);
    const QString& getCode()const;
protected slots:
    void onFinished();
    void onReadyRead();
    void onCityCurrChanged(int);
    void onProvinceCurrChanged(int);
protected:
    QString selectCityCode;
    QList<Province*> provinces;
    QList<Province*> cities;
    request_type request;
    QNetworkReply *reply;
    QNetworkAccessManager networkManager;
private:
    Ui::CityDialog *ui;
};

#endif // CITYDIALOG_H
