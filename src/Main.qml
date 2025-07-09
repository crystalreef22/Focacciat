import QtQuick
import QtQuick.Controls

import "components" as MyComponents

ApplicationWindow {
    id: root
    width: 640
    height: 480
    visible: true
    title: qsTr("Focus Assist 9??")

    flags: Qt.WindowStaysOnTopHint

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    Rectangle {
        color: palette.window
        anchors.fill: parent
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
            MyComponents.TimeInput {
                font.pointSize: 100;
            }
        }
    }

}
