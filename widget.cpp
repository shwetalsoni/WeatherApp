#include "widget.h"
#include "ui_widget.h"
#include "QtNetwork/QNetworkReply"
#include "QUrl"
#include "QtNetwork/QNetworkAccessManager"
#include "QtNetwork/QNetworkRequest"
#include "QJsonArray"
#include "QJsonObject"
#include "QJsonParseError"
#include "QJsonValue"

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

            QString answer = reply->readAll();

            qDebug() << answer;
            ui->query->setText(answer);
        }
    );


}

Widget::~Widget()
{
    delete ui;
}


void Widget::on_queryButton_clicked()
{
    QString str = ui->query->text();
//    ui->textarea->setPlainText(ui->query->text());
    request.setUrl(QUrl("http://api.openweathermap.org/data/2.5/weather?q=mumbai&appid=3130fe9c95bcd429a94c61a79c4f7c32&units=metric"));
    manager->get(request);
}
