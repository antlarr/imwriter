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

#include "bucket.h"
#include <QDebug>

Bucket::Bucket(int size, int id) : QByteArray(), m_isEOF(false), m_id(id), m_dataSize(0)
{
   if (size>0)
      resize(size+Alignment);
}

Bucket::~Bucket()
{

}
void Bucket::setEOF(bool b)
{
   m_isEOF=b;  
   dataResize(0);
}

void Bucket::dataResize(int size)
{
   m_dataSize=size;
}

void Bucket::dataGrow(int addedSize)
{
   m_dataSize+=addedSize;
}

char* Bucket::alignedData()
{
   uintptr_t addr=(uintptr_t)data();
   if (addr % Alignment != 0)
      addr+=Alignment - addr % Alignment;
   return (char *)addr;
}

const char* Bucket::constAlignedData() const
{
   uintptr_t addr=(uintptr_t)constData();
   if (addr % Alignment != 0)
      addr+=Alignment - addr % Alignment;
   return (char *)addr;
}

int Bucket::maxDataSize() const
{
    return capacity()-Alignment;
}

int Bucket::remainingDataSize() const
{
    return capacity()-Alignment-m_dataSize;
}
