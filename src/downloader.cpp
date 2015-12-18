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

#include "downloader.h"
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include <QNetworkReply>
#include <qglobal.h>

Downloader::Downloader(BucketStream *input, BucketStream *output, const QUrl &url, qint64 expectedSize) : 
  Worker(input, output), m_url(url), m_firstTime(true), m_networkAccessManager(0L), m_request(0L), m_reply(0L)
{
  m_expectedSize=expectedSize;
}
Downloader::~Downloader()
{
   
}

void Downloader::requestUrl(const QUrl& url)
{
   m_request->setUrl(m_url);
   m_firstTime=true;
   
   m_reply = m_networkAccessManager->get(*m_request);
   m_reply->setReadBufferSize(128*1024);

   connect(m_reply, SIGNAL (readyRead()),  this, SLOT (readData()), Qt::DirectConnection);
   connect(m_reply, SIGNAL (finished()),  this, SLOT (fileDownloaded())  );
   connect(m_reply, SIGNAL (error(QNetworkReply::NetworkError)), this, SLOT (errorDownloading(QNetworkReply::NetworkError)) );
}

void Downloader::run()
{
   m_networkAccessManager=new QNetworkAccessManager;
   m_request=new QNetworkRequest;

   requestUrl(m_url);

   QThread::exec();
   
   if (m_currentBucket.dataSize()>0)
       m_outputStream->pushBucket(m_currentBucket);
    
   m_currentBucket=m_inputStream->popBucket();
   m_currentBucket.setEOF(true);
   m_outputStream->pushBucket(m_currentBucket);

   delete m_reply;
   delete m_request;
   delete m_networkAccessManager;

   qDebug() << "downloader really done";
   
}
void Downloader::fileDownloaded()
{
   qDebug() << "File downloaded";
   QThread::exit(0);
}

void Downloader::errorDownloading(QNetworkReply::NetworkError code)
{
   qDebug() << "Error downloading " << qPrintable( code );
   QThread::exit(1);
}

void Downloader::readData()
{
  if (m_firstTime)
  {
    QVariant value=m_reply->header(QNetworkRequest::ContentLengthHeader);

    qDebug() << "ContentDisposition" << m_reply->header(QNetworkRequest::ContentDispositionHeader);
    int status=m_reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
    if (status >= 300 && status <= 399)
    {
      // File has moved!
      m_url = m_reply->header(QNetworkRequest::LocationHeader).toUrl();
      qDebug() << "Content moved to " << m_url;

      m_reply->disconnect(SIGNAL(finished()), this, SLOT(fileDownloaded()));
      m_reply->deleteLater();

      requestUrl(m_url);
      return;
    }
    QList <QNetworkReply::RawHeaderPair>pairs=m_reply->rawHeaderPairs();
    if (value.isValid()) 
    {
      m_expectedSize=value.toULongLong();
      qDebug() << "Content-Length size:" << m_expectedSize;
    }
    m_firstTime=false;
  }
  int bytesAvailable=m_reply->bytesAvailable();
  //qDebug() << bytesAvailable << "bytes received";
  m_processedBytesCount+=bytesAvailable;
  while (bytesAvailable>0)
  {
    if (m_currentBucket.capacity()==0)
    {
       m_currentBucket=m_inputStream->popBucket();
    }
    int bytesToRead=qMin(bytesAvailable, m_currentBucket.remainingDataSize());
    int bytesRead=m_reply->read(m_currentBucket.alignedData()+m_currentBucket.dataSize(), bytesToRead);
    m_currentBucket.dataGrow(bytesRead);
    if (m_currentBucket.remainingDataSize()==0)
    {
       m_outputStream->pushBucket(m_currentBucket);
       m_currentBucket=Bucket();
    }
    bytesAvailable-=bytesRead;
  }
  /*
  while (bytesAvailable>0)
  {
    Bucket bucket=m_inputStream->popBucket();
    int bytesToRead=qMin(bytesAvailable, bucket.maxSize());
    int bytesRead=m_reply->read(bucket.alignedData(), bytesToRead);
    bucket.dataResize(bytesRead);
    m_outputStream->pushBucket(bucket);
    bytesAvailable-=bytesRead;
  }
  */
}

#include "downloader.moc"
