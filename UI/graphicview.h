#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H



#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_curve.h>
#include <Qwt/qwt_curve_fitter.h>
#include <Qwt/qwt_legend.h>
#include <Qwt/qwt_plot_zoomer.h>

//#include <Donnees.h>
#include <FenPrincipale.h>
#include <InPut/sensorvalue.h>
#include <InPut/sensor.h>

class FenPrincipale;

namespace Ui {
class FenPrincipale;
}

class MyQwtPlotZoomer : public QwtPlotZoomer{
public:
    MyQwtPlotZoomer(QwtPlotCanvas* cnv) : QwtPlotZoomer(cnv){

    }

    void zoom(const QRectF &rect)
    {
        QRectF newRect;
        const QRectF & baseRect = zoomBase();
        newRect.setCoords( rect.left(), baseRect.top(), rect.right(), baseRect.bottom());
        QwtPlotZoomer::zoom( newRect );
    }
};


class GraphicView : public QwtPlot
{
    Q_OBJECT

    public:
        explicit GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent = 0);
        void majData(QTime n_duration);
        void majData();

    protected:
        void calculateCurve(QTime maxTime = QTime(0,1));
        void majCurve();


    private:
        int m_capteur;
        int m_valeur;

        FenPrincipale *m_parent;

        QwtPlotCurve* courbe;
        MyQwtPlotZoomer* zoomer;

        QVector<double> xValues;
        QVector<double> yValues;

        SensorValue *value;
        QTime duration;
};

#endif // GRAPHICVIEW_H
