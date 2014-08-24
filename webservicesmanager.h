#ifndef WEBSERVICESMANAGER_H
#define WEBSERVICESMANAGER_H

#include "QSettings"

#include <QNetworkReply>
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QUrl>

#include "QDebug"

#define DATASERVER 1
#define METEWOWSERVER 2

class WebServicesManager : public QObject
{
    Q_OBJECT
public:
    WebServicesManager();
    void update(int id_c, int id_v, double value);
    void test(int type);

    void metewowRegister();
    void metewowDelete();
public slots:
    void reponse();
    void erreur(QNetworkReply::NetworkError);
    void progression(qint64, qint64);
signals:
    void notification(int, QString);
    void message(QString);
private:
    QSettings *mSettings;
    QNetworkAccessManager *net;
};

#endif // WEBSERVICESMANAGER_H
