#ifndef MASKEDAPPLICATIONWINDOW_H
#define MASKEDAPPLICATIONWINDOW_H

#include <QObject>
#include <QQuickWindow>

class MaskedApplicationWindow : public QQuickWindow
{
    Q_OBJECT
    QML_ELEMENT
public:
    explicit MaskedApplicationWindow(QWindow *parent = nullptr);
protected:
    void resizeEvent(QResizeEvent *) override;

signals:
};

#endif // MASKEDAPPLICATIONWINDOW_H
