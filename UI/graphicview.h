#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H



#include <Qwt/qwt_plot.h>
#include <Qwt/qwt_plot_curve.h>
#include <Qwt/qwt_curve_fitter.h>
#include <Qwt/qwt_legend.h>
#include <Qwt/qwt_plot_zoomer.h>
#include <Qwt/qwt_scale_draw.h>

#include <FenPrincipale.h>
#include <InPut/sensorvalue.h>
#include <InPut/sensor.h>


class FenPrincipale;

namespace Ui {
class FenPrincipale;
}


class TimeScaleDraw: public QwtScaleDraw
{
public:
    TimeScaleDraw(const QTime &base):
        baseTime(base)
    {
    }
    virtual QwtText label(double v) const
    {
        QTime upTime = baseTime.addSecs((int)v);
        return upTime.toString();
    }
private:
    QTime baseTime;
};
class MyQwtPlotZoomer;

class GraphicView : public QwtPlot
{
    Q_OBJECT

    public:
        explicit GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent = 0);
        void majData(QTime n_duration);
        void majData();
        double getMin();
        double getMax();


        void setData(QVector<Data> i_data) { data = i_data; majCurve();}
        int getCapteur() { return m_capteur; }
        int getValeur () { return m_valeur ; }
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
        QVector<Data>   data;

        SensorValue *value;
        QTime duration;
};


class MyQwtPlotZoomer : public QwtPlotZoomer{
private:
    GraphicView* parent;
public:
    MyQwtPlotZoomer(QwtPlotCanvas* cnv, GraphicView* _parent) : QwtPlotZoomer(cnv){
        parent=_parent;
    }

    void zoom(const QRectF &rect)
    {
        QRectF newRect;
        const QRectF & baseRect = zoomBase();
        newRect.setCoords( rect.left(), baseRect.top(), rect.right(), baseRect.bottom());
        QwtPlotZoomer::zoom( newRect );

    }

    void rescale()
    {
        QwtPlot *plt = plot();
        if ( !plt )
            return;

        QwtPlotZoomer::rescale();

        plt->setAxisAutoScale(yAxis(), true);
       // plt->setAxisScale(xAxis(),parent->getMin(), parent->getMax());
        plt->replot();
    }

    QPolygon adjustedPoints(const QPolygon &points) const
    {

        QPolygon adjusted;

        const QwtPlot *plt = plot();
        if ( !plt )
            return adjusted;

        if ( points.size() == 2 )
        {
            const int width = points[1].x() - points[0].x();

            QRect rect(points[0].x(), 0, width, plt->height());

            adjusted += rect.topLeft();
            adjusted += rect.bottomRight();
        }
        return adjusted;
    }
};

#endif // GRAPHICVIEW_H
