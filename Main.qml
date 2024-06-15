import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import AppSettings 1.0
import "./src/QML"

Window {
    width: 1200
    height: 480
    visible: true
    color: "lightslategray"
    title: qsTr("Migi")

    RowLayout {
        anchors.fill: parent

        Sidebar {
            id: sidebar
            width: 200
            Layout.fillHeight: true
            onSelectedIndexChanged: {
                mainWindow.toggle()
            }
        }

        ColumnLayout {
            id: mainWindow
            Layout.fillWidth: true
            Layout.fillHeight: true

            AnimeLibrary {
                id: library
                property int index: 1
                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.margins: 5
                function toggle() {
                    visible = sidebar.selectedIndex === index
                    enabled = sidebar.selectedIndex === index
                }
            }

            function toggle() {
                for (var i = 0; i < mainWindow.children.length; i++) {
                    var child = mainWindow.children[i];
                    if (child.toggle) child.toggle()
                }
            }
        }
    }
}
