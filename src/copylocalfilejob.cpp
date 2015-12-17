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

#include "copylocalfilejob.h"
#include <QObject>
#include <QDebug>
#include "bucketfactory.h"
#include "bucketstream.h"
#include "reader.h"
#include "writer.h"
#include "imagelistmodel.h"
#include "checksumchecker.h"

CopyLocalFileJob::CopyLocalFileJob(const QString &sourceFile, const QString &targetFile):
   CopyImageJob(), m_sourceFile(sourceFile), m_targetFile(targetFile)
{
   m_factory=new BucketFactory(1024*1024, 10); 
   m_stream=new BucketStream(QStringLiteral("stream1"));
   m_stream2=new BucketStream(QStringLiteral("stream2"));

   m_reader=new Reader(m_factory, m_stream, m_sourceFile);
   m_writer=new Writer(m_stream, m_stream2, m_targetFile);
}

CopyLocalFileJob::CopyLocalFileJob(const Image& sourceImage, const QString& targetFile):
   CopyImageJob(), m_sourceImage(sourceImage), m_targetFile(targetFile)
{
   m_factory=new BucketFactory(1024*1024, 10); 
   m_stream=new BucketStream(QStringLiteral("stream1"));
   m_stream2=new BucketStream(QStringLiteral("stream2"));

   m_reader=new Reader(m_factory, m_stream, m_sourceImage.url.toLocalFile());
   m_writer=new Writer(m_stream, m_stream2, m_targetFile);
   
}

CopyLocalFileJob::~CopyLocalFileJob()
{
}

#include "copylocalfilejob.moc"


