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

#include "imagelistmodel.h"
#include <QIcon>
#include <QVariant>
#include <QUrl>
#include <QMimeData>
#include <QDebug>
#include <QDomDocument>
#include <QFile>
#include <iostream>

#include "imagerepository.h"


ImageListModel::ImageListModel() : m_imageRepository(this)
{
   connect(&m_imageRepository, SIGNAL(loaded()), this, SLOT(reloadImages()));
   //m_imageRepository.load(QUrl(QStringLiteral("http://192.168.1.130/~antonio/imagerepository.xml")));
   m_imageRepository.load(QUrl::fromLocalFile(QStringLiteral("/usr/share/imwriter/imagerepository.xml")));
   m_images=m_imageRepository.images(); 
}

ImageListModel::~ImageListModel()
{
}

QVariant ImageListModel::data(const QModelIndex& index, int role) const
{
   if (!index.isValid()) return QVariant();
   const Image &image=m_images[index.row()];
   switch (role)
   {
      case (Qt::DisplayRole) : return image.name; break;
      case (Qt::DecorationRole) : return image.icon; break;
      case (Qt::UserRole) : return image.url.toString(); break;
      case (Qt::ToolTipRole) : return image.description; break;
      case (Qt::StatusTipRole) : return image.url.toString(); break;
   }
   return QVariant();
}

Qt::ItemFlags ImageListModel::flags(const QModelIndex &index) const
{
   return Qt::ItemIsSelectable | Qt::ItemIsDragEnabled | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | QAbstractItemModel::flags(index);
}

int ImageListModel::rowCount(const QModelIndex& parent) const
{
   return m_images.count();
}
int ImageListModel::columnCount(const QModelIndex& parent) const
{
   return 1;
}
QModelIndex ImageListModel::parent(const QModelIndex& child) const
{
   return QModelIndex();
}

QModelIndex ImageListModel::index(int row, int column, const QModelIndex& parent) const
{
   if ( row >= 0 && row < m_images.count())
      return createIndex(row, column);
   return QModelIndex();
}

QMimeData* ImageListModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData *mimeData = new QMimeData();

    QList <QUrl> urls;
    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
	    urls.append(data(index, Qt::UserRole).toUrl());
        }
    }

    mimeData->setUrls(urls);
    return mimeData;

}

Image ImageListModel::imageFromUrl(const QUrl& url)
{
   for (int i=0; i<self()->m_images.count(); ++i)
   {
      if (self()->m_images.at(i).url==url) return s_self->m_images.at(i);
   }
   return Image();
}

void ImageListModel::reloadImages()
{
   beginResetModel();
   m_images=m_imageRepository.images();
   endResetModel();
}

ImageListModel* ImageListModel::self()
{
   if (!s_self)
      s_self=new ImageListModel;
   return s_self;
}

ImageListModel *ImageListModel::s_self=0L;

#include "imagelistmodel.moc"