#ifndef PREDICTWND_H
#define PREDICTWND_H

#include <QMainWindow>
#include "weather.h"
#include <QTableWidgetItem>

namespace Ui {
class PredictWnd;
}

class PredictWnd : public QMainWindow
{
    Q_OBJECT

public:
    explicit PredictWnd(QLocale* locale,Weather_Predict* predict,QWidget *parent = nullptr);
    ~PredictWnd();

protected:
    Weather_Predict* predict;

    void updatePredict();
    QLocale* locale;
    QTableWidgetItem* getItem(int row,int col);
    void setItemIcon(int row,int col,const char* img);

protected slots:
    void onQss(bool);
private:
    Ui::PredictWnd *ui;
};

#endif // PREDICTWND_H
