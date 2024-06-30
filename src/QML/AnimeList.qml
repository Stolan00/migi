import QtQuick 2.15
import QtQuick.Controls 2.15
import com.migi.models

Item {
    id: animeListContainer
    width: parent.width - 220
    height: 300

    ListView {
        id: animeListView
        width: parent.width
        height: parent.height

        model: AnimeListModel {
            id: animeListModel
        }

        delegate: Loader {
            width: animeListContainer.width
            height: 150 // Adjust height according to your needs
            sourceComponent: animeItemDelegate
            property var modelData: model
        }

        flickDeceleration: 10000
        maximumFlickVelocity: 3000
    }

    // Define animeItemDelegate component here
    Component {
        id: animeItemDelegate

        Item {
            width: animeListContainer.width
            height: 150 // Adjust height according to your needs
            Row {
                spacing: 10

                Image {
                    width: 50
                    height: 75
                    fillMode: Image.PreserveAspectFit
                    source: modelData && modelData.anime_id ? "file:///" + getAnimeImageSource(modelData.anime_id) : ""
                }

                Column {
                    spacing: 5

                    Text {
                        text: modelData ? (modelData.titleEnglish && modelData.titleEnglish !== "" ? modelData.titleEnglish : modelData.titleRomaji) : ""
                        wrapMode: Text.WordWrap
                        font.family: 'Helvetica'
                    }

                    Row {
                        spacing: 10

                        Text {
                            text: modelData ? "Progress: " +  modelData.progress : ""
                            wrapMode: Text.WordWrap
                            font.family: 'Helvetica'
                        }

                        Text {
                            text: modelData ? "Score: " +  modelData.score : ""
                            wrapMode: Text.WordWrap
                            font.family: 'Helvetica'
                        }

                        Text {
                            text: modelData ? modelData.type : ""
                            wrapMode: Text.WordWrap
                            font.family: 'Helvetica'
                        }
                    }
                }
            }
        }
    }

    // Functions and logic specific to your application
    function setStatusFilter(id) {
        animeListModel.setStatusFilter(id)
    }

    function getAnimeImageSource(id) {
        if (anilist === null || anilist === undefined) {
            return "path/to/placeholder.png"; // Return a placeholder image path if anilist is not available
        }
        return anilist.getAnimeImage(id);
    }

    function updateList(animeData) {
        animeListModel.clear();
        for (var i = 0; i < animeData.length; i++) {
            animeListModel.append(animeData[i]);
            console.log(animeData[i]);
        }
        console.log("Updated list with data: ", animeData)
    }
}
