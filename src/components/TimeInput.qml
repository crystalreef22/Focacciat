import QtQuick 2.15
import QtQuick.Controls
import components 1.0

FocusScope {
    id: scope
    property alias color: rectangle.color
    x: rectangle.x; y: rectangle.y;
    width: rectangle.width + margin; height: rectangle.height + margin;
    property int padding: 10
    property int borderWidth: 2
    property int margin: 5
    property alias font: text.font

    SystemPalette {
        id: myPalette;
        colorGroup: scope.activeFocus ? SystemPalette.Active : SystemPalette.Inactive
    }

    TimeInput {
        id: timeInput
        focus: scope.focus
    }

    Keys.onPressed: (event)=> {
        event.accepted = timeInput.handleKeyPress(event.key, event.modifiers)
    }

    Rectangle {
        id: rectangle
        anchors.centerIn: parent;
        color: myPalette.base
        width: text.width + padding
        height: text.height + padding
        border.width: scope.focus ? borderWidth : 0
        border.color: myPalette.accent

        Label {
            anchors.centerIn: parent;
            id: text
            text: timeInput.displayText
            font.features: { "tnum": true }
            background: Rectangle {
                color: "lightblue";
                visible: timeInput.selected;
            }
        }


    }
    MouseArea {
        anchors.fill: parent
        onClicked: () => {
            if (scope.focus) {
                timeInput.selected = !timeInput.selected;
            }
            scope.forceActiveFocus()
        }
    }

}
