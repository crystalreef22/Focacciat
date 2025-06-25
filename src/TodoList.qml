import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Todo 1.0

// https://github.com/rohanrajpal/QtToDoList
// https://www.youtube.com/watch?v=9BcAYDlpuT8

ColumnLayout {
    property string labelText: ""
    Label {
        text: labelText
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
