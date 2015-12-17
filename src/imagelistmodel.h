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

#ifndef IMAGELISTMODEL_H
#define IMAGELISTMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QUrl>
#include <QString>
#include <QIcon>
#include "imagerepository.h"
#include "image.h"

class ImageListModel : public QAbstractItemModel
{
    Q_OBJECT

public:
    ImageListModel();
    ~ImageListModel();
    
    QVariant data(const QModelIndex &index, int role) const;
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QModelIndex parent(const QModelIndex &child) const;
    int rowCount(const QModelIndex &parent) const;
    int columnCount(const QModelIndex &parent) const;
    
    QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

    QMimeData* mimeData(const QModelIndexList& indexes) const;
        
    static Image imageFromUrl(const QUrl &url);
    static ImageListModel *self();
public slots:
    void reloadImages();
private:
    static ImageListModel *s_self;
    ImageRepository m_imageRepository;
    QList<Image> m_images;
};

#endif // IMAGELISTMODEL_H
