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

#ifndef COPYREMOTEIMAGEJOB_H
#define COPYREMOTEIMAGEJOB_H
#include "copyimagejob.h"
#include "image.h"
#include <QString>
#include <QSemaphore>
#include "reader.h"
#include "writer.h"
#include "checksumchecker.h"
#include "bucketfactory.h"
#include "bucketstream.h"

class CopyRemoteImageJob : public CopyImageJob
{
    Q_OBJECT

public:
   CopyRemoteImageJob(const Image &sourceFile, const QString &targetFile);
    ~CopyRemoteImageJob();

protected:
    Image m_sourceImage;
    QString m_targetFile;
};

#endif // COPYLOCALFILE_H
