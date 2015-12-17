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

#ifndef DOWNLOADER_H
#define DOWNLOADER_H

#include "worker.h"
#include <QString>
#include <QUrl>
#include <QNetworkReply>

class Downloader : public Worker
{
    Q_OBJECT
    
public:
    Downloader(BucketStream* input, BucketStream* output, const QUrl& url, qint64 expectedSize);
    ~Downloader();
    
    virtual void run();

public slots:
   void readData();
   void fileDownloaded();
   void errorDownloading(QNetworkReply::NetworkError code);

protected:
    QUrl m_url;
    QNetworkReply *m_reply;
    bool m_firstTime;
    Bucket m_currentBucket;

};

#endif // READER_H
