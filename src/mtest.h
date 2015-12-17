#include <QObject>
#include <QNetworkAccessManager>
#include <QThread>

class Test : public QThread
{
   Q_OBJECT
   
public:
   Test();
   
protected:
   void run();
   
public slots:
   void finished();
   void replyFinished(QNetworkReply*);
};