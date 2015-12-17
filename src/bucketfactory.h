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

#ifndef BUCKETFACTORY_H
#define BUCKETFACTORY_H

#include "bucketstream.h"

class BucketFactory : public BucketStream
{
public:
    BucketFactory(qint64 bucketSize, int maxSize);
    ~BucketFactory();
    
    void pushBucket(const Bucket &data);
    Bucket popBucket();

protected:
    qint64 m_bucketSize;
    int m_maxSize;
    int m_id;
};

#endif // BUCKETFACTORY_H
