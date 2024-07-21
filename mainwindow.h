#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QNetworkReply>
#include <QNetworkAccessManager>

#include "qweatherstyleditemdelegate.h"
#include "weather.h"
#include <QTimer>
QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

    void update();
    void updateReal();
    void updatePredict();

    void requestWeather(const QString& code);
protected slots:
    void onPredict(bool);
    void onCity(bool);
    void onTimer();

    void onNetFinish();
    void onNetReadyRead();
protected:
    QNetworkReply *reply;
    QNetworkAccessManager networkManager;

    QLocale locale;
    QTimer timer;
    //QWeatherStyledItemDelegate* styledDelegate;
    Weather weather;
private:
    Ui::MainWindow *ui;
};
#endif // MAINWINDOW_H
