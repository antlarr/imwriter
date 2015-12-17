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

#ifndef MEMORYPOOL_H
#define MEMORYPOOL_H

#include <QObject>
#include <QByteArray>
#include <QSharedPointer>
#include <QSet>

class Bucket : public QByteArray
{   
public:
   static const int Alignment=512;
   
   Bucket(int size=0, int id=0);
   ~Bucket(); // QByteArray doesn't have a virtual destructor, but that shouldn't
              // be a problem, since we never use it polymorphically
   
   char *alignedData();
   const char *constAlignedData() const;
   bool isEOF() const { return m_isEOF; };
   void setEOF(bool b);
   int id() const { return m_id; };
   int dataCapacity() const { return capacity()-Alignment; };
   int dataSize() const { return m_dataSize; };
   void dataResize(int size);
   void dataGrow(int addedSize);
   int maxDataSize() const;
   int remainingDataSize() const;
protected:
   bool m_isEOF;
   int m_id;
   int m_dataSize;
};

#endif // MEMORYPOOL_H
