#include "webservicesmanager.h"

WebServicesManager::WebServicesManager(FenPrincipale* parent)
{
    mSettings = new QSettings();
    net = new QNetworkAccessManager();
    mParent = parent;
}

void WebServicesManager::update(int id_c, int id_v, double value) {

   bool dataserverenabled =  mSettings->value("dataserverenabled", false).toBool();
    if(dataserverenabled) {
        QString url = mSettings->value("dataserverurl","").toString();
        QString requesturl = url + +"?t=token&nc="+QString::number(id_c)+"&nv="+QString::number(id_v)+"&v="+QString::number(value);
        emit message("Requête en cours: "+requesturl);
        QNetworkRequest request(url);

        QNetworkReply *r = net->get(request);
        r->setProperty("t",DATASERVER);
        r->setProperty("test",false);
        connect(r, SIGNAL(error(QNetworkReply::NetworkError)), this, SLOT(erreur(QNetworkReply::NetworkError)));
        connect(r, SIGNAL(finished()), this, SLOT(reponse()));
        connect(r, SIGNAL(downloadProgress(qint64, qint64)), this, SLOT(progression(qint64, qint64) ));
    }

    bool mtwserverenabled = mSettings->value("metewowserverenabled", false).toBool();
    if(mtwserverenabled) {
        QString url = mSettings->value("metewowserverurl","").toString();
        QString id = mSettings->value("metewowid","").toString();
        QString passwd = mSettings->value("metewowmdp","").toString();
        int onlineid = mSettings->value("c"+QString::number(id_c)+"v"+QString::number(id_v),-1).toInt();
        if(onlineid == -1) {
            emit message("Logger non enregistré sur MétéWow");
            return;
        }

        QString requesturl = url + "send.php?add=data&mac="+id+"&secret="+passwd+"&sensor="+QString::number(onlineid)+"&value="+QString::number(value);
        QNetworkRequest request(requesturl);
        net->get(request);

        emit message("Requête en cours: "+requesturl);
    }

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
    QString id = mSettings->value("metewowid","").toString();
    QString passwd = mSettings->value("metewowmdp","").toString();
    QString url = mSettings->value("metewowserverurl","").toString();

    QNetworkRequest requete(url+"send.php?add=server&mac="+id+"&secret="+passwd);
    QNetworkReply *r = net->get(requete);
    emit message("Requête: "+url+"send.php?add=server&mac="+id+"&secret="+passwd);

    connect(r, SIGNAL(finished()), this, SLOT(metewowRegisterResponse()));
}

void WebServicesManager::metewowRegisterResponse() {
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QString resp = r->readAll();


    QString id = mSettings->value("metewowid","").toString();
    QString passwd = mSettings->value("metewowmdp","").toString();
    QString url = mSettings->value("metewowserverurl","").toString();

    SensorManager *mgr = mParent->getSensorMgr();
    foreach(Sensor* s, mgr->getSensors()) {
        foreach(SensorValue* sv, s->getValues()) {
            QNetworkReply *r;
            if(sv->getName() == "Valeur") {
                QNetworkRequest requete(url+"send.php?add=sensor&mac="+id+"&secret="+passwd+"&name="+s->getName()+"&unit="+sv->getUnit()+"&cate=0");
                r = net->get(requete);
            } else {
                QNetworkRequest requete(url+"send.php?add=sensor&mac="+id+"&secret="+passwd+"&name="+s->getName()+ " - "+sv->getName()+"&unit="+sv->getUnit()+"&cate=0");
                r = net->get(requete);
            }
            r->setProperty("sensorid",s->getId());
            r->setProperty("valueid",sv->getID());
            connect(r, SIGNAL(finished()), this, SLOT(metewowRegisterSensorResponse()));

        }
    }
}

void WebServicesManager::metewowRegisterSensorResponse() {
    QNetworkReply *r = qobject_cast<QNetworkReply*>(sender());
    QString resp = r->readAll();
    bool ok;
    int idsensor = resp.toInt(&ok);
    if(!ok) {
        emit message("Erreur dans l'ajout du capteur");
        return;
    }

    mSettings->setValue("c"+QString::number(r->property("sensorid").toInt())+"v"+QString::number(r->property("valueid").toInt()), idsensor);
}

void WebServicesManager::metewowDelete() {
    QString id = mSettings->value("metewowid","").toString();
    QString passwd = mSettings->value("metewowmdp","").toString();
    QString url = mSettings->value("metewowserverurl","").toString();

    QNetworkRequest requete(url+"delete.php?mac="+id+"&secret="+passwd);
    net->get(requete);
    emit message("Requête: "+url+"delete.php?mac="+id+"&secret="+passwd);
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
        emit message("Serveur de réception: Erreur ==> " + r->errorString());
    }else {
        emit message("Serveur météwow: Erreur ==> "+r->errorString());
    }

    emit notification(r->property("t").toInt(), r->errorString());
    r->deleteLater();
}

void WebServicesManager::progression(qint64, qint64) {}
