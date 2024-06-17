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

            Repeater {
                model: [
                    {text: "Currently Watching", index: 0},
                    {text: "Plan to Watch", index: 1},
                    {text: "Completed", index: 2},
                    {text: "Dropped", index: 3}
                ]
                delegate: TabButton {
                    text: modelData.text
                    background: Rectangle {
                        color: tabBar.currentIndex === modelData.index ? "lightslategray" : "slategray"
                    }

                    MouseArea {
                        anchors.fill: parent
                        onPressed: {
                            tabBar.currentIndex = modelData.index
                            libraryContainer.switchTab(modelData.index)
                        }
                    }
                }
            }
        }

        StackLayout {
            id: stackLayout
            currentIndex: 0
            Layout.fillWidth: true
            Layout.fillHeight: true

            Loader {
                id: currentlyWatchingLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                sourceComponent: currentlyWatchingComponent
                active: stackLayout.currentIndex === 0
            }
            Loader {
                id: planToWatchLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                sourceComponent: planToWatchComponent
                active: stackLayout.currentIndex === 1
            }
            Loader {
                id: completedLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                sourceComponent: completedComponent
                active: stackLayout.currentIndex === 2
            }
            Loader {
                id: droppedLoader
                Layout.fillWidth: true
                Layout.fillHeight: true
                sourceComponent: droppedComponent
                active: stackLayout.currentIndex === 3
            }
        }
    }

    Component {
        id: currentlyWatchingComponent
        AnimeList {
            id: currentlyWatchingList
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component {
        id: planToWatchComponent
        AnimeList {
            id: planToWatchList
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component {
        id: completedComponent
        AnimeList {
            id: completedList
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    Component {
        id: droppedComponent
        AnimeList {
            id: droppedList
            Layout.fillWidth: true
            Layout.fillHeight: true
        }
    }

    function switchTab(index) {
        if (stackLayout.currentIndex === index) return;
        stackLayout.currentIndex = index
        var entries = anilist.readAnimeWithEntriesFromDB(index)

        switch (index) {
            case 0:
                currentlyWatchingLoader.item.updateList(entries)
                break
            case 1:
                planToWatchLoader.item.updateList(entries)
                break
            case 2:
                completedLoader.item.updateList(entries)
                break
            case 3:
                droppedLoader.item.updateList(entries)
                break
        }
    }

    Component.onCompleted: {
        stackLayout.currentIndex = -1; // Set to an invalid index
        switchTab(0)
    }
}
