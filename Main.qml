import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import AppSettings 1.0
import "./src/QML"

Window {
    width: 800
    height: 480
    visible: true
    title: qsTr("Migi")

    RowLayout {
        anchors.fill: parent

        Sidebar {
            id: sidebar
            width: 200
            Layout.fillHeight: true
            onSelectedIndexChanged: {
                animeList.visible = selectedIndex == 1
                readButton.visible = selectedIndex == 1
            }
        }

        ColumnLayout {
            Layout.fillWidth: true
            Layout.fillHeight: true

            AnimeList {
                id: animeList
                Layout.fillWidth: true
                Layout.fillHeight: true
                anchors.margins: 5
                visible: sidebar.selectedIndex == 1
            }

            Button {
                id: readButton
                text: "Read From Database"
                Layout.alignment: Qt.AlignHCenter | Qt.AlignBottom
                visible: sidebar.selectedIndex == 1
                onClicked: {
                    var entries = anilist.readAnimeWithEntriesFromDB();
                    animeList.updateList(entries);
                }
            }
        }
    }
}
