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
                    list: todoList // todoList defined in main.cpp
                }

                delegate: RowLayout {
                    width: todoListView.width
                    CheckBox {
                        checked: model.active
                        onClicked: model.active = checked
                    }

                    CheckBox {
                        checked: model.done
                        onClicked: model.done = checked
                    }
                    TextField {
                        Layout.fillWidth: true
                        onEditingFinished: model.description = text
                        text: model.description
                    }
                    TextField {
                        text: model.timeEstimate
                        onEditingFinished: model.timeEstimate = text
                        implicitWidth: 80;
                    }
                    Text {
                        text: model.timeRemaining
                    }
                }
            }
        }
    }

    RowLayout {
        Button {
            text: qsTr("Add")
            onClicked: todoList.appendItem()
            Layout.fillWidth: true
        }
        Button {
            text: qsTr("Remove completed")
            onClicked: todoList.removeCompletedItems()
            Layout.fillWidth: true
        }
    }
}
