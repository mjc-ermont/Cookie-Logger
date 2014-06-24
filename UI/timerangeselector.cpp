#include "timerangeselector.h"

TimeRangeSelector::TimeRangeSelector(QWidget *parent) :
    QHBoxLayout(parent)
{
    min_date = QDateTime(QDate(2014,1,1),QTime(13,37,0));
    max_date = QDateTime(QDate(2014,6,24),QTime(14,42,37));


    range_slider = new QxtSpanSlider(Qt::Orientation::Horizontal);
    date_start = new QLabel(min_date.toString("dd/MM/yy hh:mm:ss"));
    date_end = new QLabel(max_date.toString("dd/MM/yy hh:mm:ss"));

    range_slider->setRange(min_date.toTime_t(), max_date.toTime_t());
    range_slider->setLowerValue(min_date.toTime_t());
    range_slider->setUpperValue(max_date.toTime_t());

    this->addWidget(date_start,0);
    this->addWidget(range_slider,1);
    this->addWidget(date_end,0);

    connect(range_slider,SIGNAL(lowerValueChanged(int)),this,SLOT(onLowerValueChanged(int)));
    connect(range_slider,SIGNAL(upperValueChanged(int)),this,SLOT(onUpperValueChanged(int)));
}

void TimeRangeSelector::onLowerValueChanged(int newValue) {
    date_start->setText(QDateTime::fromTime_t(newValue).toString("dd/MM/yy hh:mm:ss"));
    emit startDateChanged(QDateTime::fromTime_t(newValue));
}

void TimeRangeSelector::onUpperValueChanged(int newValue) {
    date_end->setText(QDateTime::fromTime_t(newValue).toString("dd/MM/yy hh:mm:ss"));
    emit endDateChanged(QDateTime::fromTime_t(newValue));
}

void TimeRangeSelector::setMinimumDate(QDateTime min) {
    min_date = min;
    range_slider->setMinimum(min.toTime_t());
}

void TimeRangeSelector::setMaximumDate(QDateTime max) {
    bool lock_up_slider = (range_slider->upperValue() == max_date.toTime_t());

    max_date = max;
    range_slider->setMaximum(max.toTime_t());

    if(lock_up_slider)
        range_slider->setUpperValue(max.toTime_t());
}
