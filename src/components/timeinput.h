#ifndef TIMEINPUT_H
#define TIMEINPUT_H

#include <QObject>
#include <QKeyEvent>

class TimeInput : public QObject
{
    Q_OBJECT
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged FINAL)
    Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged FINAL)
    Q_PROPERTY(bool focus READ focus WRITE setFocus NOTIFY focusChanged FINAL)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged FINAL)
public:
    explicit TimeInput(QObject *parent = nullptr);

    QString displayText() const;
    int time() const;
    bool focus() const;
    bool selected() const;

    bool setTime(int secs);
    void setFocus(bool focus);
    void setSelected(bool selected);

signals:
    void displayTextChanged();
    void timeChanged();
    void focusChanged();
    void selectedChanged();
    void editingFinished();

public slots:
    bool handleKeyPress(int key, int modifiers);

private:
    int _internalValue;
    int _committedValue;
    bool _focus;
    bool _selected;
    static int timeToValue(int t);
    static int valueToTime(int v);
    void commit();
};

#endif // TIMEINPUT_H
