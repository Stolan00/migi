import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15

Item {
    id: libraryContainer
    width: parent.width
    height: parent.height

    ColumnLayout {
        anchors.fill: parent

        TabBar {
            id: tabBar
            Layout.fillWidth: true
            currentIndex: 0

            TabButton {
                text: "Currently Watching"
                background: Rectangle {
                    color: tabBar.currentIndex === 0 ? "lightslategray" : "slategray"
                }
                onClicked: libraryContainer.switchTab(0)
            }
            TabButton {
                text: "Plan to Watch"
                background: Rectangle {
                    color: tabBar.currentIndex === 1 ? "lightslategray" : "slategray"
                }
                onClicked: libraryContainer.switchTab(1)
            }
            TabButton {
                text: "Completed"
                background: Rectangle {
                    color: tabBar.currentIndex === 2 ? "lightslategray" : "slategray"
                }
                onClicked: libraryContainer.switchTab(2)
            }
            TabButton {
                text: "Dropped"
                background: Rectangle {
                    color: tabBar.currentIndex === 3 ? "lightslategray" : "slategray"
                }
                onClicked: libraryContainer.switchTab(3)
            }
        }

        StackLayout {
            id: stackLayout
            currentIndex: 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            AnimeList {
                id: currentlyWatchingList
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            AnimeList {
                id: planToWatchList
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            AnimeList {
                id: completedList
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
            AnimeList {
                id: droppedList
                Layout.fillWidth: true
                Layout.fillHeight: true
            }
        }
    }

    function switchTab(index) {
        if (stackLayout.currentIndex === index) return;
        stackLayout.currentIndex = index
        var entries = anilist.readAnimeWithEntriesFromDB(index)

        switch (index) {
            case 0:
                currentlyWatchingList.updateList(entries)
                break
            case 1:
                planToWatchList.updateList(entries)
                break
            case 2:
                completedList.updateList(entries)
                break
            case 3:
                droppedList.updateList(entries)
                break
        }
    }

    Component.onCompleted: {
        stackLayout.currentIndex = -1; // Set to an invalid index
        switchTab(0)
    }
}
