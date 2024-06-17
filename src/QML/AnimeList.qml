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

        delegate: Item {
            width: animeListContainer.width
            height: 100
            Row {
                spacing: 10

                Image {
                    width: 50
                    height: 75
                    fillMode: Image.PreserveAspectFit
                    source: "file:///" + getAnimeImageSource(model.id)
                }

                Text {
                    text: model.titleEnglish !== null && model.titleEnglish !== "" ? model.titleEnglish : model.titleRomaji
                    wrapMode: Text.WordWrap
                    width: parent.width - 70
                    font.family: 'Helvetica'
                }
            }
        }
    }

    // Define the function in the top-level Item
    function getAnimeImageSource(id) {
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
