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

#ifndef IMAGEREPOSITORY_H
#define IMAGEREPOSITORY_H
#include <QString>
#include <QList>
#include <QIODevice>
#include <QUrl>
#include "lazyloader.h"
#include "image.h"

class ImageRepository : public QObject
{
    Q_OBJECT
public:
    ImageRepository(QObject *parent = 0L);
    ~ImageRepository();
    
    void load(const QUrl &url);
    void load(QIODevice *filename);
    
    QList <Image> images() const { return m_images; };
    
protected slots:
    void loadDataFromLazyLoader();
    
signals:
    void loaded();
protected:    
    QList <Image> m_images;
    LazyLoader *m_loader;
};

#endif // IMAGESREPOSITORY_H
