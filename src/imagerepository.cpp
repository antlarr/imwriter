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

#include "imagerepository.h"
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <QBuffer>
#include <iostream>
#include "image.h"

ImageRepository::ImageRepository(QObject *parent): QObject(parent), m_loader(0L)
{

}

ImageRepository::~ImageRepository()
{

}
void ImageRepository::load(const QUrl &url)
{
   if (url.isLocalFile())
   {
      QFile file(url.toLocalFile());
      if (!file.open(QIODevice::ReadOnly))
         return;
      qDebug() << "file" <<url.toLocalFile();

      load(&file);
   }
   else
   {
      m_loader = new LazyLoader(url);
      connect(m_loader, SIGNAL(finished()), this, SLOT(loadDataFromLazyLoader()));
   }
}

void ImageRepository::load(QIODevice *file)
{
   QDomDocument doc(QStringLiteral("repository"));
   QString errorMsg;
   int errorLine, errorColumn;
   if (!doc.setContent(file, false, &errorMsg, &errorLine, &errorColumn)) {
      std::cerr << qPrintable(QStringLiteral("Error processing xml input: %1 (line %2, col %3)").arg(errorMsg).arg(errorLine).arg(errorColumn)) << std::endl;
      return;
   }
   
   QDomElement docElem = doc.documentElement();
   QDomNode n = docElem.firstChild();
   
   while(!n.isNull()) 
   {
      QDomElement e = n.toElement(); // try to convert the node to an element.
      if(!e.isNull() && e.tagName()==QStringLiteral("image"))
      {
	 Image image;
	 
         qDebug() << qPrintable(e.tagName()); // the node really is an element.
	 image.name=n.firstChildElement(QStringLiteral("name")).text();
	 image.url=n.firstChildElement(QStringLiteral("url")).text();
	 image.description=n.firstChildElement(QStringLiteral("description")).text();
	 image.arch=n.firstChildElement(QStringLiteral("arch")).text();
	 image.type=n.firstChildElement(QStringLiteral("type")).text();
	 image.loadIcon(n.firstChildElement(QStringLiteral("icon")).text());
	 image.checksumUrl=n.firstChildElement(QStringLiteral("checksumurl")).text();

	 m_images.append(image);
      }
      n = n.nextSibling();
   }
}


void ImageRepository::loadDataFromLazyLoader()
{
   QByteArray data=m_loader->data();
   delete m_loader; m_loader=0L;
   
   QBuffer buffer(&data);
   load(&buffer);
   
   emit(loaded());
}
