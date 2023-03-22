#include "spinbox.h"

SpinBox::SpinBox(QWidget *parent): QSpinBox(parent){
	setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
	setAlignment(Qt::AlignCenter);
}

void SpinBox::enterEvent(QEvent* /*event*/){
	setButtonSymbols(QAbstractSpinBox::ButtonSymbols::UpDownArrows);
}

void SpinBox::leaveEvent(QEvent* /*event*/){
	setButtonSymbols(QAbstractSpinBox::ButtonSymbols::NoButtons);
}

void SpinBox::stepBy(int steps)
{
    emit stepEventTrigger(steps);
    QSpinBox::stepBy(steps);
}
