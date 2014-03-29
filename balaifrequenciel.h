#ifndef BALAIFREQUENCIEL_H
#define BALAIFREQUENCIEL_H

#include <QDialog>
#include <InPut/serial.h>
#include <qwt/qwt_plot.h>
#include "UI/histogram.h"

namespace Ui {
class BalaiFrequenciel;
}

class BalaiFrequenciel : public QwtPlot
{
    Q_OBJECT
    
public:
    explicit BalaiFrequenciel(Serial* com_, QWidget *parent = 0);
    ~BalaiFrequenciel();
    void startBalayage();
    void setData(QVector<double> values);
    
private:
    Serial* com;
    Histogram* histogram;
};

#endif // BALAIFREQUENCIEL_H