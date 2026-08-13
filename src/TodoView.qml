import QtQuick
import QtQuick.Controls
import QtQuick.Layouts
import QtMultimedia
import QtCore

import Focacciat

// https://github.com/rohanrajpal/QtToDoList
// https://www.youtube.com/watch?v=9BcAYDlpuT8

ColumnLayout {
    id: root
    property string labelText: ""

    Label {
        text: labelText
    }
    SystemPalette {
        id: myPalette;
    }
    Item {
        id: draggableListViewRoot
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true;

        Component {
            id: todoListViewDelegate
            Item {
                required property var model
                id: todoListViewRoot
                anchors {
                    left: parent?.left
                    right: parent?.right
                }
                height: todoListViewContent.height
                Rectangle {
                    id: todoListViewContent
                    width: parent.width
                    height: 32
                    color: todoListViewDragArea.held ? "yellow" : (model.active ? myPalette.highlight : myPalette.dark)
                    states: State {
                        when: todoListViewDragArea.held

                        ParentChange {
                            target: todoListViewContent
                            parent: draggableListViewRoot
                        }
                    }
                    Drag.active: todoListViewDragArea.held
                    Drag.source: todoListViewContent
                    Drag.hotSpot {
                        x: width/2
                        y: height/2
                    }
                    DropArea {
                        anchors.fill: parent
                        anchors.margins: 10

                        onEntered: (drag) => {
                            todoListView.model.moveItem(
                                drag.source.DelegateModel.itemsIndex,
                                todoListViewContent.DelegateModel.itemsIndex)
                        }
                    }

                    MouseArea {
                        onClicked: forceActiveFocus()
                        anchors.fill: parent
                    }
                    RowLayout {
                        id: todoDelegateLayout
                        width: parent.width - 15
                        anchors.verticalCenter: parent.verticalCenter

                        Rectangle {
                            width: 20; height: 31
                            MouseArea {
                                id: todoListViewDragArea

                                property bool held: false

                                anchors.fill: parent
                                height: todoListViewContent.height
                                drag.target: held ? todoListViewContent : undefined
                                drag.axis: Drag.YAxis
                                onPressed: held = true
                                onReleased: held = false
                            }
                            Label {
                                text: "mv"
                            }
                        }
                        CheckBox {
                            checked: model.item.done
                            onClicked: model.item.done = checked
                        }
                        TextInput {
                            Layout.fillWidth: true
                            clip: true
                            onEditingFinished: model.item.description = text
                            text: model.item.description
                            focusPolicy: Qt.StrongFocus
                        }
                        TimeInput {
                            visible: todoDelegateLayout.width > 175
                            time: model.item.timeEstimate / 1000
                            onEditingFinished: model.item.timeEstimate = time * 1000
                            editorFlags: AbstractTimeInput.NoSeconds
                        }
                        Label {
                            visible: todoDelegateLayout.width > 230
                            text: FormatUtils.msToTime(model.item.timeRemaining)
                            font.features: { "tnum": true }
                        }
                        ComboBox {
                            visible: todoDelegateLayout.width > 340
                            displayText: todoListViewRoot.model.blocklistIndex.valid ? todoListViewRoot.model.blocklistIndex.data(BlocklistManager.NameRole) : "None"
                            focusPolicy: Qt.TabFocus
                            popup: Menu {
                                id: todoListViewComboboxPopup
                                popupType: Popup.Native
                                MenuItem {
                                    text: "None"
                                    onTriggered: todoListViewRoot.model.blocklistIndex = GlobalState.constructInvalidQModelIndex();
                                }
                                MenuSeparator{}
                                Instantiator {
                                    id: todoListViewComboboxPopupInstantiator
                                    model: GlobalState.blocklistManager;
                                    delegate: MenuItem {
                                        text: model.name
                                        checked: todoListViewRoot.model.blocklistIndex === model.modelIndex
                                        onTriggered: todoListViewRoot.model.blocklistIndex = model.modelIndex
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
                        onClicked: ()=>{model.active = !model.active; forceActiveFocus()}
                    }
                }
            }
        }

        ListView {
            id: todoListView
            anchors.fill: parent
            anchors.margins: 2
            spacing: 2
            acceptedButtons: Qt.NoButton // disable flicking
            model: GlobalState.todoModel
            delegate: todoListViewDelegate

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

        }

    }

    RowLayout {
        Layout.alignment: Qt.AlignBottom
        Button {
            text: qsTr("Add")
            onClicked: GlobalState.todoModel.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove checked")
            onClicked: GlobalState.todoModel.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
