import QtQuick
import QtQuick.Controls
import FocusAssist9 1.0

Window {
    width: 640
    height: 480
    visible: true
    title: qsTr("Focus Assist 9??")

    Column {
        Text {
            text: "FocusAssist9";
            font.pointSize: 100;
        }
        Text {
            id: tmtx;
            font.pointSize: 20;
        }
        Button {
            onClicked: ft.start(10000);
            text: "START";
        }
    }
    Timer {
        id: tm
    }

    FocusTimer {
        id: ft
        onTimerUpdated: {
            tmtx.text = Math.ceil(elapsedMs/10)*10;
        }
    }
}
