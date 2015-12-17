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

#include "copyimagejob.h"
#include <QObject>
#include <QDebug>
#include <QEventLoop>
#include <QTimer>
#include "bucketfactory.h"
#include "bucketstream.h"
#include "downloader.h"
#include "reader.h"
#include "writer.h"
#include "imagelistmodel.h"
#include "checksumchecker.h"
#include <iostream>

CopyImageJob::CopyImageJob():
   m_reader(0L), m_writer(0L), m_checksumChecker(0L), m_factory(0L), m_stream(0L), m_stream2(0L)
{
}

CopyImageJob::~CopyImageJob()
{
  delete m_reader;
  delete m_writer;
  delete m_checksumChecker;
  delete m_factory;
  delete m_stream;
  delete m_stream2;
}

void CopyImageJob::run()
{
//   bool checkChecksum = !m_sourceImage.checksumUrl.isEmpty();
      
   std::cout << "created" << std::endl;
   if (m_checksumChecker)
      QObject::connect(m_checksumChecker, SIGNAL(finished()), this, SLOT(finishedWriting()));
   else
      QObject::connect(m_writer, SIGNAL(finished()), this, SLOT(finishedWriting()));
   m_reader->start();
   m_writer->start();
   if (m_checksumChecker) m_checksumChecker->start();
   std::cout << "threads started" << std::endl;
   QTimer timer;
   connect(&timer, SIGNAL(timeout()), this, SLOT(updateProgress()));
   timer.start(1000);
   
   exec(); // Run the event loop
   
   timer.stop();
   qDebug() << QStringLiteral("threads really done");
   exit(1);
}

void CopyImageJob::finishedWriting()
{
    qDebug() << QStringLiteral("done writting");
    exit(0);
}

void CopyImageJob::updateProgress()
{
  int percentage;
  if (m_checksumChecker)
     percentage= m_checksumChecker->processedBytesCount()*100/m_reader->expectedSize();
  else
     percentage= m_writer->processedBytesCount()*100/m_reader->expectedSize();
  qDebug() << percentage << "%";
}

#include "copyimagejob.moc"
