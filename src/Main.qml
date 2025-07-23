import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

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
            Label {
                text: "FocusAssist9";
                font.pointSize: 100;
            }

            TodoList {
                labelText: "today's tasks"
            }
            RowLayout {
                Button {
                    text: "check Firefox integration"
                    onClicked: ExtensionIntegration.checkFirefoxEnabled();
                }
                CheckBox {
                    text: "integration enabled"
                    onToggled:  ExtensionIntegration.firefoxEnabled = checked;
                    checked: ExtensionIntegration.firefoxEnabled;
                    Component.onCompleted: ExtensionIntegration.checkFirefoxEnabled();
                }
            }
            RowLayout {
                TextField {
                    id: toSend
                }
                Button {
                    text: "Send data"
                    onClicked: ExtensionIntegration.send(toSend.text);
                }
            }
        }
    }

}
