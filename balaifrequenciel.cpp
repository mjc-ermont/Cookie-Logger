#include "balaifrequenciel.h"
#include "ui_balaifrequenciel.h"

#include <qwt/qwt_plot.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_marker.h>

#include <qwt/qwt_plot_histogram.h>

#include <QtGlobal>

BalaiFrequenciel::BalaiFrequenciel(Serial *com_, QWidget *parent) :
    QwtPlot(parent)
{
    com = com_;


    setAxisTitle( QwtPlot::yLeft, "Test" );
    setAxisTitle( QwtPlot::xBottom, "Test" );

    QwtLegend *legend = new QwtLegend;
    legend->setItemMode( QwtLegend::CheckableItem );
    insertLegend( legend, QwtPlot::RightLegend );

    QwtPlotGrid *grid = new QwtPlotGrid;
    grid->enableX( false );
    grid->enableY( true );
    grid->enableXMin( false );
    grid->enableYMin( false );
    grid->setMajPen( QPen( Qt::black, 0, Qt::DotLine ) );
    grid->attach(this );


    histogram = new Histogram("",Qt::red);
    histogram->attach(this);
    replot();
    connect(com, SIGNAL(balayageDone(QVector<double>)), this, SLOT(balayageDone(QVector<double>)));

    startBalayage();


}

void BalaiFrequenciel::balayageDone(QVector<double> values) {
    setData(values);
}

void BalaiFrequenciel::startBalayage() {
    com->balayageFrequenciel();
}

void BalaiFrequenciel::setData(QVector<double> values) {
    histogram->setValues(values.size(), &values.toStdVector()[0] );
   replot();
}

BalaiFrequenciel::~BalaiFrequenciel()
{
}


