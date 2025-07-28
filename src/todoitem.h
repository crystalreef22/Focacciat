#ifndef TODOITEM_H
#define TODOITEM_H

#include <QObject>
#include <QPointer>
#include <blocklist.h>

class TodoItem : public QObject {
    Q_OBJECT
    Q_PROPERTY(bool done READ done WRITE setDone NOTIFY doneChanged FINAL)
    Q_PROPERTY(QString description READ description WRITE setDescription NOTIFY descriptionChanged FINAL)
    Q_PROPERTY(long long timeEstimate READ timeEstimate WRITE setTimeEstimate NOTIFY timeEstimateChanged FINAL)
    Q_PROPERTY(long long timeElapsed READ timeElapsed WRITE setTimeElapsed NOTIFY timeElapsedChanged FINAL)
    Q_PROPERTY(long long timeRemaining READ timeRemaining NOTIFY timeRemainingChanged FINAL)
    Q_PROPERTY(bool timerExpired READ timerExpired NOTIFY timerExpiredChanged FINAL)
    Q_PROPERTY(QPointer<Blocklist> blocklist MEMBER _blocklist FINAL)
public:
    explicit TodoItem(QObject *parent = nullptr);

    bool done() const;
    QString description() const;
    long long timeEstimate() const;
    long long timeElapsed() const;
    long long timeRemaining() const;
    bool timerExpired();

    void setDone(bool value);
    void setDescription(QString value);
    void setTimeEstimate(long long value);
    void setTimeElapsed(long long value);

signals:
    void doneChanged();
    void descriptionChanged();
    void timeEstimateChanged();
    void timeElapsedChanged();
    void timeRemainingChanged();
    void timerExpiredChanged();

public slots:
    void updateTimer();
    void resetTimer();
    bool applyBlocklist();

private:
    bool _done;
    QString _description;
    long long _timeEstimate;
    long long _timeElapsed;
    long long _lastResetTime;

    bool _timerExpired;

    QPointer<Blocklist> _blocklist;
};

#endif // TODOITEM_H
