import QtQuick 2.15
import QtQuick.Controls
import QtQuick.Effects
import Focacciat 1.0

FocusScope {
    id: scope
    property alias color: rectangle.color
    x: rectangle.x; y: rectangle.y;
    width: rectangle.width + margin; height: rectangle.height + margin;
    property int padding: 10
    property int borderWidth: 2
    property int margin: 5
    property alias font: text.font
    property alias time: timeInput.time
    property alias editorFlags: timeInput.editorFlags

    signal editingFinished;
    activeFocusOnTab: true

    SystemPalette {
        id: myPalette;
    }

    AbstractTimeInput {
        id: timeInput
        focus: scope.activeFocus
        onEditingFinished: scope.editingFinished();
        editorFlags: AbstractTimeInput.None;
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
        border.width: scope.activeFocus ? borderWidth : 0
        border.color: Qt.rgba(myPalette.accent.r, myPalette.accent.g, myPalette.accent.b, 0.5)
        radius: 1

        Label {
            anchors.centerIn: parent;
            id: text
            text: timeInput.displayText
            font.features: { "tnum": true }
            background: Rectangle {
                color: myPalette.highlight;
                visible: timeInput.selected;
            }
        }


    }

    MultiEffect {
        source: rectangle
        anchors.fill: rectangle
        shadowBlur: 0.08
        shadowEnabled: true
        shadowColor: "black"
        shadowOpacity: 0.23
        shadowVerticalOffset: 1
        shadowHorizontalOffset: 0
    }
    MouseArea {
        anchors.fill: parent
        onClicked: () => {
            if (scope.activeFocus) {
                timeInput.selected = !timeInput.selected;
            }
            scope.forceActiveFocus()
        }
        cursorShape: Qt.IBeamCursor
        hoverEnabled: true
    }

}
