#include "graphicview.h"


GraphicView::GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent) :
    QwtPlot(parent), m_capteur(indexCapteur), m_valeur(indexValeur) {
    m_parent = parent;
    m_xy = false;
    value = m_parent->getSensorMgr()->getSensor(indexCapteur)->getValues().at(indexValeur);


    this->setTitle(value->getCapteur()->getName());
    this->setCanvasBackground(QColor(Qt::white));

    this->setAutoReplot(false);


    m_capteur_x=0;
    m_valeur_x=0;
    m_capteur=0;
    m_valeur=0;

    zoomed = false;

    // axis
    this->setAxisTitle(QwtPlot::xBottom, "Temps");
    this->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw());
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

GraphicView::GraphicView(int indexCapteur, int indexValeur,int indexCapteur_x, int indexValeur_x, FenPrincipale *parent) :
    QwtPlot(parent), m_capteur(indexCapteur), m_valeur(indexValeur), m_capteur_x(indexCapteur_x), m_valeur_x(indexValeur_x) {
    m_xy = true;

    m_parent = parent;
    value = m_parent->getSensorMgr()->getSensor(indexCapteur)->getValues().at(indexValeur);
    value_x = m_parent->getSensorMgr()->getSensor(indexCapteur_x)->getValues().at(indexValeur_x);


    this->setTitle(value->getCapteur()->getName());
    this->setCanvasBackground(QColor(Qt::white));

    this->setAutoReplot(false);

    zoomed = false;

    // axis
    this->setAxisTitle(QwtPlot::xBottom, value_x->getName());
    this->setAxisTitle(QwtPlot::yLeft, value->getName());

    this->setAxisAutoScale(QwtPlot::yLeft, true);

    zoomer = new MyQwtPlotZoomer(this->canvas(),this);

    courbe = new QwtPlotCurve("Courbe");
    courbe->setStyle(QwtPlotCurve::Dots);
    courbe->setPen(QPen(QBrush(Qt::black),10));

    setWindowTitle(value->getCapteur()->getName() + " - " + value->getName() + " | " + value_x->getCapteur()->getName() + " - " + value_x->getName() );


    courbe->attach(this);
    majCurve();
}




void GraphicView::majData() {
    value->getData("graph",false,start_dt, end_dt);
}


void GraphicView::majCurve() {
    calculateCurve();
    QwtSplineCurveFitter* fitter = new QwtSplineCurveFitter;

       fitter->setFitMode(fitter->Spline);
       fitter->setSplineSize(50);

       courbe->setCurveFitter(fitter);

    courbe->setRawSamples(xValues.data(),yValues.data(),xValues.size());
    if(xValues.size() == 0) {
        replot();
        return;
    }

    if(!zoomed) {
        this->setAxisScale(QwtPlot::xBottom, getMin()-10, getMax()+10);
        this->setAxisScale(QwtPlot::yLeft, getMinX()-10, getMaxX()+10);
    }
    replot();
}

double GraphicView::getMin() {
    if(xValues.isEmpty())
        return 0;
    int min = xValues.first();
    foreach(int v, xValues)
        if (v < min)
            min = v;
    return min;
}

double GraphicView::getMax() {
    if(xValues.isEmpty())
        return 1;
    int max = xValues.first();
    foreach(int v, xValues)
        if (v > max)
            max = v;
    return max;
}

double GraphicView::getMinX() {
    if(yValues.isEmpty())
        return 0;
    int min = yValues.first();
    foreach(int v, yValues)
        if (v < min)
            min = v;
    return min;
}

double GraphicView::getMaxX() {
    if(yValues.isEmpty())
        return 1;
    int max = yValues.first();
    foreach(int v, yValues)
        if (v > max)
            max = v;
    return max;
}

void GraphicView::calculateCurve() {
    yValues.clear();
    xValues.clear();

    if(m_xy) {
        int s = data.size() > data_x.size() ? data_x.size() : data.size();
        qDebug() << "s:" << s << "x:" << data_x.size() << "y" << data.size();
        for(int i=0;i<s;i++) {
            xValues.append(data_x[i].value);
            yValues.append(data[i].value);
        }
    } else {
        foreach(Data d, data) {
            xValues.append(d.time.toTime_t());
            yValues.append(d.value);
        }
    }
}


