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

#ifndef DEVICEMODEL_H
#define DEVICEMODEL_H

#include <QAbstractListModel>
#include <QMimeData>
#include <QUrl>
#include <QString>
#include <solid/devicenotifier.h>

class DeviceModel : public QAbstractItemModel
{
   Q_OBJECT
   class Device 
   {
   public:
      QString udi;
      QString device;
      QString description;
      qulonglong size;
   };
    
public:
   DeviceModel();
   ~DeviceModel();
    
   void populateModel();
    
   QVariant data(const QModelIndex &index, int role) const;
   Qt::ItemFlags flags(const QModelIndex &index) const;
   QModelIndex parent(const QModelIndex &child) const;
   int count() const;
   int rowCount(const QModelIndex &parent) const;
   int columnCount(const QModelIndex &parent) const;
    
   QModelIndex index(int row, int column, const QModelIndex &parent = QModelIndex()) const;

   bool dropMimeData(const QMimeData *data, Qt::DropAction action,
                      int row, int column, const QModelIndex &parent);
   QMimeData *mimeData(const QModelIndexList &indexes) const;
   QStringList mimeTypes() const;

   static QString formatSizeDisplay(qint64 size);
signals:
   void writeImageToDevice(const QUrl &url, const QString &data);
   void noAvailableDevices(bool);
   
public slots:
   void addDevice(const QString &udi);
   void removeDevice(const QString &udi);
   
private:
   QList <Device>m_devices;
   Solid::DeviceNotifier *m_deviceNotifier;
};

#endif // DEVICEMODEL_H
