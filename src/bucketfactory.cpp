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

#include "bucketfactory.h"

BucketFactory::BucketFactory(qint64 bucketSize, int maxSize) : BucketStream(QStringLiteral("Factory")), 
   m_bucketSize(bucketSize), m_maxSize(maxSize), m_id(0)
{

}

BucketFactory::~BucketFactory()
{

}

void BucketFactory::pushBucket(const Bucket &data)
{
   QMutexLocker locker(&m_mutex);
   m_buckets.append(data);
   m_semaphore.release();
}

Bucket BucketFactory::popBucket()
{
   QMutexLocker locker(&m_mutex);
   if (!m_buckets.isEmpty())
   {
       m_semaphore.acquire();
       Bucket bucket=m_buckets.takeFirst();
       bucket.dataResize(0);
       return bucket;
   }
   return Bucket(m_bucketSize, m_id++);
}
