#include "timerangeselector.h"

TimeRangeSelector::TimeRangeSelector(QWidget *parent) :
    QHBoxLayout(parent)
{
    min_date = QDateTime::currentDateTime();
    max_date = QDateTime::currentDateTime();


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

    connect(range_slider,SIGNAL(sliderPressed()),this,SLOT(sliderPressed()));
    connect(range_slider,SIGNAL(sliderReleased()),this,SLOT(sliderReleased()));
}

void TimeRangeSelector::sliderPressed() {

    startLval = range_slider->lowerValue();
    endLval = range_slider->upperValue();
}

void TimeRangeSelector::sliderReleased() {

    if(startLval != range_slider->lowerValue())
        emit startDateChanged(this->getLowerDate());

    if(endLval != range_slider->upperValue())
        emit endDateChanged(this->getUpperDate());
}

void TimeRangeSelector::onLowerValueChanged(int newValue) {
    date_start->setText(QDateTime::fromTime_t(newValue).toString("dd/MM/yy hh:mm:ss"));
}

void TimeRangeSelector::onUpperValueChanged(int newValue) {
    date_end->setText(QDateTime::fromTime_t(newValue).toString("dd/MM/yy hh:mm:ss"));
}

QDateTime TimeRangeSelector::getLowerDate() {
    return QDateTime::fromTime_t(range_slider->lowerValue());
}

QDateTime TimeRangeSelector::getUpperDate() {
    return QDateTime::fromTime_t(range_slider->upperValue());
}

void TimeRangeSelector::setMinimumDate(QDateTime min) {
    min_date = min;
    range_slider->setMinimum(min.toTime_t());
    range_slider->setLowerValue(min.toTime_t());
}

void TimeRangeSelector::setMaximumDate(QDateTime max) {
    bool lock_up_slider = (range_slider->upperValue() == max_date.toTime_t());

    max_date = max;
    range_slider->setMaximum(max.toTime_t());

    if(lock_up_slider) {
        range_slider->setUpperValue(max.toTime_t());
        emit endDateChanged(this->getUpperDate());
    }

}
