import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore
import Status

ApplicationWindow {
    id: root
    visible: true
    title: qsTr("Git Status")
    width: 1200
    height: 800

    font.pixelSize: 20

    property GitStatus status;

    Component.onCompleted: {
        statusModel.repository.open()
        statusModel.update()
    }

    GitStatusModel {
        id: statusModel

        repository: GitRepository {
            path: "/home/alex/prj/GitTools"
        }
    }

    ListView {
        id: listView
        model: statusModel
        activeFocusOnTab: true
        anchors.fill: parent

        Keys.onPressed: {
            if (event.key === Qt.Key_Enter || event.key === Qt.Key_Return) {
                console.log("listView Keys.onPressed");
                activeItem(listView.currentIndex);
                event.accepted = true;
            }
        }

        delegate: Item {

            id: item

            implicitHeight: txt.implicitHeight
            implicitWidth: listView.width

            required property int index
            required property string fileName
            required property string fileStatus
            required property color fileColor

            Label {
                id: txt
                text: item.fileName + " (" + item.fileStatus + ")"
                color: item.fileColor
               // font.family: root.fontFamily
               // font.pixelSize: root.fontSize
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = item.index;
                    listView.forceActiveFocus();
                }
            }
        }

        highlight: Item {

            Rectangle {
                x: 0
                y: 0
                height: parent.height
                width: listView.width
                color: "cornflowerblue"
                radius: 5
            }
        }
    }

}
