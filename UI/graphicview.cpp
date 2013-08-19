#include "graphicview.h"

GraphicView::GraphicView(int indexCapteur, int indexValeur, FenPrincipale *parent) :
    QwtPlot(parent), m_capteur(indexCapteur), m_valeur(indexValeur) {
    m_parent = parent;
    value = m_parent->getSensorMgr()->getSensor(indexCapteur)->getValues().at(indexValeur);


    this->setTitle(value->getCapteur()->getName());
    this->setCanvasBackground(QColor(Qt::white));

    this->setAutoReplot(false);


    /*QwtLegend *legend = new QwtLegend;
    legend->setFrameStyle(QFrame::Box|QFrame::Sunken);
    this->insertLegend(legend, QwtPlot::BottomLegend);*/

    // axis
    this->setAxisTitle(QwtPlot::xBottom, "Temps");
    this->setAxisTitle(QwtPlot::yLeft, value->getName());


    courbe = new QwtPlotCurve("Courbe");
    courbe->setStyle(QwtPlotCurve::Lines);
    courbe->setPen(QPen(QBrush(Qt::black),2));
  //  courbe->setCurveAttribute(QwtPlotCurve::Fitted);

    setWindowTitle(value->getCapteur()->getName() + " - " + value->getName());

    duration = QTime(0,5,0);
    courbe->attach(this);
    majCurve();
}

void GraphicView::majData() {
    majCurve();
}

void GraphicView::majData(QTime n_duration) {
    duration=n_duration;
    majCurve();
}

void GraphicView::majCurve() {
    calculateCurve(duration);

    courbe->setRawSamples(xValues.data(),yValues.data(),xValues.size());
    if(xValues.size() == 0) {
        replot();
        return;
    }

    double min = xValues.at(xValues.size()-1)-duration.hour()*3600 - duration.minute()* 60-duration.second();
    min = min < 0.0 ? 0 : min;
    this->setAxisScale(QwtPlot::xBottom, min, xValues.at(xValues.size()-1));
   // this->setAxisAutoScale(QwtPlot::xBottom, false);
   // this->setAxisScale(, 42, 142);
    replot();
}

void GraphicView::calculateCurve(QTime maxTime) {
    yValues.clear();
    xValues.clear();
    foreach(Data *d, value->getData()) {
       // if(QTime(0,0).secsTo(d->time) >=  maxTime.secsTo(QTime::currentTime())) {
            xValues.append(m_parent->getDepart().secsTo(d->time));
            yValues.append(d->value);
       // }
    }

    xValues.append(0);
    yValues.append(12);
    xValues.append(10);
    yValues.append(13);
    xValues.append(30);
    yValues.append(10);
    xValues.append(60);
    yValues.append(15);
    xValues.append(180);
    yValues.append(-9);
    xValues.append(900);
    yValues.append(-5);
    xValues.append(3000);
    yValues.append(2);
}
