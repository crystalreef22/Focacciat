#include "maskedapplicationwindow.h"
#include <QDebug>

MaskedApplicationWindow::MaskedApplicationWindow(QWindow *parent)
    : QQuickWindow{parent}
{ }

void MaskedApplicationWindow::resizeEvent(QResizeEvent *ev) {
    const QSize& size = ev->size();
    QQuickWindow::resizeEvent(ev);

    // create a region, masking the top circle. The top circle will have a radius
    // slightly bigger to allow for anti aliasing within the shape
    QRegion region = QRegion(-1, -1, size.width()+2, size.width()+2, QRegion::Ellipse)
                    .united({0, size.width()/2, size.width(), size.height(), QRegion::Rectangle});
    setMask(region);
}
