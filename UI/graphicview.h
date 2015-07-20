#ifndef GRAPHICVIEW_H
#define GRAPHICVIEW_H


#include <FenPrincipale.h>
#include <input/sensorvalue.h>
#include <input/sensor.h>

#include <qwt/qwt_plot_canvas.h>
#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_curve.h>
#include <qwt/qwt_curve_fitter.h>
#include <qwt/qwt_legend.h>
#include <qwt/qwt_plot_zoomer.h>
#include <qwt/qwt_scale_draw.h>



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

        bool isXY() {return m_xy;}

        // Mode temporel
        explicit GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent = 0);
        void setData(QVector<Data> i_data) { data = i_data; majCurve();}
        int getCapteur() { return m_capteur; }
        int getValeur () { return m_valeur ; }

        // Mode X-Y
        explicit GraphicView(int indexCapteur, int indexValeur, int indexCapteur_x, int indexValeur_x, FenPrincipale *parent = 0);
        void setDataX(QVector<Data> i_data_x) { data_x = i_data_x;majCurve();}
        void setDataY(QVector<Data> i_data_y) { data = i_data_y; }
        int getCapteur_x() { return m_capteur_x; }
        int getValeur_x () { return m_valeur_x ; }

        void majData();
        double getMin();
        double getMax();
        double getMinX();
        double getMaxX();

        void setRange(QDateTime start, QDateTime end) {start_dt = start; end_dt = end; majData();}

        void setZoomed(bool zoomed_p) {zoomed = zoomed_p;}
    protected:
        void calculateCurve();
        void majCurve();

     public slots:
        void setStartDT(QDateTime start) { start_dt = start;}
        void setEndDT(QDateTime end) { end_dt = end;}


    private:
        bool m_xy;

        int m_capteur;
        int m_valeur;
        int m_capteur_x;
        int m_valeur_x;

        FenPrincipale *m_parent;

        QwtPlotCurve* courbe;
        MyQwtPlotZoomer* zoomer;

        QVector<double> xValues;
        QVector<double> yValues;
        QVector<Data>   data;
        QVector<Data>   data_x;

        SensorValue *value;
        SensorValue *value_x;

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
