#ifndef TIMEINPUT_H
#define TIMEINPUT_H

// todo: flags to allow negatives and disallow seconds

#include <QObject>
#include <QKeyEvent>
#include <QFlags>

#include <QtQmlIntegration/qqmlintegration.h>

class TimeInput : public QObject
{
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString displayText READ displayText NOTIFY displayTextChanged FINAL)
    Q_PROPERTY(int time READ time WRITE setTime NOTIFY timeChanged FINAL)
    Q_PROPERTY(bool focus READ focus WRITE setFocus NOTIFY focusChanged FINAL)
    Q_PROPERTY(bool selected READ selected WRITE setSelected NOTIFY selectedChanged FINAL)
    Q_PROPERTY(TimeInput::EditorFlags editorFlags READ editorFlags WRITE setEditorFlags NOTIFY editorFlagsChanged FINAL)
public:
    explicit TimeInput(QObject *parent = nullptr);

    enum EditorFlag {
        None = 0,
        NoSeconds       = 1 << 0,
        AbsoluteTime    = 1 << 1,
        AllowNegative   = 1 << 2,
    };
    Q_ENUM(EditorFlag)
    Q_DECLARE_FLAGS(EditorFlags, EditorFlag)

    QString displayText() const;
    int time() const;
    bool focus() const;
    bool selected() const;
    EditorFlags editorFlags() const;

    bool setTime(int secs);
    void setFocus(bool focus);
    void setSelected(bool selected);
    void setEditorFlags(EditorFlags value);

signals:
    void displayTextChanged();
    void timeChanged();
    void focusChanged();
    void selectedChanged();
    void editingFinished();
    void editorFlagsChanged();

public slots:
    bool handleKeyPress(int key, int modifiers);
    bool setFromString(QString s);

private:
    EditorFlags m_editorFlags{None};

    int m_internalValue;
    int m_committedValue;
    bool m_internalNegative{false};
    bool m_committedNegative{false};
    bool m_focus;
    bool m_selected;
    int timeToValue(int t) const;
    int valueToTime(int v) const;
    static int stringToPositiveTime(QString s);
    void commit();
};

Q_DECLARE_OPERATORS_FOR_FLAGS(TimeInput::EditorFlags)

#endif // TIMEINPUT_H
