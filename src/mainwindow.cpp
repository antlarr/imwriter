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

#include "mainwindow.h"
#include <QAction>
#include <QApplication>
#include <QStackedWidget>
#include <QListView>
#include <QHBoxLayout>
#include <KStandardAction>
#include <KActionCollection>
#include <QStringListModel>
#include <QSplitter>
#include <klocalizedstring.h>
#include "devicemodel.h"
#include "imagelistmodel.h"

MainWindow::MainWindow()
    : KXmlGuiWindow(0L), m_imWriter(new ImWriter)
{
  m_mainArea = new QSplitter(this);
  
  m_imagesView = new QListView(this);
  m_imagesView->setViewMode( QListView::IconMode );
//  m_imagesView->setLayoutMode(QListView::Batched);
  m_imagesView->setDragEnabled(true);
  m_imagesView->setUniformItemSizes(true);
  m_imagesView->setResizeMode( QListView::Adjust );
  m_imagesView->setIconSize(QSize(64,64));  
  //m_imagesView->setGridSize(QSize(256,128));  
  
  m_devicesLabel = new QLabel( this );
  m_devicesLabel->setAlignment(Qt::AlignHCenter | Qt::AlignVCenter);
  m_devicesLabel->setScaledContents(true);
  m_devicesLabel->setText(QStringLiteral("<big>")+i18n("Please, insert a usb drive")+QStringLiteral("</big>"));
  m_devicesView = new QListView( this );
  m_devicesView->setViewMode( QListView::IconMode );
  m_devicesView->setIconSize(QSize(64,64));  
  ImageListModel *m_imageListModel=ImageListModel::self();
  m_imagesView->setModel(m_imageListModel);
  DeviceModel *m_deviceModel=new DeviceModel();
  m_devicesView->setModel(m_deviceModel);

  QObject::connect(m_deviceModel, SIGNAL(writeImageToDevice(QUrl, QString)), 
	   m_imWriter, SLOT(writeImageToDevice(QUrl, QString)) );
  m_devStack=0L;
  QObject::connect(m_deviceModel, SIGNAL(noAvailableDevices(bool)), 
	   this, SLOT(noAvailableDevices(bool)) );
  
  m_devStack=new QStackedWidget( this );
  m_devStack->addWidget(m_devicesView);
  m_devStack->addWidget(m_devicesLabel);
  
  QHBoxLayout *layout = new QHBoxLayout;
  layout->addWidget(m_imagesView);
  layout->addWidget(m_devStack);
  m_mainArea->setLayout(layout);
  
  setCentralWidget(m_mainArea);

  noAvailableDevices( m_deviceModel->count() == 0 );
  setupActions();
}
 
void MainWindow::setupActions()
{
  QAction* addImageAction = new QAction(this);
  addImageAction->setText(i18n("&Add Image..."));
  addImageAction->setIcon(QIcon::fromTheme(QStringLiteral("document-new")));
  actionCollection()->setDefaultShortcut(addImageAction, Qt::CTRL + Qt::Key_W);
  actionCollection()->addAction(QStringLiteral("addImage"), addImageAction);
  connect(addImageAction, SIGNAL(triggered(bool)),
          m_mainArea, SLOT(addImage()));
 
  KStandardAction::quit(qApp, SLOT(quit()), actionCollection());
 
  setupGUI();
}

MainWindow::~MainWindow()
{
}

void MainWindow::addImage()
{
   printf("hola\n");
}

void MainWindow::noAvailableDevices(bool noAvailableDevices)
{
   if (noAvailableDevices)
   {
      m_devStack->setCurrentWidget(m_devicesLabel);
   }
   else
   {
      m_devStack->setCurrentWidget(m_devicesView);
   }
}

#include "mainwindow.moc"
