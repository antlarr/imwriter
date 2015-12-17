/*
 * <one line to give the program's name and a brief idea of what it does.>
 * Copyright (C) 2015  Antonio Larrosa <larrosa@kde.org>
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

#include "image.h"
#include <QDebug>

void Image::loadIcon(const QString& name)
{
   QUrl url(name);
   if (!url.isValid() || url.scheme().isEmpty())
   {
      qDebug() << name << "not valid";
      icon=QIcon(QStringLiteral("/usr/share/imwriter/icons/%1.png").arg(name));
      return;
   }
   qDebug() << "valid" << url.scheme(); 
   
}
