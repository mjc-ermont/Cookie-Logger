#include "webservicesmanager.h"

WebServicesManager::WebServicesManager()
{
    mSettings = new QSettings();
    net = new QNetworkAccessManager();
}

void WebServicesManager::update(int id_c, int id_v, double value) {

   bool dataserverenabled =  mSettings->value("dataserverenabled", false).toBool();
    if(dataserverenabled) {
        QString url = mSettings->value("dataserverurl","").toString();
        QString requesturl = url + +"?t=token&nc="+QString::number(id_c)+"&nv="+QString::number(id_v)+"&v="+QString::number(value);
        emit message("Requête en cours: "+requesturl);
        QNetworkRequest request(url);

        QNetworkReply *r = net->get(request);
        r->setProperty("test",false);
        connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(erreur(QNetworkReply::NetworkError)));
        connect(r, SIGNAL(finished()), this, SLOT(reponse()));
        connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progression(qint64, qint64) ));
    }

    bool mtwserverenabled = mSettings->value("metewowserverenabled", false).toBool(); //todo: implement this*/
}

void WebServicesManager::test(int type) {
    switch (type) {
        case DATASERVER:
            emit notification(DATASERVER, "Test...");

            if(mSettings->value("dataserverenabled", false).toBool()) {
                emit message("Test du serveur de réception");
                QString url = mSettings->value("dataserverurl","").toString();
                QNetworkRequest requete(url);

                QNetworkReply *r = net->get(requete);
                r->setProperty("t",DATASERVER);
                r->setProperty("test",true);
                connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(erreur(QNetworkReply::NetworkError)));
                connect(r, SIGNAL(finished()), this, SLOT(reponse()));
                connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progression(qint64, qint64) ));
            } else {
                emit notification(DATASERVER, "Désactivé");
            }
            break;
        case METEWOWSERVER:
            emit notification(METEWOWSERVER, "Test...");

            if(mSettings->value("metewowserverenabled", false).toBool()) {
                emit message("Test du serveur météwow");
                QString url = mSettings->value("metewowserverurl","").toString();
                QNetworkRequest requete(url);

                QNetworkReply *r = net->get(requete);
                r->setProperty("t",METEWOWSERVER);
                r->setProperty("test",true);
                connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(erreur(QNetworkReply::NetworkError)));
                connect(r, SIGNAL(finished()), this, SLOT(reponse()));
                connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progression(qint64, qint64) ));
            } else {
                emit notification(METEWOWSERVER, "Désactivé");
            }
            break;
    }
}

void WebServicesManager::metewowRegister() {

}

void WebServicesManager::metewowDelete() {

}

void WebServicesManager::reponse() {
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QString resp = r->readAll();

    QVariant statusCode = r->attribute( QNetworkRequest::HttpStatusCodeAttribute );
    int status;
    if ( !statusCode.isValid() )
        status = -1;
    else
        status = statusCode.toInt();


    if(r->property("test").toBool() == true) {
        if(r->property("t").toInt() == DATASERVER) {
            emit message("Test Serveur de réception: Code "+QString::number(status)+ " ==> taille de la réponse: "+QString::number(resp.size()));
        }else {
            emit message("Test Serveur météwow: Code "+QString::number(status)+ " ==> taille de la réponse: "+QString::number(resp.size()));
        }
    }

    emit notification(r->property("t").toInt(), QString::number(status));
    r->deleteLater();
}

void WebServicesManager::erreur(QNetworkReply::NetworkError) {
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());

    if(r->property("t").toInt() == DATASERVER) {
        emit message("Test Serveur de réception: Erreur ==> " + r->errorString());
    }else {
        emit message("Test Serveur météwow: Erreur ==> "+r->errorString());
    }

    emit notification(r->property("t").toInt(), r->errorString());
    r->deleteLater();
}

void WebServicesManager::progression(qint64, qint64) {}
