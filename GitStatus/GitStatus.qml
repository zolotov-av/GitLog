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
            path: "/home/alex/prj/test/GitTest"
        }
    }

    ColumnLayout {
        anchors.fill: parent

        ListView {
            id: listView
            model: statusModel
            activeFocusOnTab: true
            Layout.fillWidth: true
            Layout.fillHeight: true

            function gitStage() {
                if ( currentItem )
                    console.log("gitStage: " + currentItem.fileName);
                else
                    console.log("gitStage: <NONE>")
            }

            function gitUnstage() {
                if ( currentItem )
                    console.log("gitUnstage: " + currentItem.fileName);
                else
                    console.log("gitUnstage: <NONE>")
            }

            function gitDiff() {
                if ( currentItem )
                    console.log("gitDiff: " + currentItem.fileName);
                else
                    console.log("gitDiff: <NONE>")
            }

            function handleKeys(event)
            {
                switch (event.key )
                {
                case Qt.Key_Enter:
                case Qt.Key_Return:
                    console.log("listView Keys.onPressed");
                    //activeItem(listView.currentIndex);
                    event.accepted = true;
                    break;
                case Qt.Key_Space:
                    console.log("listView Qt.Key_Space pressed");
                    if (currentItem) {
                        var pos = currentItem.mapToItem(listView, 0, currentItem.height);
                        contextMenu.popup(pos.x, pos.y)
                    }
                    event.accepted = true;
                    break;
                case Qt.Key_S:
                    console.log("listView Qt.Key_S pressed");
                    listView.gitStage();
                    event.accepted = true;
                    break;
                case Qt.Key_U:
                    console.log("listView Qt.Key_U pressed");
                    listView.gitUnstage();
                    event.accepted = true;
                    break;
                case Qt.Key_D:
                    console.log("listView Qt.Key_D pressed");
                    listView.gitDiff();
                    event.accepted = true;
                    break;
                case Qt.Key_Delete:
                    console.log("listView Qt.Key_Delete pressed");
                    event.accepted = true;
                    break;
                }
            }

            Keys.onPressed: event => handleKeys(event)

            delegate: Item {

                id: item

                implicitHeight: txt.implicitHeight
                implicitWidth: listView.width

                readonly property point menuPos: Qt.point(x + width / 4, y + height)

                required property int index
                required property string fileName
                required property string fileStatus
                required property color fileColor

                Label {
                    id: txt
                    text: item.fileName + " (" + item.fileStatus + ")"
                    color: item.fileColor
                }

                MouseArea {
                    anchors.fill: parent
                    onClicked: {
                        listView.currentIndex = item.index;
                        listView.forceActiveFocus();
                    }
                }

                MouseArea {
                    anchors.fill: parent
                    acceptedButtons: Qt.RightButton
                    onClicked: (mouse) => {
                        listView.currentIndex = item.index
                        var pos = item.mapToItem(listView, mouse.x, mouse.y)
                        contextMenu.x = pos.x
                        contextMenu.y = pos.y
                        contextMenu.open()
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

            Menu {
                id: contextMenu

                MenuItem {
                    text: "Stage File\tS"
                    onTriggered: listView.gitStage()
                }
                MenuItem {
                    text: "Unstage File\tU"
                    onTriggered: listView.gitUnstage();
                }
                MenuItem {
                    text: "Show Diff\tD"
                    onTriggered: listView.gitDiff();
                }
            }
        }

        Rectangle {
            id: hints
            color: "burlywood"
            implicitHeight: rowLayout.implicitHeight + rowLayout.anchors.margins * 2
            Layout.fillWidth: true

            RowLayout {
                id: rowLayout
                anchors.fill: parent
                anchors.margins: 8
                spacing: 26

                Label {
                    text: qsTr("Keys") + ":"
                }

                Label {
                    text: qsTr("Stage") + " (S)"
                }

                Label {
                    text: qsTr("Unstage") + " (U)"
                }

                Label {
                    text: qsTr("Diff") + " (D)"
                }

                Label {
                    text: qsTr("Menu") + " (Space)"
                }

                Item {
                    Layout.fillWidth: true
                }
            }

        }
    }
}
