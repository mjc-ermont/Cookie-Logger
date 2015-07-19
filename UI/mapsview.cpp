#include "mapsview.h"

MapsView::MapsView(QVBoxLayout *parent) : QWebView()
{
    connect(this, SIGNAL(titleChanged(QString)),this,SLOT(titleChanged(QString)));
    mapLoaded = false;

    parent->addWidget(this);

    QFile fichier("://html/maps.html");
    fichier.open(QFile::ReadOnly);
    QString s= QString(fichier.readAll());
    this->setHtml(s);
    this->settings()->setAttribute(QWebSettings::JavascriptEnabled, true);
    this->settings()->setAttribute(QWebSettings::DeveloperExtrasEnabled, true);
    this->settings()->setAttribute(QWebSettings::LocalStorageEnabled, true);
    this->settings()->setAttribute(QWebSettings::OfflineWebApplicationCacheEnabled, true);

    fichier.close();

    hist_x.clear();
    hist_y.clear();


}

void MapsView::addPoint() {

    qDebug() << "adding point! (" << mX << ";" << mY << ")";
    QWebFrame* f = this->page()->mainFrame();
    f->evaluateJavaScript("newPos(" + QString::number(mY) + "," + QString::number(mX) + ")");
    mX = 0;
    mY = 0;
}

void MapsView::updateX(double x) {
    mX = x;
    if((mX != 0)&&(mX <= 180)&&(mX >= -180)&&(mY != 0)&&(mY <= 180)&&(mY >= -180))
        addPoint();
}

void MapsView::updateY(double y) {
    mY = y;
    if((mX != 0)&&(mX <= 180)&&(mX >= -180)&&(mY != 0)&&(mY <= 180)&&(mY >= -180))
        addPoint();

}

void MapsView::titleChanged(QString message) {
    if(message == "loaded") {
        qDebug() << "Mapview loaded!" << message;
        mapLoaded = true;
        if(hist_x.size() > 0 && hist_y.size() > 0)
            onStartData();
    }
}

void MapsView::onStartData() {
    if(!mapLoaded)
        return;

    int end = hist_x.size() > hist_y.size() ? hist_y.size() : hist_x.size();
    for(int i=0;i<end;i++) {
        updateX(hist_x[i].value);
        updateY(hist_y[i].value);
    }

    hist_x.clear();
    hist_y.clear();
}

void MapsView::onStartDataX(QVector<Data> &d) {
    hist_x = d;
    if(hist_x.size() > 0 && hist_x.size() > 0)
        onStartData();
}

void MapsView::onStartDataY(QVector<Data> &d) {
    hist_y = d;
    if(hist_x.size() > 0 && hist_x.size() > 0)
        onStartData();
}
