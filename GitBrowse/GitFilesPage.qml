import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore
import Browse

ColumnLayout {
    id: root

    spacing: 0

    required property string refName
    required property string filePath

    Component.onCompleted: {
        filesModel.update();
        listView.currentIndex = 0;
    }

    StackView.onActivated: listView.forceActiveFocus()

    GitFilesModel {
        id: filesModel
        repository: gitRepo
        referenceName: root.refName
        filePath: root.filePath
    }

    Rectangle {
        id: header
        implicitHeight: headerLayout.implicitHeight + rowLayout.anchors.margins * 2
        color: "burlywood"
        Layout.fillWidth: true

        RowLayout {
            id: headerLayout
            anchors.fill: parent
            anchors.margins: 8
            spacing: 26

            Label {
                text: "ref: " + refName
            }

            Label {
                text: "path: " + filesModel.filePath
            }

            Item {
                Layout.fillWidth: true
            }

        }

    }

    ListView {
        id: listView
        model: filesModel
        activeFocusOnTab: true
        Layout.fillWidth: true
        Layout.fillHeight: true

        function gitEnter() {
            if (currentItem) {
                var item = currentItem;

                if ( item.fileType === "up" ) {
                    gitBack();
                    return
                }

                if ( item.fileType !== "tree" )
                    return

                filesModel.enter(item.fileName);

                listView.currentIndex = 0;
            }
        }

        function gitBack() {
            console.log("gitBack() " + filesModel.filePath + " leave")

            if ( filesModel.filePath==="/" ) {
                console.log("gitBack() pop")
                stackView.pop();
                return;
            }

            listView.currentIndex = filesModel.leave();
        }

        function handleKeys(event)
        {
            if (event.modifiers !== 0)
                return;

            switch (event.key )
            {
            case Qt.Key_Enter:
            case Qt.Key_Return:
                console.log("listView Qt.Key_Enter pressed");
                listView.gitEnter()
                event.accepted = true;
                break;
            case Qt.Key_Backspace:
                console.log("GitFilesPage Qt.Key_Backspace pressed");
                listView.gitBack();
                event.accepted = true
                break;
            }
        }

        Keys.onPressed: event => handleKeys(event)

        delegate: Item {

            id: item
            implicitHeight: txt.implicitHeight
            implicitWidth: listView.width

            required property int index
            required property string fileName
            required property string fileType

            Label {
                id: txt
                text: item.fileName
                font.bold: (item.fileType === "tree")
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = item.index;
                    listView.forceActiveFocus();
                }

                onDoubleClicked: listView.gitEnter()
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

        Item {
            enabled: listView.activeFocus

            Shortcut {
                sequences: ["Ctrl+R", "F5"]
                onActivated: listView.gitUpdate()
            }

        }

        Menu {
            id: contextMenu

            MenuItem {
                text: qsTr("Stage") + " (S)"
                onTriggered: listView.gitStage()
            }
            MenuItem {
                text: qsTr("Restore staged") + " (R)"
                onTriggered: listView.gitRestoreStaged()
            }
            MenuItem {
                text: qsTr("Checkout HEAD") + " (C)"
                onTriggered: listView.gitCheckoutHead()
            }
            MenuItem {
                text: qsTr("Remove") + " (DEL)"
                onTriggered: listView.gitRemoveFile();
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
                text: qsTr("Refresh") + " (Ctrl+R)"
            }

            Item {
                Layout.fillWidth: true
            }
        }

    }

}
