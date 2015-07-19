#ifndef MAPSVIEW_H
#define MAPSVIEW_H


#include <defines.h>
#include <FenPrincipale.h>
#include <QWebView>
#include <QWebFrame>
#include <QFile>

class MapsView : public QWebView
{
    Q_OBJECT
public:
    explicit MapsView(QVBoxLayout *parent = 0);

    void addPoint();

    void updateX(double x);
    void updateY(double y);

    void onStartData();
    void onStartDataX(QVector<Data> &d);
    void onStartDataY(QVector<Data> &d);

public slots:
    void titleChanged(QString message);

private:
    QVector<QPair<double,double> > points;
    double mX,mY;

    QVector<Data> hist_x;
    QVector<Data> hist_y;

    bool mapLoaded;
};

#endif // MAPSVIEW_H
