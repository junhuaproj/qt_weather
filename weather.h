#ifndef WEATHER_H
#define WEATHER_H


struct Weather_Station
{
    char* city;
    char* code;
    char* province;
};

void del_weather_station(struct Weather_Station& station);

struct Weather_Weather
{
    float airpressure;
    float feelst;
    float humidity;
    char* img;
    char* info;
    float rain;
    float temperature;
    float temperatureDiff;
};

void del_weather_weather(struct Weather_Weather& weather);

struct Weather_Wind
{
    float degree;
    char* direct;
    char* power;
    float speed;
};

void del_weather_wnd(struct Weather_Wind& wind);
struct Weather_Real
{
    char* publish_time;
    Weather_Station station;
    Weather_Weather weather;
    Weather_Wind wind;
};

void del_weather_real(struct Weather_Real& real);
struct Weather_Predict_Weather
{
    char* img;
    char* info;
    char* temperature;
};
void del_weather_predict_weather(struct Weather_Predict_Weather& );
struct Weather_Predict_Wind
{
    char* direct;
    char* power;
};
void del_weather_predict_wind(struct Weather_Predict_Wind& );
struct Weather_Predict_Weather_Predict
{
    Weather_Predict_Weather weather;
    Weather_Predict_Wind wind;
};
void del_weather_predict_weather_predict(struct Weather_Predict_Weather_Predict&);
struct Weather_Predict_Day
{
    char* date;
    Weather_Predict_Weather_Predict day;
    Weather_Predict_Weather_Predict night;
    float precipitation;
    char* pt;
};
void del_weather_predict_day(struct Weather_Predict_Day&);
struct Weather_Predict
{
    Weather_Station station;
    char* publish_time;
    int detail_cnt;
    Weather_Predict_Day* detail;
};
void del_weather_predict(struct Weather_Predict&);
struct Weather{
    Weather_Predict predict;
    Weather_Real real;
};

void del_weather(struct Weather& );
char* get_buf(const char* cstr,int len=-1);


#include <QString>
#include <QJsonArray>
#include <QJsonObject>
#include <QJsonDocument>

char* get_buf(const QJsonValue& value);
void str_weather_station(const struct Weather_Station& station,QString& out);
void str_weather_station(const struct Weather_Station& station,QString& out);

extern const char* weather_date_format;
class WeatherParser
{
public:
    WeatherParser(Weather* weather);
    int parser(const QString& filePath);
    int parser(const QByteArray& bytes);
    int parser(const QJsonDocument& doc);
    int parser(const QJsonObject& obj);

    int parser_real(const QJsonObject& obj,struct Weather_Real*);
    int parser_station(const QJsonObject& obj,struct Weather_Station*);
    int parser_weather(const QJsonObject& obj,struct Weather_Weather*);
    int parser_wind(const QJsonObject& obj,struct Weather_Wind*);
    int praser_predict(const QJsonObject& obj, struct Weather_Predict*);
    int parser_predict_day(const QJsonObject& obj, struct Weather_Predict_Day*);
    int parser_predict_day_day(const QJsonObject& obj, struct Weather_Predict_Weather_Predict*);
    int parser_predict_day_weather(const QJsonObject& obj, struct Weather_Predict_Weather*);
    int parser_predict_day_wind(const QJsonObject& obj, struct Weather_Predict_Wind*);
protected:
    Weather* weather;
};

#endif // WEATHER_H
