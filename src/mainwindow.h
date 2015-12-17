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

#ifndef MAINWINDOW_H
#define MAINWINDOW_H
#include <KXmlGuiWindow>
#include <QWidget>
#include <QListView>
#include <QLabel>
#include <QStackedWidget>
#include "imwriter.h"
#include "imagelistmodel.h"
#include "devicemodel.h"

class MainWindow : public KXmlGuiWindow
{
   Q_OBJECT

public:
   MainWindow();
   ~MainWindow();
    
public slots:
   void addImage();
   void noAvailableDevices(bool noAvailableDevices);
private:
   QWidget* m_mainArea;
   QListView *m_imagesView;
   QListView *m_devicesView;
   QLabel *m_devicesLabel;
   QStackedWidget *m_devStack;
   ImageListModel *m_imageListModel;
   DeviceModel *m_deviceModel;

   ImWriter *m_imWriter;   
   
   void setupActions();
};

#endif // MAINWINDOW_H
