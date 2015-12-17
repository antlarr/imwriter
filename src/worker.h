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

#ifndef WORKER_H
#define WORKER_H

#include <QThread>
#include "bucketstream.h"

class Worker : public QThread
{
public:
    Worker(BucketStream *input, BucketStream *output);
    virtual ~Worker();

    virtual qint64 expectedSize() const { return m_expectedSize; };
    virtual qint64 processedBytesCount() const { return m_processedBytesCount; };
    
protected:
    virtual void run();

    BucketStream* m_inputStream;
    BucketStream* m_outputStream;
    qint64 m_expectedSize;
    qint64 m_processedBytesCount;

};

#endif // WORKER_H
