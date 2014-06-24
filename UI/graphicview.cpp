#include "graphicview.h"


GraphicView::GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent) :
    QwtPlot(parent), m_capteur(indexCapteur), m_valeur(indexValeur) {
    m_parent = parent;
    value = m_parent->getSensorMgr()->getSensor(indexCapteur)->getValues().at(indexValeur);


    this->setTitle(value->getCapteur()->getName());
    this->setCanvasBackground(QColor(Qt::white));

    this->setAutoReplot(false);



    // axis
    this->setAxisTitle(QwtPlot::xBottom, "Temps");
    this->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(QTime(0,0)));
    this->setAxisTitle(QwtPlot::yLeft, value->getName());

    zoomer = new MyQwtPlotZoomer(this->canvas(),this);

    courbe = new QwtPlotCurve("Courbe");
    courbe->setStyle(QwtPlotCurve::Lines);
    courbe->setPen(QPen(QBrush(Qt::black),2));

    setWindowTitle(value->getCapteur()->getName() + " - " + value->getName());


    setRange(QDateTime::fromTime_t(0), QDateTime::currentDateTime());

    courbe->attach(this);
    majCurve();
}

void GraphicView::majData() {
    value->getData("graph",false,start_dt, end_dt);
}


void GraphicView::majCurve() {
    calculateCurve();
    QwtSplineCurveFitter* fitter = new QwtSplineCurveFitter;

       fitter->setFitMode(fitter->Auto);
       fitter->setSplineSize(50);

       courbe->setCurveFitter(fitter);

    courbe->setRawSamples(xValues.data(),yValues.data(),xValues.size());
    if(xValues.size() == 0) {
        replot();
        return;
    }

    this->setAxisScale(QwtPlot::xBottom, getMin(), getMax());
   // this->setAxisAutoScale(QwtPlot::xBottom, false);
   // this->setAxisScale(, 42, 142);
    replot();
}

double GraphicView::getMin() {

    return start_dt.toTime_t();
}

double GraphicView::getMax() {
    return end_dt.toTime_t();
}

void GraphicView::calculateCurve() {
    yValues.clear();
    xValues.clear();

    foreach(Data d, data) {
        xValues.append(d.time.toTime_t());
        yValues.append(d.value);
    }
}


