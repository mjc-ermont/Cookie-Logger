#include "mapsview.h"

MapsView::MapsView(QVBoxLayout *parent) : QWebView()
{
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

}

void MapsView::addPoint() {

    qDebug() << "Map";

    QWebFrame* f = this->page()->mainFrame();
    f->evaluateJavaScript("newPos(" + QString::number(mY) + "," + QString::number(mX) + ")");
    qDebug() << "NOUVEAU POINT SUR LA CARTE : newPos(" << QString::number(mY) << "," << QString::number(mX) << ")";
    qDebug() << "FMap";

}

void MapsView::updateX(double x) {
    mX = x;
    if((mX != 42)&&(mX != 99999999)&&(mY != 42)&&(mY != 99999999))
        addPoint();
}

void MapsView::updateY(double y) {
    mY = y;
    if((mX != 42)&&(mX != 99999999)&&(mY != 42)&&(mY != 99999999))
        addPoint();
}
