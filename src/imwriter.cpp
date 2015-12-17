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

#include "imwriter.h"
#include "copylocalfilejob.h"
#include "copyremoteimagejob.h"
#include <QDebug>
#include <QApplication>
#include <openssl/evp.h>
#include <gpgme++/global.h>

ImWriter::ImWriter() : QObject()
{
   OpenSSL_add_all_algorithms();
   GpgME::initializeLibrary();  
}

ImWriter::~ImWriter()
{
}

Job *ImWriter::writeImageToDevice(const QUrl &url, const QString &deviceName)
{
       qDebug() << "Now should write " << url << " to " << deviceName;
       Image image=ImageListModel::imageFromUrl(url);
       CopyImageJob *job=0L;
       if (url.isLocalFile())
       {
	 qDebug() << url << image.isValid();
	  if (image.isValid())
	     job=new CopyLocalFileJob(image,  deviceName);
	  else
	  {
             job=new CopyLocalFileJob(url.toLocalFile(),  deviceName);
	     qDebug() << job;
	  }
       }
       else
          job=new CopyRemoteImageJob(image,  deviceName);
       qDebug() << job;
       connect(job, SIGNAL(finished()), job, SLOT(deleteLater()));
       qDebug() << job;
       job->start();
       return job;
}

Job *ImWriter::writeImageToDevice(const Image &image, const QString &deviceName)
{
       qDebug() << "Now should write " << image.url << " to " << deviceName;
       CopyRemoteImageJob *job=new CopyRemoteImageJob(image,  deviceName);
       connect(job, SIGNAL(finished()), job, SLOT(deleteLater()));
       job->start();
       return job;
}


#include "imwriter.moc"
