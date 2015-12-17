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

#include "copyremoteimagejob.h"
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

CopyRemoteImageJob::CopyRemoteImageJob(const Image& sourceImage, const QString& targetFile):
   CopyImageJob(), m_sourceImage(sourceImage)
{
   m_factory=new BucketFactory(1024*1024, 10); 
   m_stream=new BucketStream(QStringLiteral("stream1"));
   m_stream2=new BucketStream(QStringLiteral("stream2"));

   m_reader=new Downloader(m_factory, m_stream, m_sourceImage.url, m_sourceImage.size);
   m_writer=new Writer(m_stream, m_stream2, m_targetFile);
   m_checksumChecker=new ChecksumChecker(m_stream2, m_factory, m_sourceImage.checksumUrl.toString(), m_sourceImage.url.fileName());
}

CopyRemoteImageJob::~CopyRemoteImageJob()
{
}

#include "copyremoteimagejob.moc"
