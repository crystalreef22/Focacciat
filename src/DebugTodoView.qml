import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

import "components" as MyComponents

import Focacciat

// https://github.com/rohanrajpal/QtToDoList
// https://www.youtube.com/watch?v=9BcAYDlpuT8

ColumnLayout {
    property string labelText: ""

    MediaPlayer {
        id: expiredNotifier;
        property bool muted
        property bool expired
        audioOutput: AudioOutput { device: mediaDevices.defaultAudioOutput; }
        source: "media/alm_focus.mp3"
        loops: MediaPlayer.Infinite;
        onExpiredChanged: ()=>{
            if (!muted) {
                if (expired) {
                    play();
                } else {
                    stop();
                }
            }
        }
        onMutedChanged: () => {
            if (muted) {
                stop();
            } else {
                if (expired) {
                    play();
                }
            }
        }
    }
    MediaDevices { id: mediaDevices }

    Connections { target: TodoModel.activeItem; function onTimerExpiredChanged() {
        expiredNotifier.expired = TodoModel.activeItem.timerExpired;
    }}
    Connections { target: TodoModel; function onActiveItemChanged() {
        expiredNotifier.expired = TodoModel.activeItem && TodoModel.activeItem.timerExpired
    }}

    Label {
        text: labelText
    }

    Switch {
        text: "mute"
        onClicked: expiredNotifier.muted = checked;
    }

    Rectangle {
        id: indicator;
        width: 100;
        height: 50;

        color: "green"
        SequentialAnimation {
            id: flash
            loops: Animation.Infinite
            running: TodoModel.paused;

            PropertyAnimation {
                target: indicator
                property: "color"
                to: "black"
                duration: 250
                easing {type: Easing.Linear}
            }
            PropertyAnimation {
                target: indicator
                property: "color"
                to: "red"
                duration: 250
                easing {type: Easing.Linear}
            }
            onStarted: indicator.color = "red"
            onStopped: indicator.color = "green"
        }
    }

    Label {
        text: ( TodoModel.paused ? "Paused for " : "Last paused " )
              + FormatUtils.msToTime(TodoModel.pausedTime)
              + (TodoModel.paused ? "" : " ago");
    }

    RowLayout {
        width: todoListView.width

        Label {
            text: TodoModel.activeItem ? "Active" : "Inactive"
        }

        CheckBox {
            checked: TodoModel.activeItem ? TodoModel.activeItem.done : false;
            onClicked: TodoModel.activeItem.done = checked
            enabled: TodoModel.activeItem
        }
        TextField {
            Layout.fillWidth: true
            onEditingFinished: TodoModel.activeItem.description = text
            text: TodoModel.activeItem ? TodoModel.activeItem.description : ""
            enabled: TodoModel.activeItem
        }
        MyComponents.TimeInput {
            time: TodoModel.activeItem ? TodoModel.activeItem.timeEstimate/1000 : "0"
            onEditingFinished: TodoModel.activeItem.timeEstimate = time*1000
            enabled: TodoModel.activeItem
            implicitWidth: 80;
        }
        Label {
            text: "time left"
        }
        Label {
            text: TodoModel.activeItem ? FormatUtils.msToTime(TodoModel.activeItem.timeRemaining) : "xx:xx:xx"
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
                model: TodoModel

                delegate: RowLayout {
                    id: todoListViewDelegate
                    required property var model
                    width: todoListView.width
                    Button {
                        text: "mvTo"
                        onClicked: moveAmt.text = TodoModel.moveItem(model.index, parseInt(moveAmt.text)) ? "" : "Failed!";
                    }
                    TextField {
                        id: moveAmt
                        Layout.preferredWidth: 15
                    }

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
                        editorFlags: TimeInput.NoSeconds
                    }
                    Label {
                        text: "time left"
                    }
                    Label {
                        text: FormatUtils.msToTime(model.item.timeRemaining)
                        font.features: { "tnum": true }
                    }
                    ComboBox {
                        displayText: todoListViewDelegate.model.item.blocklist ? todoListViewDelegate.model.item.blocklist.name : "None"
                        focusPolicy: Qt.TabFocus
                        popup: Menu {
                            id: todoListViewComboboxPopup
                            popupType: Popup.Native
                            MenuItem {
                                text: "None"
                                onTriggered: todoListViewDelegate.model.item.blocklist = null;
                            }
                            MenuSeparator{}
                            Instantiator {
                                id: todoListViewComboboxPopupInstantiator
                                model: BlocklistListModel
                                delegate: MenuItem {
                                    text: model.name
                                    checked: todoListViewDelegate.model.item.blocklist === model.item
                                    onTriggered: todoListViewDelegate.model.item.blocklist = model.item
                                }
                                onObjectAdded: (index, object) => todoListViewComboboxPopup.insertItem(index+2, object) // index 1 is None, 2 is seperator
                                onObjectRemoved: (index, object) => todoListViewComboboxPopup.removeItem(object)
                            }
                        }
                    }
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("Add")
            onClicked: TodoModel.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove completed")
            onClicked: TodoModel.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
