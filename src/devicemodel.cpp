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

#include "devicemodel.h"
#include <QIcon>
#include <QVariant>
#include <QDebug>
#include <solid/device.h>
#include <solid/devicenotifier.h>
#include <solid/storagedrive.h>
#include <solid/storagevolume.h>
#include <solid/block.h>

DeviceModel::DeviceModel()
{
   populateModel();
   connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceAdded(QString)),
	   this, SLOT(addDevice(QString)));
   connect(Solid::DeviceNotifier::instance(), SIGNAL(deviceRemoved(QString)), 
	   this, SLOT(removeDevice(QString)));
   
   
}

DeviceModel::~DeviceModel()
{
}

void DeviceModel::populateModel()
{
   m_devices.clear();
   
   Solid::DeviceNotifier *notifier = Solid::DeviceNotifier::instance();
 
   //get a list of all devices that are AudioHw
   foreach (Solid::Device device, Solid::Device::listFromType(Solid::DeviceInterface::StorageDrive, QString()))
   {
      if(!device.is<Solid::Block>() ) continue;
      Solid::Block *block=device.as<Solid::Block>();
      Solid::StorageDrive *drive=device.as<Solid::StorageDrive>();
      
      if (!drive->isHotpluggable()) continue;
      
      Device dev;
      dev.udi=device.udi();
      dev.device=block->device();
      dev.description=device.description();
      dev.size=drive->size();
      m_devices.append(dev);
      qDebug() << block->device();
   }

   foreach (Solid::Device device, Solid::Device::listFromType(Solid::DeviceInterface::StorageVolume, QString()))
   {
      if(!device.is<Solid::Block>() ) continue;
      Solid::Block *block=device.as<Solid::Block>();
      Solid::StorageVolume *volume=device.as<Solid::StorageVolume>();
      
      if ( /*volume->usage()!=Solid::StorageVolume::PartitionTable
         ||*/ !device.product().contains(QStringLiteral("USB"))) continue;
      
      Device dev;
      dev.udi=device.udi();
      dev.device=block->device();
      dev.description=device.description();
      dev.size=volume->size();
      m_devices.append(dev);
      qDebug() << block->device();
   }
   
   std::sort(m_devices.begin(), m_devices.end(), [](const Device &a, const Device &b) -> bool
                                                   { return a.device < b.device; } );
   
   noAvailableDevices( m_devices.count()==0 );
}

void DeviceModel::addDevice(const QString& udi)
{
    qDebug() << "addDevice" << udi;
   Solid::Device device(udi);
   if(!device.is<Solid::Block>()) return;
   Solid::Block *block=device.as<Solid::Block>();
   Device dev;
   dev.udi=device.udi();
   dev.device=block->device();
   dev.description=device.description();
      
   if (device.is<Solid::StorageDrive>()) 
   {
      Solid::StorageDrive *drive=device.as<Solid::StorageDrive>();  
      if (!drive->isHotpluggable()) return;
      
      dev.size=drive->size();
   }
   else if(device.is<Solid::StorageVolume>())
   {
      if (!device.product().contains(QStringLiteral("USB"))) return;
      Solid::StorageVolume *volume=device.as<Solid::StorageVolume>();
   
      dev.size=volume->size();
   }
   else return;
   beginInsertRows(QModelIndex(), m_devices.count(), m_devices.count());
   m_devices.append(dev);
   endInsertRows();

   qDebug() << block->device() << m_devices.count();
   if (m_devices.count()==1) noAvailableDevices( false );
}

void DeviceModel::removeDevice(const QString &udi)
{
  qDebug() << "removeDevice" << udi;
   for (int idx=0;idx<m_devices.count(); ++idx)
   {
      if (m_devices[idx].udi == udi)
      {
	qDebug() << "removingDevice" << udi;
	 beginRemoveRows(QModelIndex(), idx, idx);
	 m_devices.removeAt(idx);
	 endRemoveRows();
	 break;
      }
   }
   if (m_devices.count()==0) noAvailableDevices( true );
}

QString DeviceModel::formatSizeDisplay(qint64 size)
{
   if (size>1000000000L) return QString::number(size/1000000000.0, 'f', 1)+QStringLiteral(" GB");
   if (size>1000000L) return QString::number(size/1000000.0, 'f', 1)+QStringLiteral(" MB");
   return QString::number(size/1000.0, 'f', 1)+QStringLiteral(" KB");
}

QVariant DeviceModel::data(const QModelIndex& index, int role) const
{
  if (!index.isValid()) return QVariant();
  
  Device dev=m_devices[index.row()];
  switch (role)
  {
     case (Qt::DisplayRole) : return QStringLiteral("%1\n%2\n%3").arg(dev.device).arg(dev.description).arg(formatSizeDisplay(dev.size)); break;
     case (Qt::DecorationRole) : return QIcon::fromTheme(QStringLiteral("drive-removable-media")); break;
     case (Qt::UserRole) : return dev.device; break;
  }
  return QVariant();
}

Qt::ItemFlags DeviceModel::flags(const QModelIndex &index) const
{
   if (index.isValid())
      return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | Qt::ItemIsDropEnabled | QAbstractItemModel::flags(index);
   return Qt::ItemIsSelectable | Qt::ItemIsEnabled | Qt::ItemNeverHasChildren | QAbstractItemModel::flags(index);
}

int DeviceModel::rowCount(const QModelIndex& parent) const
{
   return m_devices.count();
}

int DeviceModel::count() const
{
   return m_devices.count();
}

int DeviceModel::columnCount(const QModelIndex& parent) const
{
   return 1;
}

QModelIndex DeviceModel::parent(const QModelIndex& child) const
{
   return QModelIndex();
}

QModelIndex DeviceModel::index(int row, int column, const QModelIndex&) const
{
   if ( row >= 0 && row < m_devices.count())
      return createIndex(row, column);
   return QModelIndex();
}

QMimeData* DeviceModel::mimeData(const QModelIndexList& indexes) const
{
    QMimeData *mimeData = new QMimeData();
    QByteArray encodedData;

    QDataStream stream(&encodedData, QIODevice::WriteOnly);

    foreach (QModelIndex index, indexes) {
        if (index.isValid()) {
            QUrl url = data(index, Qt::UserRole).toUrl();
            stream << url;
        }
    }

    mimeData->setData(QStringLiteral("text/uri-list"), encodedData);
    return mimeData;

}
QStringList DeviceModel::mimeTypes() const
{
    return QStringList() << QStringLiteral("text/uri-list");
}

bool DeviceModel::dropMimeData(const QMimeData* mimeData, Qt::DropAction action, int row, int column, const QModelIndex& parent)
{
   Q_UNUSED(row); Q_UNUSED(column);
   
    if (!mimeData->hasUrls())
        return false;

    if (action == Qt::IgnoreAction)
        return true;

    foreach (QUrl url, mimeData->urls())
    {
       emit writeImageToDevice(url, data(parent, Qt::UserRole).toString() );
    }

    return true;
}

#include "devicemodel.moc"
