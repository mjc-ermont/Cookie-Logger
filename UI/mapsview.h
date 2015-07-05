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


private:
    QVector<QPair<double,double> > points;
    double mX,mY;
};

#endif // MAPSVIEW_H
