import QtQuick
import QtQuick.Layouts
import QtQuick.Controls

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("Git Status")
    width: 1200
    height: 800

    font.pixelSize: 20

    StackView {
        id: stackView
        anchors.fill: parent
        initialItem: "GitStatusPage.qml"
    }

}
