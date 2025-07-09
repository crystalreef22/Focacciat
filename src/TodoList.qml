import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

import Todo 1.0

// https://github.com/rohanrajpal/QtToDoList
// https://www.youtube.com/watch?v=9BcAYDlpuT8

ColumnLayout {
    property string labelText: ""

    MediaPlayer {
        id: expiredNotifier;
        audioOutput: AudioOutput { device: mediaDevices.defaultAudioOutput; }
        source: "media/alm_focus.mp3";
        loops: MediaPlayer.Infinite;
        function setPlayStatus(b) {
            if (b) {
                expiredNotifier.play();
            } else {
                expiredNotifier.stop();
            }
        }
    }
    MediaDevices { id: mediaDevices }

    Connections { target: todoModel.activeItem; function onTimerExpiredChanged() {
        expiredNotifier.setPlayStatus(todoModel.activeItem.timerExpired)
    }}
    Connections { target: todoModel; function onActiveItemChanged() {
        expiredNotifier.setPlayStatus(todoModel.activeItem && todoModel.activeItem.timerExpired)
    }}

    Label {
        text: labelText
    }
    RowLayout {
        width: todoListView.width

        Label {
            text: todoModel.activeItem ? "Active" : "Inactive"
        }

        CheckBox {
            checked: todoModel.activeItem ? todoModel.activeItem.done : false;
            onClicked: todoModel.activeItem.done = checked
            enabled: todoModel.activeItem
        }
        TextField {
            Layout.fillWidth: true
            onEditingFinished: todoModel.activeItem.description = text
            text: todoModel.activeItem ? todoModel.activeItem.description : ""
            enabled: todoModel.activeItem
        }
        TextField {
            text: todoModel.activeItem ? todoModel.activeItem.timeEstimate : "0"
            onEditingFinished: todoModel.activeItem.timeEstimate = text
            enabled: todoModel.activeItem
            implicitWidth: 80;
        }
        Text {
            text: "time left"
        }
        Text {
            text: todoModel.activeItem ? FormatUtils.msToTime(todoModel.activeItem.timeRemaining) : "xx:xx:xx"
            font.features: { "tnum": true }
        }
    }

    Frame {
        Layout.fillWidth: true
        ScrollView {
            implicitWidth: 500
            implicitHeight: 250
            anchors.fill: parent
            clip: true;
            ListView {
                id: todoListView
                model: TodoModel {
                    id: todoModel
                    //list: todoList // todoList defined in main.cpp
                }

                delegate: RowLayout {
                    width: todoListView.width
                    CheckBox {
                        checked: model.active
                        onClicked: model.active = checked
                    }

                    CheckBox {
                        checked: model.item.done
                        onClicked: model.item.done = checked
                    }
                    TextField {
                        Layout.fillWidth: true
                        onEditingFinished: model.item.description = text
                        text: model.item.description
                    }
                    TextField {
                        text: model.item.timeEstimate
                        onEditingFinished: model.item.timeEstimate = text
                        implicitWidth: 80;
                    }
                    Text {
                        text: "time left"
                    }
                    Text {
                        text: FormatUtils.msToTime(model.item.timeRemaining)
                    }
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("Add")
            onClicked: todoModel.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove completed")
            onClicked: todoModel.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
