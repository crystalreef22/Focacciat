import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import "components" as MyComponents
import Focacciat 1.0

ApplicationWindow {
    id: root
    width: 640
    height: 640
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
            width: parent.width
            Label {
                text: "Focacciat";
                font.pointSize: 100;
            }
            Label {
                text: "Debug view. Dark mode will be glitchy. Make sure to open Focacciat before opening Firefox, and install the extension so that blocking will work."
                width: parent.width
                wrapMode: Text.Wrap
                bottomPadding: 10
            }

            TodoView {
                labelText: "today's tasks"
                blocklists: blocklists
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
            Button {
                text: "Ping extension"
                onClicked: ExtensionIntegration.sendPing();
            }
            BlocklistListModel {
                id: blocklists
            }

            BlocklistPopup {
                id: blocklistPopup
                blocklists: blocklists
            }

            Button {
                text: "edit blocklist"
                onClicked: blocklistPopup.open();
            }
            Row {
                MyComponents.TimeInput {
                    id: debugTimeInput1
                    editorFlags: (debugTimeInput1AllowNegative.checked ? TimeInput.AllowNegative : TimeInput.None) | (debugTimeInput1NoSeconds.checked ? TimeInput.NoSeconds : TimeInput.None)
                }
                Label {
                    text: FormatUtils.msToTime(debugTimeInput1.time*1000)
                }
            }
            CheckBox {
                id: debugTimeInput1AllowNegative
                text: "allow negative"
            }
            CheckBox {
                id: debugTimeInput1NoSeconds
                text: "no seconds"
            }
        }
    }

}
