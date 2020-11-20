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
#include <string>

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);


    manager = new QNetworkAccessManager();
    QObject::connect(manager, &QNetworkAccessManager::finished,
        this, [=](QNetworkReply *reply) {
            if (reply->error()) {
                qDebug() << reply->errorString();
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
//    ui->textarea->setPlainText(ui->query->text());
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

    QString weatherCondition = dataWeather["main"].toString();
    int weatherId = dataWeather["id"].toInt();

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
    int timezone = data["timezone"].toInt();

    QDateTime sunriseDateTime;
    sunriseDateTime.setTime_t(sunriseTimestamp);
    QDateTime sunsetDateTime;
    sunsetDateTime.setTime_t(sunsetTimestamp);

    qDebug() << sunriseDateTime.toString(Qt::SystemLocaleDate);
    qDebug() << sunsetDateTime.toString(Qt::SystemLocaleShortDate);

    qDebug() << sunriseDateTime.toString(Qt::SystemLocaleLongDate);

    ui->query->setText(city);

    ui->location->setText(city);
    ui->countryCode->setText(countryCode);
    ui->weatherlabel->setText(weatherCondition);
    ui->temperature->setText(QString::number(temp));
    ui->feelsLikeTempValue->setText(QString::number(feelsLike));
    ui->maxTempValue->setText(QString::number(tempMax));
    ui->minTempValue->setText(QString::number(tempMin));
    ui->pressurevalue->setText(QString::number(pressure));
    ui->humidityValue->setText(QString::number(humidity));
    ui->visibilityValue->setText(QString::number(visibility));
    ui->windSpeedValue->setText(QString::number(windSpeed));
    ui->windDirValue->setText(QString::number(windDirDegree));

}
