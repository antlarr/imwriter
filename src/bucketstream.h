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

#ifndef BUCKETSTREAM_H
#define BUCKETSTREAM_H

#include <QList>
#include <QSharedPointer>
#include <QSemaphore>
#include <QMutex>

#include "bucket.h"

class BucketStream
{
public:
   BucketStream(const QString &name=QStringLiteral(""));
   virtual ~BucketStream();
   
   virtual void pushBucket(const Bucket &data);
   virtual Bucket popBucket();
   
protected:
   QString m_name;
   QList <Bucket> m_buckets;
   QSemaphore m_semaphore;
   QMutex m_mutex;
};

#endif // BYTEARRAYSTREAM_H
