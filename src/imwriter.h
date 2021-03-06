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

#ifndef IMWRITER_H
#define IMWRITER_H

#include <kmainwindow.h>
#include <QUrl>
#include <QString>
#include "job.h"
#include "image.h"

class ImWriter : public QObject
{
    Q_OBJECT
    
public:
    ImWriter();
    ~ImWriter();
    
public slots:
    Job *writeImageToDevice(const QUrl &imageUrl, const QString &deviceName);
    Job *writeImageToDevice(const Image &image, const QString &deviceName);

private:
};

#endif // IMWRITER_H
