#ifndef HISTOGRAM_H
#define HISTOGRAM_H

#include <qwt/qwt_plot_layout.h>
#include <qwt/qwt_legend.h>
//#include <qwt/qwt_legend_item.h>
#include <qwt/qwt_plot_grid.h>
#include <qwt/qwt_plot_histogram.h>
#include <qwt/qwt_column_symbol.h>
#include <qwt/qwt_series_data.h>

class Histogram: public QwtPlotHistogram
{
public:
    Histogram( const QString &, const QColor & );

    void setColor( const QColor & );
    void setValues( uint numValues, const double * );
};

#endif // HISTOGRAM_H
