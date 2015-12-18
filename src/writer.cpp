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

#include "writer.h"
#include <QDebug>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <iostream>
#include <unistd.h>
#include "functions.h"

Writer::Writer(BucketStream *input, BucketStream *output, const QString& filename) : Worker(input, output), m_filename(filename)
{
   //m_filename=QStringLiteral("/home/antonio/data2.raw");
   m_fd=::open(m_filename.toLocal8Bit().constData(), O_RDWR | O_CREAT | O_TRUNC | O_LARGEFILE, S_IRUSR | S_IWUSR);
   if (m_fd == -1)
   {
      int err=errno;
      std::cerr << qPrintable(QStringLiteral("Error opening %1: %2").arg(m_filename).arg(qStrerror(err))) << std::endl;
   }
}

Writer::~Writer()
{
   ::close(m_fd);
}

void Writer::run()
{
   int bytesWritten;
   Bucket bucket=m_inputStream->popBucket();
   //qDebug() << QStringLiteral("pop");
   while (!bucket.isEOF())
   {
      bytesWritten=::write(m_fd, bucket.constAlignedData(), bucket.dataSize());
      if (bytesWritten!=bucket.dataSize())
      {
	 if (bytesWritten==-1)
	 {
	    int err=errno;
	    qDebug() << QStringLiteral("Error writing %1 bytes to %2: %3").arg(bucket.dataSize()).arg(m_filename).arg(qStrerror(err));
	 }
	 else
	    qDebug() << QStringLiteral("%1 bytes requested to be written but only %2 bytes written").arg(bucket.dataSize()).arg(bytesWritten);
	 ::exit(1);
      }
      m_processedBytesCount+=bytesWritten;
      m_outputStream->pushBucket(bucket);
      
      bucket=m_inputStream->popBucket();
   };
   m_outputStream->pushBucket(bucket);
   qDebug() << "writer done";
}

#include "writer.moc"
