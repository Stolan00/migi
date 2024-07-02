import QtQuick 2.15
import QtQuick.Controls 2.15
import com.migi.models

Item {
    id: animeListContainer
    width: parent.width - 220
    height: 300

    TableView {
        id: animeTableView
        anchors.fill: parent
        anchors.topMargin: 30 // Make room for the header
        flickDeceleration: 10000
        maximumFlickVelocity: 3000

        columnWidthProvider: function (column) {
            switch (column) {
                case 0: return 60  // Image column
                case 1: return animeTableView.width * 0.4  // Anime title
                case 2: return animeTableView.width * 0.25 // Progress
                case 3: return animeTableView.width * 0.15 // Score
                case 4: return animeTableView.width * 0.1  // Type
                default: return 0
            }
        }
        rowHeightProvider: function (row) { return 80; }

        model: AnimeListModel {
            id: animeListModel
        }

        delegate: Rectangle {
            implicitHeight: 80
            color: row % 2 === 0 ? "lightslategray" : "slategray"

            Item {
                anchors.fill: parent
                anchors.margins: 5

                Image {
                    width: 50
                    height: 70
                    fillMode: Image.PreserveAspectFit
                    source: column === 0 ? "file:///" + getAnimeImageSource(model.anime_id) : ""
                    visible: column === 0
                }

                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    text: {
                        switch (column) {
                            case 1: return model.titleEnglish && model.titleEnglish !== "" ? model.titleEnglish : model.titleRomaji
                            case 2: return model.progress + " / " + model.episodes
                            case 3: return model.score
                            case 4: return model.type
                            default: return ""
                        }
                    }
                    visible: column !== 0
                    elide: Text.ElideRight
                    font.family: 'Helvetica'
                }
            }
        }
    }

    // Header
    Row {
        id: header
        width: parent.width
        height: 30
        z: 1 // Ensure header is above the TableView

        Repeater {
            model: ["", "Anime title", "Progress", "Score", "Type"]
            Rectangle {
                width: animeTableView.columnWidthProvider(index)
                height: parent.height
                color: "#e0e0e0"
                border.width: 1
                border.color: "#c0c0c0"

                Text {
                    anchors.centerIn: parent
                    text: modelData
                    font.bold: true
                    font.family: 'Helvetica'
                }
            }
        }
    }

    // Functions remain the same
    function setStatusFilter(id) {
        animeListModel.setStatusFilter(id)
    }

    function getAnimeImageSource(id) {
        if (anilist === null || anilist === undefined) {
            return "path/to/placeholder.png";
        }
        return anilist.getAnimeImage(id);
    }

    function updateList(animeData) {
        animeListModel.clear();
        for (var i = 0; i < animeData.length; i++) {
            animeListModel.append(animeData[i]);
        }
        console.log("Updated list with data: ", animeData)
    }
}
