#ifndef FOCUSTIMER_H
#define FOCUSTIMER_H

#include <QTimer>

class FocusTimer
    : public QObject
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


public slots:
    void start(long long timerLength);

signals:
    void runningChanged();
    void timerUpdated(long long elapsedMs);

private:
    void updateTimer();

    TimerStatus _running = TimerStatus::Paused;
    long long _elapsedMsSinceEpoch;
    QTimer _timer;

};

#endif // FOCUSTIMER_H
