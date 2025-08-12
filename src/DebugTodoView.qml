import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia

import "components" as MyComponents

import Todo 1.0
import Focacciat

// https://github.com/rohanrajpal/QtToDoList
// https://www.youtube.com/watch?v=9BcAYDlpuT8

ColumnLayout {
    property string labelText: ""
    required property BlocklistListModel blocklists
    required property TodoModel todoModel

    MediaPlayer {
        id: expiredNotifier;
        property bool muted
        property bool expired
        audioOutput: AudioOutput { device: mediaDevices.defaultAudioOutput; }
        source: "media/alm_focus.mp3";
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

    Connections { target: todoModel.activeItem; function onTimerExpiredChanged() {
        expiredNotifier.expired = todoModel.activeItem.timerExpired;
    }}
    Connections { target: todoModel; function onActiveItemChanged() {
        expiredNotifier.expired = todoModel.activeItem && todoModel.activeItem.timerExpired
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
            running: todoModel.paused;

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
        text: ( todoModel.paused ? "Paused for " : "Last paused " )
              + FormatUtils.msToTime(todoModel.pausedTime)
              + (todoModel.paused ? "" : " ago");
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
        Label {
            text: "time left"
        }
        Label {
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
                model: todoModel

                delegate: RowLayout {
                    id: todoListViewDelegate
                    required property var model
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
                                model: blocklists
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
