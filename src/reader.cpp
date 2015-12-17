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

#include "reader.h"
#include <QDebug>
#include <QFileInfo>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "functions.h"

Reader::Reader(BucketStream *input, BucketStream *output, const QString& filename) : Worker(input, output), m_filename(filename)
{
   //m_filename=QStringLiteral("/home/antonio/data1.raw");
   QFileInfo info(m_filename);
   m_expectedSize=info.size();
   m_fd=::open(m_filename.toLocal8Bit().constData(), O_RDONLY | O_LARGEFILE | O_DIRECT);
   if (m_fd == -1)
   {
      int err=errno;
      std::cerr << "Error opening " << qPrintable(filename) << " : " << qPrintable(qStrerror(err)) << std::endl;
   }
}

Reader::~Reader()
{
   ::close(m_fd);
}
void Reader::run()
{
   int bytesRead;
   Bucket bucket=m_inputStream->popBucket();
   while ((bytesRead=::read(m_fd, bucket.alignedData(), bucket.dataCapacity()))>0)
   {
      if (bytesRead==0) qDebug() << "0 bytes Read";
      bucket.dataResize(bytesRead);
      m_outputStream->pushBucket(bucket);
      m_processedBytesCount+=bytesRead;
      
      bucket=m_inputStream->popBucket();
   };
   if (bytesRead<0)
   {
      int err=errno;
      std::cerr << "ERROR reading :" << qPrintable(qStrerror(err)) << std::endl;
   }
   bucket.setEOF(true);
   m_outputStream->pushBucket(bucket);
   qDebug() << "reader done";
}

#include "reader.moc"
