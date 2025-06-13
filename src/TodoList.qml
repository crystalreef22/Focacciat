import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

import Todo 1.0

// https://github.com/rohanrajpal/QtToDoList
// https://www.youtube.com/watch?v=9BcAYDlpuT8

ColumnLayout {
    Frame {
        Layout.fillWidth: true
        ScrollView {
            implicitWidth: 250
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
                        checked: model.done
                        onClicked: model.done = checked
                    }
                    TextField {
                        Layout.fillWidth: true
                        onEditingFinished: model.description = text
                        text: model.description
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
