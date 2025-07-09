#include "timeinput.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGUIApplication>
#include <QClipboard>

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

int TimeInput::stringToTime(QString s) {
    int smhd[4] = {};
    size_t smhdI = 0;
    int place = 1;
    for (qsizetype i = s.length() - 1; i >= 0; i--) {
        QChar c = s.at(i);
        if (c.isDigit()) {
            smhd[smhdI] += c.digitValue() * place;
            place *= 10;
        } else {
            if (c == QChar(':') || c == QChar('\t')) {
                smhdI++;
                if (smhdI >= 4) return -1;
                place = 1;
            } else if (QSet<QChar>({' ', '\r', '\n', ','}).contains(c)) {
            } else if (c == QChar('.')) {
                break;
            } else return -1;
        }
    }
    return smhd[0] + (smhd[1] + (smhd[2] + smhd[3] * 24) * 60) * 60;
}

bool TimeInput::setFromString(QString s) {
    int t = stringToTime(s);
    if (t == -1) return false;
    if (t > 86400) t = 86400;
    _internalValue = timeToValue(t);
    emit displayTextChanged();
    return true;
}

bool TimeInput::focus() const { return _focus; }
bool TimeInput::selected() const { return _selected; }

void TimeInput::setSelected(bool selected) {
    _selected = selected;
    emit selectedChanged();
}

void TimeInput::setFocus(bool focus) {
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
    if (modifiers & Qt::ControlModifier) {
        switch (key) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            _internalValue = 0;
            emit displayTextChanged();
            break;
        case Qt::Key_A:
            setSelected(true);
            break;
        case Qt::Key_Insert:
        case Qt::Key_C: {
            QClipboard *clipboard = QGuiApplication::clipboard();
            clipboard->setText(displayText());
            break;
        }
        case Qt::Key_V:
            setFromString(QGuiApplication::clipboard()->text());
            break;
        default:
            return false;
        }
        return true;
    }

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
        if (_selected) {
            _internalValue = 0;
        } else {
            _internalValue /= 10;
        }
        emit displayTextChanged();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        setSelected(true);
        commit();
        break;
    case Qt::Key_Escape:
        if (_selected) {
            setSelected(false);
        } else {
            setSelected(true);
            _internalValue = _committedValue;
            emit selectedChanged();
            emit displayTextChanged();
        }
        break;
    default:
        return false;
    }
    return true;
}

