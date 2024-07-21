
#include <stdlib.h>
#include <string.h>
#include "weather.h"

#include <QFile>

const char* weather_date_format="yyyy-MM-dd";
WeatherParser::WeatherParser(Weather* w)
    :weather(w)
{

}

int WeatherParser::parser(const QString& filePath)
{
    QFile f(filePath);
    if(!f.open(QFile::ReadOnly))
        return 0;
    QByteArray arr=f.readAll();
    f.close();
    return parser(arr);
}
int WeatherParser::parser(const QByteArray& bytes)
{
    QJsonDocument doc=QJsonDocument::fromJson(bytes);
    if(!doc.isEmpty())
        return parser(doc);
    return 0;
}
int WeatherParser::parser(const QJsonDocument& doc)
{
    if(!doc.isObject())
        return 0;
    return parser(doc.object());
}
int WeatherParser::parser_station(const QJsonObject& obj,struct Weather_Station* station)
{
    station->city=get_buf(obj.value("city"));
    station->code=get_buf(obj.value("code"));
    station->province=get_buf(obj.value("province"));
    return 1;
}

int WeatherParser::parser_weather(const QJsonObject& obj,struct Weather_Weather* weather)
{
    weather->airpressure=obj.value("airpressure").toDouble();
    weather->feelst=obj.value("feelst").toDouble();
    weather->humidity=obj.value("humidity").toDouble();
    weather->rain=obj.value("rain").toDouble();
    weather->temperature=obj.value("temperature").toDouble();
    weather->temperatureDiff=obj.value("temperatureDiff").toDouble();

    weather->img=get_buf(obj.value("img"));
    weather->info=get_buf(obj.value("info"));
    return 1;
}

int WeatherParser::parser_wind(const QJsonObject& obj,struct Weather_Wind* wind)
{
    wind->degree=obj.value("degree").toDouble();
    wind->speed=obj.value("speed").toDouble();
    wind->direct=get_buf(obj.value("direct"));
    wind->power=get_buf(obj.value("power"));
    return 1;
}

int WeatherParser::parser_real(const QJsonObject& obj,struct Weather_Real* real)
{
    real->publish_time=get_buf(obj.value("publish_time"));
    QJsonValue value=obj.value("station");
    if(value.isObject())
    {
        parser_station(value.toObject(),&real->station);
    }
    value=obj.value("weather");
    if(value.isObject())
    {
        parser_weather(value.toObject(),&real->weather);
    }
    value=obj.value("wind");
    if(value.isObject())
    {
        parser_wind(value.toObject(),&real->wind);
    }
    return 1;
}

int WeatherParser::parser_predict_day_weather(const QJsonObject& obj, struct Weather_Predict_Weather* weather)
{
    weather->img=get_buf(obj.value("img"));
    weather->info=get_buf(obj.value("info"));
    weather->temperature=get_buf(obj.value("temperature"));
    return 1;
}
int WeatherParser::parser_predict_day_wind(const QJsonObject& obj, struct Weather_Predict_Wind* wind)
{
    wind->direct=get_buf(obj.value("direct"));
    wind->power=get_buf(obj.value("power"));
    return 1;
}

int WeatherParser::parser_predict_day_day(const QJsonObject& obj, struct Weather_Predict_Weather_Predict* weather)
{
    QJsonValue value=obj.value("weather");
    if(value.isObject())
        parser_predict_day_weather(value.toObject(),&weather->weather);

    value=obj.value("wind");
    if(value.isObject())
        parser_predict_day_wind(value.toObject(),&weather->wind);
    return 1;
}

int WeatherParser::parser_predict_day(const QJsonObject& obj, struct Weather_Predict_Day* day)
{
    day->date=get_buf(obj.value("date"));
    day->precipitation=obj.value("precipitation").toDouble();
    day->pt=get_buf(obj.value("pt"));
    QJsonValue value=obj.value("day");
    if(value.isObject())
        parser_predict_day_day(value.toObject(),&day->day);
    value=obj.value("night");
    if(value.isObject())
        parser_predict_day_day(value.toObject(),&day->night);
    return 1;
}
int WeatherParser::praser_predict(const QJsonObject& obj, struct Weather_Predict* predict)
{
    predict->publish_time=get_buf(obj.value("publish_time"));
    QJsonValue value=obj.value("station");
    if(value.isObject())
    {
        parser_station(value.toObject(),&predict->station);
    }
    value=obj.value("detail");
    if(value.isArray())
    {
        QJsonArray arr=value.toArray();
        predict->detail_cnt=arr.size();
        predict->detail=(struct Weather_Predict_Day*)malloc(sizeof(struct Weather_Predict_Day)*predict->detail_cnt);
        memset(predict->detail,0,sizeof(struct Weather_Predict_Day)*predict->detail_cnt);
        for(int i=0;i<predict->detail_cnt;i++)
        {
            parser_predict_day(arr.at(i).toObject(),predict->detail+i);
        }
    }
    return 1;
}
int WeatherParser::parser(const QJsonObject& obj)
{
    QJsonValue value=obj.value("real");
    if(value.isObject())
    {
        parser_real(value.toObject(),&weather->real);
    }
    else
    {
        value=obj.value("data");
        if(value.isObject())
            return parser(value.toObject());
        return 0;
    }
    value=obj.value("predict");
    if(value.isObject())
    {
        praser_predict(value.toObject(),&weather->predict);
    }
    else return 0;
    return 1;
}

void del_weather_station(struct Weather_Station& station)
{
    if(station.city)
        free(station.city);
    if(station.code)
        free(station.code);
    if(station.province)
        free(station.province);
    memset(&station,0,sizeof(struct Weather_Station));
}

void del_weather_weather(struct Weather_Weather& weather)
{
    if(weather.img)
        free(weather.img);
    if(weather.info)
        free(weather.info);
    memset(&weather,0,sizeof(struct Weather_Weather));
}
void del_weather_wnd(struct Weather_Wind& wind)
{
    if(wind.direct)
        free(wind.direct);
    if(wind.power)
        free(wind.power);
    memset(&wind,0,sizeof(struct Weather_Wind));
}

void del_weather_real(struct Weather_Real& real)
{
    if(real.publish_time)
    {
        free(real.publish_time);
        real.publish_time=NULL;
    }
    del_weather_station(real.station);
    del_weather_weather(real.weather);
    del_weather_wnd(real.wind);
    memset(&real,0,sizeof(struct Weather_Real));
}
void del_weather_predict_weather(struct Weather_Predict_Weather& weather)
{
    if(weather.img)
        free(weather.img);
    if(weather.info)
        free(weather.info);
    if(weather.temperature)
        free(weather.temperature);
    memset(&weather,0,sizeof(struct Weather_Predict_Weather));
}
void del_weather_predict_wind(struct Weather_Predict_Wind& wind)
{
    if(wind.direct)
        free(wind.direct);
    if(wind.power)
        free(wind.power);
    memset(&wind,0,sizeof(struct Weather_Predict_Wind));
}

void del_weather_predict_weather_predict(struct Weather_Predict_Weather_Predict& predict)
{
    del_weather_predict_weather(predict.weather);
    del_weather_predict_wind(predict.wind);
    memset(&predict,0,sizeof(struct Weather_Predict_Weather_Predict));
}
void del_weather_predict_day(struct Weather_Predict_Day& day)
{
    if(day.date)
        free(day.date);
    if(day.pt)
        free(day.pt);
    del_weather_predict_weather_predict(day.day);
    del_weather_predict_weather_predict(day.night);
    memset(&day,0,sizeof(struct Weather_Predict_Day));
}

void del_weather_predict(struct Weather_Predict& predict)
{
    del_weather_station(predict.station);
    if(predict.publish_time)
        free(predict.publish_time);
    if(predict.detail)
    {
        for(int i=0;i<predict.detail_cnt;i++)
        {
            del_weather_predict_day(predict.detail[i]);
        }
        free(predict.detail);
    }
    memset(&predict,0,sizeof(struct Weather_Predict));
}
void del_weather(struct Weather& weather)
{
    del_weather_real(weather.real);
    del_weather_predict(weather.predict);
}

char* get_buf(const char* cstr,int len)
{
    if(len<0)
        len=strlen(cstr);
    char* p=(char*)malloc(len+1);
    strncpy(p,cstr,len);
    p[len]=0;
    return p;
}
char* get_buf(const QJsonValue& value)
{
    std::string s=value.toString().toStdString();
    return get_buf(s.c_str(),s.length());
}
void str_weather_station(const struct Weather_Station& station,QString& out)
{
    out=QString("%1%2").arg(station.province,station.city);
}
