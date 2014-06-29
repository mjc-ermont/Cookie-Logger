#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H



#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_curve_fitter.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_scale_draw.h>

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
    TimeScaleDraw(){}

    virtual QwtText label(double v) const
    {
        QDateTime upTime = QDateTime::fromTime_t((int)v);
        return upTime.toString("dd/MM hh:mm:ss");
    }
};
class MyQwtPlotZoomer;

class GraphicView : public QwtPlot
{
    Q_OBJECT

    public:
        explicit GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent = 0);

        void majData();
        double getMin();
        double getMax();

        void setRange(QDateTime start, QDateTime end) {start_dt = start; end_dt = end; majData();}

        void setData(QVector<Data> i_data) { data = i_data; majCurve();}
        int getCapteur() { return m_capteur; }
        int getValeur () { return m_valeur ; }
        void setZoomed(bool zoomed_p) {zoomed = zoomed_p;}
    protected:
        void calculateCurve();
        void majCurve();

     public slots:
        void setStartDT(QDateTime start) { start_dt = start; majData();}
        void setEndDT(QDateTime end) { end_dt = end; majData();}


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

        QDateTime start_dt;
        QDateTime end_dt;

        bool zoomed;
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
        QwtPlot *plt = plot();
        if ( !plt )
            return;

        plt->setAxisAutoScale(yAxis(), true);
        plt->setAxisScale(xAxis(),rect.left(), rect.right());
        plt->replot();

        parent->setZoomed(true);
    }

    void rescale()
    {
        QwtPlot *plt = plot();
        if ( !plt )
            return;

        QwtPlotZoomer::rescale();

        plt->setAxisAutoScale(QwtPlot::yLeft, true);
        plt->setAxisScale(xAxis(),parent->getMin(), parent->getMax());
        plt->replot();

        parent->setZoomed(false);
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
