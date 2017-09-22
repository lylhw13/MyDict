#include "clickablelabel.h"

ClickableLabel::ClickableLabel(const QString &text,QWidget* parent, Qt::WindowFlags f)
    : QLabel(parent)
{
    setText(text);
}

ClickableLabel::~ClickableLabel() {}

void ClickableLabel::mousePressEvent(QMouseEvent *event)
{
    emit pressed();
}

void ClickableLabel::mouseReleaseEvent(QMouseEvent* event)
{
    emit released();
}
