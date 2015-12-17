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

#include "lazyloader.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>

LazyLoader::LazyLoader(const QUrl& url, QObject* parent): QObject(parent)
{
   if (!m_networkAccessManager) m_networkAccessManager=new QNetworkAccessManager();
   m_request.setUrl(url);
   m_reply = m_networkAccessManager->get(m_request);
   connect(m_reply, SIGNAL (finished()),  this, SIGNAL (finished())  );
   connect(m_reply, SIGNAL (error(QNetworkReply::NetworkError)), this, SIGNAL (error(QNetworkReply::NetworkError)) );
}

QByteArray LazyLoader::data() const
{
   return m_reply->readAll();
}

QNetworkAccessManager *LazyLoader::m_networkAccessManager=0L;
#include "lazyloader.moc"
