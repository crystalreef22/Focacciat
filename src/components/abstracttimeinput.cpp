#include "abstracttimeinput.h"
#include <QDebug>
#include <QKeyEvent>
#include <QGuiApplication>
#include <QClipboard>
#include <QFlags>

AbstractTimeInput::AbstractTimeInput(QObject *parent)
    : QObject{parent}
{}

int AbstractTimeInput::timeToValue(int t) const {
    int val;
    t = qAbs(t);

    if (m_editorFlags & NoSeconds) {
        t /= 60;
    }
    val = t % 60; // secs
    t /= 60;
    val += (t % 60) * 100; // mins
    t /= 60;
    val += t * 10000; // hours
    return val;
}

int AbstractTimeInput::valueToTime(int v) const {
    if (m_editorFlags & NoSeconds) {
        v *= 100;
    }
    return (v / 10000  ) * 3600  // hours
         + ((v / 100) % 100) * 60    // mins
         +  v % 100                    // secs
    ;
}

int AbstractTimeInput::stringToPositiveTime(QString s) {
    int smhd[3] = {};
    size_t smhdI = 0;
    int place = 1;
    if (s.length() == 0) return -1;
    for (qsizetype i = s.length() - 1; i >= 0; i--) {
        QChar c = s.at(i);
        if (c.isDigit()) {
            smhd[smhdI] += c.digitValue() * place;
            place *= 10;
        } else {
            if (c == QChar(':') || c == QChar('\t')) {
                smhdI++;
                if (smhdI >= 3) return -1;
                place = 1;
            } else if (QSet<QChar>({' ', '\r', '\n', ','}).contains(c)) {
            } else if (c == QChar('.')) {
                break;
            } else return -1;
        }
    }
    return smhd[0] + (smhd[1] + (smhd[2]) * 60) * 60;
}

bool AbstractTimeInput::setFromString(QString s) {
    int t = stringToPositiveTime(s);
    if (s.length() == 0 || t == -1) return false;
    if (t > 86400) t = 86400;
    m_internalValue = timeToValue(t);
    if (m_editorFlags & AllowNegative) m_internalNegative = s.at(0) == "-";
    emit displayTextChanged();
    return true;
}

bool AbstractTimeInput::focus() const { return m_focus; }
bool AbstractTimeInput::selected() const { return m_selected; }
AbstractTimeInput::EditorFlags AbstractTimeInput::editorFlags() const { return m_editorFlags; }

void AbstractTimeInput::setSelected(bool selected) {
    m_selected = selected;
    emit selectedChanged();
}

void AbstractTimeInput::setFocus(bool focus) {
    m_focus = focus;
    if (!focus) {
        commit();
        setSelected(false);
    } else {
        setSelected(true);
    }
    emit focusChanged();
}

void AbstractTimeInput::setEditorFlags(AbstractTimeInput::EditorFlags value) {
    if ((m_editorFlags & NoSeconds) != (value & NoSeconds)) {
        if (value & NoSeconds) {
            m_committedValue /= 100;
            m_internalValue /= 100;
            emit timeChanged();
        } else {
            m_committedValue *= 100;
            m_internalValue *= 100;
        }
    }
    if (!(value & AllowNegative)) {
        m_internalNegative = m_committedNegative = false;
    }
    m_editorFlags = value;
    emit editorFlagsChanged();
    emit displayTextChanged();
}

void AbstractTimeInput::commit() {
    m_internalValue = timeToValue(std::clamp(valueToTime(m_internalValue),0,86400));
    m_committedValue = m_internalValue;
    m_committedNegative = m_internalNegative;
    emit timeChanged();
    emit displayTextChanged();
    emit editingFinished();
}

QString AbstractTimeInput::displayText() const {
    const QString negative = m_internalNegative ? "-" : "";
    if (m_editorFlags & NoSeconds) {
        return negative + QString("%1:%2")
            .arg(( m_internalValue / 100     )      , 2, 10, QChar('0')) // hours
            .arg(( m_internalValue / 1       ) % 100, 2, 10, QChar('0'));// mins
    }
    else {
        return negative + QString("%1:%2:%3")
            .arg(( m_internalValue / 10000   )      , 2, 10, QChar('0')) // hours
            .arg(( m_internalValue / 100     ) % 100, 2, 10, QChar('0')) // mins
            .arg(( m_internalValue / 1       ) % 100, 2, 10, QChar('0'));// secs
    }
}

int AbstractTimeInput::time() const {
    return valueToTime(m_committedValue) * (m_committedNegative ? -1 : 1);
}

bool AbstractTimeInput::setTime(int secs) {
    if (secs < 0 && !(m_editorFlags & AllowNegative)) {
        qWarning() << "abstracttimeinput.h: tried to set negative time";
        return false;
    }
    m_internalValue = qAbs(timeToValue(secs));
    m_internalNegative = secs < 0;
    emit timeChanged();
    if (!m_focus) {
        m_committedValue = m_internalValue;
        m_committedNegative = m_internalNegative;
        emit displayTextChanged();
    }
    return true;
}

bool AbstractTimeInput::handleKeyPress(int key, int modifiers) {
    if (modifiers & Qt::ControlModifier) {
        switch (key) {
        case Qt::Key_Delete:
        case Qt::Key_Backspace:
            m_internalValue = 0;
            m_internalNegative = false;
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
        if (m_selected) {
            m_internalValue = 0;
            setSelected(false);
        }
        if (m_editorFlags & NoSeconds && m_internalValue > 999) {
            return false;
        } if (m_internalValue > 99999) {
            return false;
        }

        m_internalValue = m_internalValue * 10 + key - Qt::Key_0; // append
        emit displayTextChanged();
        return true;
    }
    switch (key) {
    case Qt::Key_Delete:
    case Qt::Key_Backspace:
        if (m_selected) {
            m_internalValue = 0;
        } else {
            m_internalValue /= 10;
        }
        emit displayTextChanged();
        break;
    case Qt::Key_Return:
    case Qt::Key_Enter:
        setSelected(true);
        commit();
        break;
    case Qt::Key_Escape:
        if (m_selected) {
            setSelected(false);
        } else {
            setSelected(true);
            m_internalValue = m_committedValue;
            m_internalNegative = m_committedNegative;
            emit selectedChanged();
            emit displayTextChanged();
        }
        break;
    case Qt::Key_Minus:
        if (m_editorFlags & AllowNegative) m_internalNegative = !m_internalNegative;
        emit displayTextChanged();
        break;
    default:
        return false;
    }
    return true;
}

