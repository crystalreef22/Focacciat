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

    Connections { target: GlobalState.todoModel.activeItem; function onTimerExpiredChanged() {
        expiredNotifier.expired = GlobalState.todoModel.activeItem.timerExpired;
    }}
    Connections { target: GlobalState.todoModel; function onActiveItemChanged() {
        expiredNotifier.expired = GlobalState.todoModel.activeItem && GlobalState.todoModel.activeItem.timerExpired
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
            running: GlobalState.todoModel.paused;

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
        text: ( GlobalState.todoModel.paused ? "Paused for " : "Last paused " )
              + FormatUtils.msToTime(GlobalState.todoModel.pausedTime)
              + (GlobalState.todoModel.paused ? "" : " ago");
    }

    RowLayout {
        width: todoListView.width

        Label {
            text: GlobalState.todoModel.activeItem ? "Active" : "Inactive"
        }

        CheckBox {
            checked: GlobalState.todoModel.activeItem ? GlobalState.todoModel.activeItem.done : false;
            onClicked: GlobalState.todoModel.activeItem.done = checked
            enabled: GlobalState.todoModel.activeItem
        }
        TextField {
            Layout.fillWidth: true
            onEditingFinished: GlobalState.todoModel.activeItem.description = text
            text: GlobalState.todoModel.activeItem ? GlobalState.todoModel.activeItem.description : ""
            enabled: GlobalState.todoModel.activeItem
        }
        MyComponents.TimeInput {
            time: GlobalState.todoModel.activeItem ? GlobalState.todoModel.activeItem.timeEstimate/1000 : "0"
            onEditingFinished: GlobalState.todoModel.activeItem.timeEstimate = time*1000
            enabled: GlobalState.todoModel.activeItem
            implicitWidth: 80;
        }
        Label {
            text: "time left"
        }
        Label {
            text: GlobalState.todoModel.activeItem ? FormatUtils.msToTime(GlobalState.todoModel.activeItem.timeRemaining) : "xx:xx:xx"
            font.features: { "tnum": true }
        }
    }

    Frame {
        Layout.fillWidth: true
        ScrollView {
            implicitWidth: 620
            implicitHeight: 250
            anchors.fill: parent
            clip: true;
            ListView {
                id: todoListView
                model: GlobalState.todoModel

                delegate: RowLayout {
                    id: todoListViewDelegate
                    required property var model
                    width: todoListView.width
                    Button {
                        text: "mvTo"
                        onClicked: moveAmt.text = GlobalState.todoModel.moveItem(model.index, parseInt(moveAmt.text)) ? "" : "Failed!";
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
                        editorFlags: AbstractTimeInput.NoSeconds
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
                                model: GlobalState.blocklistManager
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
            onClicked: GlobalState.todoModel.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove completed")
            onClicked: GlobalState.todoModel.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
