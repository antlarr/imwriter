#include <QApplication>
#include <solid/devicenotifier.h>
#include "imwriter.h"
#include "mainwindow.h"
#include "job.h"

int main(int argc, char**argv)
{
   if (argc==3)
   {
      QCoreApplication app(argc, argv);

      ImWriter imwriter;
      Job *job=imwriter.writeImageToDevice(QUrl::fromLocalFile(QString::fromLocal8Bit(argv[1])), QString::fromLocal8Bit(argv[2]));
      QObject::connect(job, SIGNAL(finished()), qApp, SLOT(quit()));
      return app.exec();      
   }
   
   QApplication app(argc, argv);

//   ImWriter imwriter;
   //imwriter.writeImageToDevice(QStringLiteral(""), QStringLiteral(""));
   MainWindow* window = new MainWindow();
   window->show();

   return app.exec();
}
