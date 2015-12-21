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

#include "checksumchecker.h"
#include <QNetworkReply>
#include <QDebug>
#include <QString>
#include <QByteArray>
#include <qeventloop.h>
#include <QTimer>
#include <qgpgme/dataprovider.h>
#include <gpgme++/context.h>
#include <gpgme++/data.h>
#include <gpgme++/verificationresult.h>
#include <gpgme++/decryptionresult.h>
#include <memory>
#include <openssl/evp.h>
#include <iostream>

ChecksumChecker::ChecksumChecker(BucketStream* input, BucketStream* output, const QString &checksum, const QString &filename): 
  Worker(input, output), m_checksumURL(checksum), m_reply(0L), m_filename(filename)
{
  OpenSSL_add_all_algorithms();
}

ChecksumChecker::~ChecksumChecker()
{
}

void ChecksumChecker::run()
{
  /*
   GpgME::Context *context=GpgME::Context::createForProtocol(GpgME::OpenPGP);

   GpgME::Data inData(input, strlen(input));
   GpgME::Data outData;
   
   std::pair<GpgME::DecryptionResult, GpgME::VerificationResult>presult=context->decryptAndVerify(inData, outData);
   GpgME::VerificationResult result=presult.second;
   std::cout << (int)result.signature(0).summary() << std::endl;
   std::cout << result << std::endl;
   exit(0);
   return;
  */
   QNetworkAccessManager networkAccessManager;   
   qDebug() << "checksumchecker::run. Downloading " << m_checksumURL;

   QNetworkRequest request;
   request.setUrl(m_checksumURL);
   m_reply = networkAccessManager.get(request);
   m_downloadedChecksum.clear();
   m_checksumTrustLevel=Unknown;
   
   connect(m_reply, SIGNAL (finished()),  this, SLOT (fileDownloaded())  );
   connect(m_reply, SIGNAL (error(QNetworkReply::NetworkError)), this, SLOT (errorDownloading(QNetworkReply::NetworkError)) );

   //execute the event loop until the file is downloaded or there's an error
   QThread::exec();
   
   qDebug() << "Checksum: " << m_downloadedChecksum << checksumTrustLevelAsString() << " is " << m_hashAlgorithm;
   bool calculateChecksum=!m_downloadedChecksum.isEmpty() && !m_hashAlgorithm.isEmpty();
   
   EVP_MD_CTX ctx;
   const EVP_MD *md=EVP_get_digestbyname(m_hashAlgorithm.toLatin1().constData());
   if (md)
   {
     EVP_MD_CTX_init(&ctx);
     EVP_DigestInit_ex(&ctx, md, NULL);
   }
   else
   {
     qDebug() << "openssl library doesn't recognize hash algorithm " << m_hashAlgorithm;
     calculateChecksum=false;
   }
   
   Bucket bucket=m_inputStream->popBucket();
   while (!bucket.isEOF())
   {
      if (calculateChecksum)
      {
	EVP_DigestUpdate(&ctx, bucket.alignedData(), bucket.dataSize());
      }
      m_processedBytesCount+=bucket.dataSize();
      m_outputStream->pushBucket(bucket);
      bucket=m_inputStream->popBucket();
   };

   m_outputStream->pushBucket(bucket);
   
   unsigned char hashBuffer[EVP_MAX_MD_SIZE];
   unsigned int hashLen=0;
   if (calculateChecksum) EVP_DigestFinal_ex(&ctx, hashBuffer, &hashLen);
   if (md) EVP_MD_CTX_cleanup(&ctx);
   
   m_calculatedChecksum=hexDataToString(hashBuffer, hashLen);
   qDebug() << "checksumchecker ending" << m_calculatedChecksum;
}

void ChecksumChecker::errorDownloading(QNetworkReply::NetworkError code)
{
   qDebug() << "Error downloading " << qPrintable( code );
   m_downloadedChecksum.clear();
   m_checksumTrustLevel=Unknown;
   QThread::exit(1);
}

void ChecksumChecker::fileDownloaded()
{
   qDebug() << "checksum downloaded";
   if (!m_reply)
   {
     qDebug() << "m_reply is NULL";
     m_downloadedChecksum.clear();
     m_checksumTrustLevel=Unknown;
   
     return;
   }
   
   QByteArray downloadedData=m_reply->readAll();
   QString message=QString::fromUtf8(downloadedData.constData());
   qDebug() << message;
   GpgME::Context *context=GpgME::Context::createForProtocol(GpgME::OpenPGP);

   GpgME::Data data;
   GpgME::VerificationResult result=context->verifyOpaqueSignature(GpgME::Data(downloadedData.constData(),downloadedData.count()), data);
   std::cout << result << std::endl;

   if (result.error().code()!=GPG_ERR_NO_ERROR)
   {
      // ERROR HANDLING
      qDebug() << "Error verifying signature:" << result.error().asString();
   }
   if (result.numSignatures()==1) 
   {
     if (result.signature(0).status()==0) m_checksumTrustLevel=Trusted;
     else
      switch (result.signature(0).summary())
      {
	case (GpgME::Signature::None): m_checksumTrustLevel=NotSigned; break;
	case (GpgME::Signature::Valid): m_checksumTrustLevel=Trusted; break;
	case (GpgME::Signature::Green): m_checksumTrustLevel=Trusted; break;
	case (GpgME::Signature::KeyMissing): m_checksumTrustLevel=KeyMissing; break;
	case (GpgME::Signature::Red): m_checksumTrustLevel=BadSignature; break;
	default: 
	  m_checksumTrustLevel=ChecksumChecker::Unknown; 
	  qDebug() << "Generic Signature Error" << (int)result.signature(0).summary(); break;
      };
   }
   else 
   {
      if (result.numSignatures()==0) m_checksumTrustLevel=NotSigned;
      else m_checksumTrustLevel=Unknown;
      qDebug() << result.numSignatures() << " signatures found";
   }
   
   // Get the checksum from the downloaded file
   QRegExp regexp;
   regexp.setPattern(QStringLiteral("\nHash: ([^\n]+)\n"));
   int pos=regexp.indexIn(message);
   if (pos!=-1) m_hashAlgorithm=regexp.capturedTexts()[1];
   else m_hashAlgorithm=QStringLiteral("SHA256");
   
   if (!m_filename.isEmpty())
      regexp.setPattern(QStringLiteral("(^|\n)([a-f0-9]{32,}) +%1\n").arg(m_filename));
   else
      regexp.setPattern(QStringLiteral("(^|\n)([a-f0-9]{32,}) +[^\n]*\n"));
   
   pos=regexp.indexIn(message);
   if (pos!=-1) m_downloadedChecksum=regexp.capturedTexts()[2];
   else // Let's try a different format of sha256sum output
   {
     regexp.setPattern(QStringLiteral("(^|\n)%1 \\(%2\\) = ([a-f0-9]{32,})\n").arg(m_hashAlgorithm).arg(m_filename));
     pos=regexp.indexIn(message,0,QRegExp::CaretAtZero);
     if (pos!=-1) m_downloadedChecksum=regexp.capturedTexts()[2];
     else m_downloadedChecksum.clear();
   }
      
   m_reply->deleteLater();
   QThread::exit(0);
}

QString ChecksumChecker::checksumTrustLevelAsString() const
{
   switch (m_checksumTrustLevel)
   {
      case (Unknown): return QStringLiteral("Unknown"); break;
      case (Trusted): return QStringLiteral("Trusted"); break;
      case (KeyMissing): return QStringLiteral("KeyMissing"); break;
      case (BadSignature): return QStringLiteral("BadSignature"); break;
      case (NotSigned): return QStringLiteral("NotSigned"); break;
      default: return QStringLiteral("UnknownValue"); break;
   };
}

QString ChecksumChecker::hexDataToString(const unsigned char* hexData, unsigned int len) const
{
  char aux[len*2];
  char hexval[16] = {'0', '1', '2', '3', '4', '5', '6', '7', '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};
  for(unsigned int j = 0; j < len; j++)
  {
    aux[j*2] = hexval[((hexData[j] >> 4) & 0xF)];
    aux[(j*2) + 1] = hexval[(hexData[j]) & 0x0F];
  }
  return QString::fromLatin1(aux, len*2);
}
