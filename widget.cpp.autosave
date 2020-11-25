#include "widget.h"
#include "ui_widget.h"
#include "QtNetwork/QNetworkReply"
#include "QUrl"
#include "QUrlQuery"
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkRequest"
#include "QJsonDocument"
#include "QJsonArray"
#include "QJsonObject"
#include "QDateTime"


Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    clearDisplay();

    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *reply) {
            if (reply->error()) {
                qDebug() << reply->errorString();
                clearDisplay();
                ui->location->setText("Not Found");
                return;
            }

            QString jsonString = reply->readAll();

            qDebug() << jsonString;

            displayWeatherData(jsonString);
        }
    );


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_queryButton_clicked()
{
    QString query = ui->query->text();
    QUrl url("http://api.openweathermap.org/data/2.5/weather");
    QUrlQuery attributes;
    attributes.addQueryItem("q", query);
    attributes.addQueryItem("appid", "3130fe9c95bcd429a94c61a79c4f7c32");
    attributes.addQueryItem("units", "metric");
    url.setQuery(attributes);
    request.setUrl(url);
    manager->get(request);
}

void Widget::displayWeatherData(QString jsonString){
    QJsonDocument jsonResponse = QJsonDocument::fromJson(jsonString.toUtf8());
    QJsonObject data = jsonResponse.object();
    QJsonObject dataMain = data["main"].toObject();
    QJsonObject dataWeather = data["weather"].toArray().at(0).toObject();
    QJsonObject dataWind = data["wind"].toObject();
    QJsonObject dataSys = data["sys"].toObject();

    QString city = data["name"].toString();
    QString countryCode = dataSys["country"].toString();
    QString location = city.append(", ").append(countryCode);

    QString weatherCondition = dataWeather["main"].toString();

    int temp = dataMain["temp"].toDouble();
    int feelsLike = dataMain["feels_like"].toDouble();
    int tempMax = dataMain["temp_max"].toDouble();
    int tempMin = dataMain["temp_min"].toDouble();

    int pressure = dataMain["pressure"].toDouble();
    int humidity = dataMain["humidity"].toDouble();

    int visibility = data["visibility"].toDouble();

    int windSpeed = dataWind["speed"].toDouble();
    int windDirDegree = dataWind["deg"].toDouble();

    int sunriseTimestamp = dataSys["sunrise"].toInt();
    int sunsetTimestamp = dataSys["sunset"].toInt();

    QDateTime sunriseDateTime;
    sunriseDateTime.setTime_t(sunriseTimestamp);
    QDateTime sunsetDateTime;
    sunsetDateTime.setTime_t(sunsetTimestamp);

    QDate date = sunriseDateTime.date();
    QTime sunriseTime = sunriseDateTime.time();
    QTime sunsetTime = sunsetDateTime.time();

    ui->query->setText(city);

    ui->location->setText(location);
    ui->date->setText(date.toString());
    ui->weatherlabel->setText(weatherCondition);
    ui->temperature->setText(QString::number(temp));
    ui->feelsLikeTempValue->setText(QString::number(feelsLike));
    ui->maxTempValue->setText(QString::number(tempMax));
    ui->minTempValue->setText(QString::number(tempMin));
    ui->pressurevalue->setText(QString::number(pressure).append(" hPa"));
    ui->humidityValue->setText(QString::number(humidity).append("%"));
    ui->visibilityValue->setText(QString::number(visibility).append(" m"));
    ui->windSpeedValue->setText(QString::number(windSpeed).append(" m/s"));
    ui->windDirValue->setText(direction(windDirDegree));
    ui->sunriseValue->setText(sunriseTime.toString().remove(5, 3));
    ui->sunsetValue->setText(sunsetTime.toString().remove(5, 3));
}

QString Widget::direction(int deg){
    QString direction;
    deg += 23;
    deg %= 360;
    if(deg <= 45){
        direction = "North";
    }else if(deg < 90){
        direction = "North-East";
    }else if(deg <= 135){
        direction = "West";
    }else if(deg < 180){
        direction = "South-West";
    }else if(deg <= 225){
        direction = "South";
    }else if(deg < 270){
        direction = "South-West";
    }else if(deg < 315){
        direction = "West";
    }else{
        direction = "North-West";
    }
    return direction;
}

void Widget::clearDisplay(){
    ui->location->setText("WeatherApp");
    ui->date->setText("");
    ui->weatherlabel->setText("Partly sunny");
    ui->temperature->setText(QString::number(0));
    ui->feelsLikeTempValue->setText(QString::number(0));
    ui->maxTempValue->setText(QString::number(0));
    ui->minTempValue->setText(QString::number(0));
    ui->pressurevalue->setText("");
    ui->humidityValue->setText("");
    ui->visibilityValue->setText("");
    ui->windSpeedValue->setText(QString::number(0).append(" m/s"));
    ui->windDirValue->setText(direction(0));
    ui->sunriseValue->setText("00:00");
    ui->sunsetValue->setText("00:00");
}
