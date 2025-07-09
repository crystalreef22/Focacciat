import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

import "components" as MyComponents

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
        MyComponents.TimeInput {
            time: todoModel.activeItem ? todoModel.activeItem.timeEstimate/1000 : "0"
            onEditingFinished: todoModel.activeItem.timeEstimate = time*1000
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
                    MyComponents.TimeInput {
                        time: model.item.timeEstimate / 1000
                        onEditingFinished: model.item.timeEstimate = time * 1000
                        implicitWidth: 80;
                    }
                    Text {
                        text: "time left"
                    }
                    Text {
                        text: FormatUtils.msToTime(model.item.timeRemaining)
                        font.features: { "tnum": true }
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
