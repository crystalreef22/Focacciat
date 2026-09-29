#include "maskedapplicationwindow.h"
#include <QDebug>
#import <Cocoa/Cocoa.h>

MaskedApplicationWindow::MaskedApplicationWindow(QWindow *parent)
    : QQuickWindow{parent}
{}

void MaskedApplicationWindow::resizeEvent(QResizeEvent *ev)
{
    const QSize &size = ev->size();
    QQuickWindow::resizeEvent(ev);

    // create a region, masking the top circle. The top circle will have a radius
    // slightly bigger to allow for anti aliasing within the shape
    QRegion region = QRegion(-1, -1, size.width() + 2, size.width() + 2, QRegion::Ellipse)
                         .united(
                             {0, size.width() / 2, size.width(), size.height(), QRegion::Rectangle});
    setMask(region);
}

void MaskedApplicationWindow::showEvent(QShowEvent *ev) {
    WId windowId = winId();

    NSView *nativeView = reinterpret_cast<NSView *>(windowId);
    NSWindow *nativeWindow = nativeView.window; // but isn't that a pointer?
    nativeWindow.collectionBehavior |= NSWindowCollectionBehaviorCanJoinAllSpaces;
}
