#include "mygraphicsview.h"

MyGraphicsView::MyGraphicsView(QWidget* parent) : QGraphicsView(parent)
{
    clicked = false;
}


void MyGraphicsView::mouseMoveEvent(QMouseEvent *event) {
    if(clicked) {
        int offset = event->x() - startX;
        int elementWidth = ELEMENT_WIDTH;

        if(this->width() / N_STAGES > elementWidth)
            elementWidth = this->width() / N_STAGES;



        int width = elementWidth*N_STAGES - this->width();

        int value = this->horizontalScrollBar()->value() - offset;
        if(value > width)
            value = width;
        this->horizontalScrollBar()->setValue(value);

        startX = event->x();
    }
}

void MyGraphicsView::mousePressEvent(QMouseEvent *event) {
    clicked = true;
    startX =  event->x();
}

void MyGraphicsView::mouseReleaseEvent(QMouseEvent *event) {
    clicked = false;
}
