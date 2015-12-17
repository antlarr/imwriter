/*
 * ImWriter - Image Writer
 * Copyright (C) 2015  Antonio Larrosa <alarrosa@suse.com>
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */

#ifndef LAZYLOADER_H
#define LAZYLOADER_H

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkReply>

class LazyLoader : public QObject
{
    Q_OBJECT
public:
    LazyLoader(const QUrl &url, QObject *parent = 0);
    QByteArray data() const;
signals:
    void finished();
    void error(QNetworkReply::NetworkError error);
private:
   static QNetworkAccessManager *m_networkAccessManager;
   QNetworkRequest m_request;
   QNetworkReply *m_reply;
};

#endif // LAZYLOADER_H
