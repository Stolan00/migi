import QtQuick 2.15
import QtQuick.Controls 2.15

Item {
    id: animeListContainer
    width: parent.width - 220
    height: 300

    ListView {
        id: animeListView
        width: parent.width
        height: parent.height

        model: ListModel {
            id: animeListModel
        }

        delegate: Loader {
            width: animeListContainer.width
            height: 100
            sourceComponent: animeItemDelegate
            active: visible
            // Pass the model data to the component
            property var modelData: model
        }
    }

    Component {
        id: animeItemDelegate

        Item {
            width: animeListContainer.width
            height: 100
            Row {
                spacing: 10

                Image {
                    width: 50
                    height: 75
                    fillMode: Image.PreserveAspectFit
                    source: anilist ? "file:///" + getAnimeImageSource(modelData.id) : ""
                }

                Text {
                    text: modelData.titleEnglish !== null && modelData.titleEnglish !== "" ? modelData.titleEnglish : modelData.titleRomaji
                    wrapMode: Text.WordWrap
                    width: parent.width - 70
                    font.family: 'Helvetica'
                }
            }
        }
    }

    function getAnimeImageSource(id) {
        if (anilist === null || anilist === undefined) {
            return "path/to/placeholder.png"; // Return a placeholder image path if anilist is not available
        }
        return anilist.getAnimeImage(id);
    }

    function getAnimeImageFullPath(id) {
        var relativePath = getAnimeImageSource(id);
        if (Qt.platform.os === "windows") {
            return "file:///" + relativePath.replace(/\\/g, "/");
        } else {
            return "file://" + relativePath;
        }
    }

    function updateList(animeData) {
        animeListModel.clear();
        for (var i = 0; i < animeData.length; i++) {
            animeListModel.append(animeData[i]);
        }
    }
}
