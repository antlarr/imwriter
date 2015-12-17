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

#ifndef WRITER_H
#define WRITER_H

#include "worker.h"
#include <QString>

class Writer: public Worker
{
    Q_OBJECT
    
public:
    Writer(BucketStream *input, BucketStream *output, const QString &filename);
    ~Writer();
    
    virtual void run();
protected:
    int m_fd;
    QString m_filename;
};

#endif // WRITER_H
