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
    this->setAxisScaleDraw(QwtPlot::xBottom, new TimeScaleDraw(QTime(0,0)));
    this->setAxisTitle(QwtPlot::yLeft, value->getName());

    zoomer = new MyQwtPlotZoomer(this->canvas());

    courbe = new QwtPlotCurve("Courbe");
    courbe->setStyle(QwtPlotCurve::Lines);
    courbe->setPen(QPen(QBrush(Qt::black),2));
   /* if(parent->interpol_curve->isChecked())
        courbe->setCurveAttribute(QwtPlotCurve::Fitted);*/

    setWindowTitle(value->getCapteur()->getName() + " - " + value->getName());
    duration = QTime(0,5,0);
    courbe->attach(this);
    majCurve();
}

void GraphicView::majData() {
    value->getData("graph",false,QDateTime::currentDateTime().addMSecs(-TimeCalcs::toMs(duration)));
}

void GraphicView::majData(QTime n_duration) {
    duration=n_duration;
    majData();
}

void GraphicView::majCurve() {
    calculateCurve(duration);
    QwtSplineCurveFitter* fitter = new QwtSplineCurveFitter;

       fitter->setFitMode(fitter->Auto);
       fitter->setSplineSize(50);

       courbe->setCurveFitter(fitter);

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

    foreach(Data d, data) {
       // if(QTime(0,0).secsTo(d->time) >=  maxTime.secsTo(QTime::currentTime())) {
            xValues.append(m_parent->getDepart().secsTo(d.time));
            yValues.append(d.value);
       // }
    }
}


