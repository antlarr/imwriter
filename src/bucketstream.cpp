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

#include "bucketstream.h"
#include <QMutexLocker>
#include <QDebug>

BucketStream::BucketStream(const QString &name): m_name(name)
{

}

BucketStream::~BucketStream()
{

}

void BucketStream::pushBucket(const Bucket &bucket)
{
   QMutexLocker locker(&m_mutex);
   m_buckets.append(bucket);
   
   //qDebug () << "push" << m_buckets.size() << bucket.id();
   m_semaphore.release();
}

Bucket BucketStream::popBucket()
{
   
   m_semaphore.acquire();
   QMutexLocker locker(&m_mutex);
   Bucket bucket=m_buckets.takeFirst();
   //qDebug () << "pop" << m_buckets.size() << bucket.id();
   return bucket;
}
