#include "timeinput.h"
#include <QDebug>
#include <QKeyEvent>

TimeInput::TimeInput(QObject *parent)
    : QObject{parent}
{}

int TimeInput::timeToValue(int t) {
    int val;

    val = t % 60; // secs
    t /= 60;
    val += (t % 60) * 100; // mins
    t /= 60;
    val += (t % 60) * 10000; // hours
    return val;
}

int TimeInput::valueToTime(int v) {
    return (v / 10000  ) * 3600  // hours
         + ((v / 100) % 100) * 60    // mins
         +  v % 100                    // secs
    ;
}

bool TimeInput::focus() const { return _focus; }
bool TimeInput::selected() const { return _selected; }

void TimeInput::setSelected(bool selected) {
    _selected = selected;
    emit selectedChanged();
}

void TimeInput::setFocus(bool focus) {
    bool initialFocus = _focus;
    _focus = focus;
    if (!focus) {
        commit();
        setSelected(false);
    } else {
        setSelected(true);
    }
    emit focusChanged();
}

void TimeInput::commit() {
    _internalValue = timeToValue(std::clamp(valueToTime(_internalValue),0,86400));
    _committedValue = _internalValue;
    emit timeChanged();
    emit displayTextChanged();
    emit editingFinished();
}

QString TimeInput::displayText() const {
    return QString("%1:%2:%3")
        .arg(( _internalValue / 10000   )      , 2, 10, QChar('0')) // hours
        .arg(( _internalValue / 100     ) % 100, 2, 10, QChar('0')) // mins
        .arg(( _internalValue / 1       ) % 100, 2, 10, QChar('0'));// secs
}

int TimeInput::time() const {
    return valueToTime(_committedValue);
}

bool TimeInput::setTime(int secs) {
    if (secs < 0) {
        qWarning() << "timeinput.h: tried to set negative time";
        return false;
    }
    _internalValue = timeToValue(secs);
    emit timeChanged();
    if (!_focus) {
        _committedValue = _internalValue;
        emit displayTextChanged();
    }
    return true;
}

bool TimeInput::handleKeyPress(int key, int modifiers) {
    if (key >= Qt::Key_0 && key <= Qt::Key_9) {
        if (_selected) {
            _internalValue = 0;
            setSelected(false);
        }
        if (_internalValue > 99999) {
            return false;
        }
        _internalValue = _internalValue * 10 + key - Qt::Key_0; // append
        emit displayTextChanged();
        return true;
    }
    switch (key) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        if (modifiers & Qt::ControlModifier || _selected) {
            _internalValue = 0;
        } else {
            _internalValue /= 10;
        }
        emit displayTextChanged();
        return true;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        setSelected(true);
        commit();
        return true;
    case Qt::Key_Escape:
        if (_selected) {
            setSelected(false);
        } else {
            setSelected(true);
            _internalValue = _committedValue;
            emit selectedChanged();
            emit displayTextChanged();
        }
        return true;
    }
    return false;
}

