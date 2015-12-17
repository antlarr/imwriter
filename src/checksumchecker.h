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

#ifndef CHECKSUMCHECKER_H
#define CHECKSUMCHECKER_H

#include "worker.h"
#include "bucketstream.h"
#include <QString>
#include <QNetworkAccessManager>
#include <QByteArray>
#include <QNetworkReply>
#include <openssl/evp.h>

class ChecksumChecker : public Worker
{
   Q_OBJECT
public:
   ChecksumChecker(BucketStream* input, BucketStream* output, const QString & checksum, const QString& filename);
    virtual ~ChecksumChecker();
    
   enum Trust { Unknown, Trusted, KeyMissing, BadSignature, NotSigned }; 
   Trust checksumTrustLevel() const { return m_checksumTrustLevel; };
   QString checksumTrustLevelAsString() const;
   
   QString downloadedChecksum() const { return m_downloadedChecksum; };
   QString calculatedChecksum() const { return m_calculatedChecksum; };
public slots:
   void fileDownloaded();
   void errorDownloading(QNetworkReply::NetworkError code);
   
protected:
   QString hexDataToString(const unsigned char *hexData, unsigned int len) const;
   virtual void run();
    
   QString m_checksumURL;
   QString m_downloadedChecksum;
   Trust m_checksumTrustLevel;

//   QNetworkAccessManager *m_networkAccessManager;
//   QNetworkRequest m_request;
   QNetworkReply *m_reply;
   QByteArray m_downloadedData;
   QString m_filename;
   
   // i.e. "SHA256"
   QString m_hashAlgorithm;
   
   // The calculated checksum from the processed data
   QString m_calculatedChecksum;
};

#endif // CHECKSUMCHECKER_H
