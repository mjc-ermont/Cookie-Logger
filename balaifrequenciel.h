#ifndef BALAIFREQUENCIEL_H
#define BALAIFREQUENCIEL_H

#include <InPut/serial.h>
#include <QDialog>
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

public slots:
    void balayageDone(QVector<double> values);

private:
    Serial* com;
    Histogram* histogram;
};

#endif // BALAIFREQUENCIEL_H
