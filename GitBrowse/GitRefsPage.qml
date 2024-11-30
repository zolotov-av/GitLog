import QtQuick
import QtQuick.Layouts
import QtQuick.Controls
import GitCore

ColumnLayout {

    Component.onCompleted: {
        refsModel.update();
    }

    GitRefsModel {
        id: refsModel
        repository: gitRepo
    }

    ListView {
        id: listView
        model: refsModel
        activeFocusOnTab: true
        Layout.fillWidth: true
        Layout.fillHeight: true

        function gitMenu() {
            if (currentItem) {
                var pos = currentItem.mapToItem(listView, 0, currentItem.height);
                contextMenu.popup(pos.x, pos.y)
            }
        }

        function gitUpdate() {
            console.log("gitUpdate")
            refsModel.update();
        }

        function handleKeys(event)
        {
            if (event.modifiers !== 0)
                return;

            switch (event.key )
            {
            case Qt.Key_Space:
                console.log("listView Qt.Key_Space pressed");
                listView.gitMenu();
                event.accepted = true;
                break;
            }
        }

        Keys.onPressed: event => handleKeys(event)

        delegate: Item {

            id: item
            implicitHeight: txt.implicitHeight + 4
            implicitWidth: listView.width

            required property int index
            required property string refName
            required property string refFullName
            required property color refColor

            Rectangle {
                x: 4
                y: 1
                width: txt.implicitWidth + 4
                height: parent.height - 2
                color: refColor
                border.color: "black"
                border.width: 1
            }

            Label {
                id: txt
                x: 6
                text: item.refName
            }

            MouseArea {
                anchors.fill: parent
                onClicked: {
                    listView.currentIndex = item.index;
                    listView.forceActiveFocus();
                }

                onDoubleClicked: {
                    listView.currentIndex = item.index;
                    stackView.push("GitFilesPage.qml", {"refName" : item.refFullName, "filePath": "/"})
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
