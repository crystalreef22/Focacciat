import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Focacciat 1.0

ApplicationWindow {
    id: root
    width: 640
    height: 1200
    visible: true
    title: qsTr("Focus Assist 9??")

    flags: Qt.WindowStaysOnTopHint

    SystemPalette { id: palette; colorGroup: SystemPalette.Active }

    Main {
        id: mainWindow
        visible: true
    }

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
            CheckBox {
                text: "show main window"
                onCheckStateChanged: mainWindow.visible = checked;
                checked: true;
            }


            FlatButton{
                id: pauseButton
                icon.name: GlobalState.todoModel.paused ? "play" : "pause"
                onClicked: GlobalState.todoModel.paused = !GlobalState.todoModel.paused;
            }
            FlatButton{
                id: stopButton
                icon.name: "stop"
                holdable: true;
                enabled: GlobalState.todoModel.activeItem.timeElapsed !== 0;
                onHoldDone: ()=>{
                    GlobalState.todoModel.paused = true;
                    GlobalState.todoModel.pausedTime += GlobalState.todoModel.activeItem.timeElapsed;
                    GlobalState.todoModel.activeItem.timeElapsed = 0;
                }
            }

            DebugTodoView {
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
            Button {
                text: "Ping extension"
                onClicked: ExtensionIntegration.sendPing();
            }

            BlocklistPopup {
                id: blocklistPopup
            }

            Button {
                text: "edit blocklist"
                onClicked: blocklistPopup.open();
            }
            Row {
                TimeInput {
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
            Row {
                Button {
                    text: "load"
                    onClicked: GlobalState.deserializeFromFile();
                }
                Button {
                    text: "save"
                    onClicked: GlobalState.serializeToFile();
                }
            }
        }
    }

}
