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

    ScrollView {
        implicitWidth: 500
        implicitHeight: 250
        Layout.fillWidth: true
        Layout.fillHeight: true
        clip: true;
        ListView {
            id: todoListView
            model: todoModel
            anchors.left: parent.left; anchors.right: parent.right

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

    RowLayout {
        Layout.alignment: Qt.AlignBottom
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
