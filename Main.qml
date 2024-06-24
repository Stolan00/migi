// main.qml
import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import AppSettings 1.0
import "./src/QML"

Window {
    id: window
    width: 1200
    height: 480
    visible: true
    color: "lightslategray"
    title: qsTr("Migi")

    StackLayout {
        anchors.fill: parent

        // Splash screen item
        Item {
            id: splashScreen
            width: parent.width
            height: parent.height
            visible: true

            Rectangle {
                anchors.fill: parent
                color: "black"
                Text {
                    anchors.centerIn: parent
                    text: "Checking updates..."
                    color: "white"
                }
            }
        }

        // Main content item
        Item {
            id: mainContent
            width: parent.width
            height: parent.height
            visible: false

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

                    Button {
                        text: "Load Setting"
                        onClicked: {
                            var value = Settings.value(AppSettingsKey.AccountAnilistViewerId);
                            console.log("Loaded value:", value);
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
    }

    Component.onCompleted: {
        anilist.databaseReady.connect(function() {
            splashScreen.visible = false;
            mainContent.visible = true;
            library.switchTab(0);
        });
    }
}
