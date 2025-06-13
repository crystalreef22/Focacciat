import QtQuick
import QtQuick.Controls
import FocusAssist9 1.0

Window {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Focus Assist 9??")

    MouseArea {
        id: defaultFocus
        anchors.fill: parent
        onClicked: defaultFocus.forceActiveFocus();
    }
    Column {
        Text {
            text: "FocusAssist9";
            font.pointSize: 100;
        }
        Text {
            id: tmtx;
            font.pointSize: 20;
        }
        Button {
            onClicked: ft.start(10000);
            text: "START";
        }

        TodoList {
            labelText: "today's tasks"
        }
    }

    FocusTimer {
        id: ft
        onTimerUpdated: {
            tmtx.text = Math.ceil(elapsedMs/10)*10;
        }
    }
}
