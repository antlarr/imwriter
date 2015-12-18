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

#ifndef COPYIMAGEJOB_H
#define COPYIMAGEJOB_H
#include "job.h"
#include "imagelistmodel.h"
#include <QString>
#include <QSemaphore>
#include "reader.h"
#include "writer.h"
#include "checksumchecker.h"
#include "bucketfactory.h"
#include "bucketstream.h"

class CopyImageJob : public Job
{
   Q_OBJECT

public:
   ~CopyImageJob();

   virtual void run();

public slots:
   void finishedWriting();
   void updateProgress();

protected:
   CopyImageJob();

   QSemaphore m_semaphore;
   Worker *m_reader;
   Writer *m_writer;
   ChecksumChecker *m_checksumChecker;

   BucketFactory *m_factory;
   BucketStream *m_stream;
   BucketStream *m_stream2;
};

#endif // COPYLOCALFILE_H
