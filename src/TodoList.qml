import QtQuick
import QtQuick.Controls
import QtQuick.Layouts

// https://github.com/rohanrajpal/QtToDoList

Frame {
    ScrollView {
        implicitWidth: 250
        implicitHeight: 250
        clip: true;
        ListView {
            model: 100
            delegate: RowLayout {
                CheckBox {}
                TextField {}
            }
        }
    }

}
