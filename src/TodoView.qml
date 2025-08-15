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

    Label {
        text: labelText
    }
    SystemPalette {
        id: myPalette;
    }

    ListView {
        id: todoListView
        model: todoModel
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true;
        spacing: 2
        acceptedButtons: Qt.NoButton // disable flicking

        MouseArea {
            onClicked: forceActiveFocus()
        }

        ScrollBar.vertical: ScrollBar {
            id: scrollbar
            policy: ScrollBar.AsNeeded
            background: null
            opacity: 0
            states: [
                State {
                    name: "active"
                    when: scrollbar.active
                    PropertyChanges { target: scrollbar; opacity: 1}
                },
                State {
                    name: "inactive"
                    when: !scrollbar.active
                    PropertyChanges { target: scrollbar; opacity: 0 }
                }
            ]

            transitions: [
                Transition {
                    from: "active"
                    to: "inactive"
                    OpacityAnimator {
                        duration: 500
                    }
                }
            ]
        }

        delegate: Rectangle {
            id: todoListViewDelegate
            required property var model
            width: todoListView.width
            height: 32
            color: model.active ? myPalette.highlight : myPalette.dark

            //DragHandler { }
            MouseArea {
                onClicked: forceActiveFocus()
                anchors.fill: parent
            }
            RowLayout {
                id: todoDelegateLayout
                width: parent.width - 15
                anchors.centerIn: parent

                CheckBox {
                    checked: model.item.done
                    onClicked: model.item.done = checked
                }
                TextInput {
                    Layout.fillWidth: true
                    clip: true
                    onEditingFinished: model.item.description = text
                    text: model.item.description
                }
                MyComponents.TimeInput {
                    visible: todoDelegateLayout.width > 175
                    time: model.item.timeEstimate / 1000
                    onEditingFinished: model.item.timeEstimate = time * 1000
                    editorFlags: TimeInput.NoSeconds
                }
                Label {
                    visible: todoDelegateLayout.width > 230
                    text: FormatUtils.msToTime(model.item.timeRemaining)
                    font.features: { "tnum": true }
                }
                ComboBox {
                    visible: todoDelegateLayout.width > 340
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
            MouseArea {
                acceptedButtons: Qt.RightButton
                anchors.fill: parent
                onClicked: model.active = !model.active
            }

        }
    }

    RowLayout {
        Layout.alignment: Qt.AlignBottom
        Button {
            text: qsTr("Add")
            onClicked: todoModel.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove checked")
            onClicked: todoModel.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
