#ifndef FOCUSTIMER_H
#define FOCUSTIMER_H

#include <QTimer>

class FocusTimer : public QTimer
{
    Q_OBJECT
public:
    Q_PROPERTY(TimerStatus timerStatus MEMBER _running NOTIFY runningChanged)

    explicit FocusTimer(QObject *parent = nullptr);

    enum class TimerStatus {
        Running,
        Paused,
        Expired
    };
    Q_ENUM(TimerStatus)

    QString toString();

public slots:

signals:
    void runningChanged();

private:
    void updateTimer();

    TimerStatus _running = TimerStatus::Paused;
    long long _elapsedSecsSinceEpoch = 0; //temptest

};

#endif // FOCUSTIMER_H
