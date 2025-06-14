import QtQuick
import QtQuick.Controls

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

        TodoList {
            labelText: "today's tasks"
        }
    }

}
