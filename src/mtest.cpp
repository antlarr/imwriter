#include "mtest.h"
#include <QDebug>
#include <QCoreApplication>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>
#include <QString>
#include <string>

Test::Test(): QThread()
{
  /*
   QNetworkAccessManager *manager = new QNetworkAccessManager(this);
   connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

   manager->get(QNetworkRequest(QUrl(QStringLiteral("http://www.qt-project.org"))));
   */
}

void Test::run()
{
   QNetworkAccessManager *manager = new QNetworkAccessManager();
   connect(manager, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyFinished(QNetworkReply*)));

   manager->get(QNetworkRequest(QUrl(QStringLiteral("http://www.qt-project.org"))));
   QEventLoop loop;
   loop.exec();
   //(true) qApp->processEvents();
   
}

void Test::finished()
{
   qDebug() << "finished";
}

void Test::replyFinished(QNetworkReply*reply)
{
   qDebug() << "replyFinished" << reply->readAll().toStdString().c_str();
   qDebug() << "replyFinished2";
}

int main(int argc, char** argv)
{
   QCoreApplication app(argc, argv);
   
   Test test;
   test.start();
   return app.exec();
}