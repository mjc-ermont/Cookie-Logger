#ifndef TIMERANGESELECTOR_H
#define TIMERANGESELECTOR_H

#include <QtGui/QHBoxLayout>
#include <QtGui/QLabel>
#include <QDateTime>
#include "qxt/qxtspanslider.h"

class TimeRangeSelector : public QHBoxLayout
{
    Q_OBJECT
public:
    explicit TimeRangeSelector(QWidget *parent = 0);
    void setMinimumDate(QDateTime min);
    void setMaximumDate(QDateTime max);

signals:
    void startDateChanged(QDateTime start);
    void endDateChanged(QDateTime end);

public slots:
    void onLowerValueChanged(int newValue);
    void onUpperValueChanged(int newValue);

private:
    QxtSpanSlider* range_slider;
    QLabel* date_start;
    QLabel* date_end;

    QDateTime min_date;
    QDateTime max_date;

};

#endif // TIMERANGESELECTOR_H
