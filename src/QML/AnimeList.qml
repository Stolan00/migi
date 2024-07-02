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
        property int rowHeight: 100

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
        rowHeightProvider: function (row) { return rowHeight; }

        model: AnimeListModel {
            id: animeListModel
        }

        delegate: Rectangle {
            implicitHeight: 80
            color: row % 2 === 0 ? "lightslategray" : "slategray"

            property var animeItem: model

            Item {
                anchors.fill: parent
                anchors.margins: 5

                Image {
                    anchors.fill: parent
                    fillMode: Image.PreserveAspectFit
                    source: column === 0 ? "file:///" + getAnimeImageSource(animeItem.anime_id) : ""
                    visible: column === 0
                }

                Text {
                    anchors.fill: parent
                    verticalAlignment: Text.AlignVCenter
                    text: {
                        switch (column) {
                            case 1: return animeItem.titleEnglish && animeItem.titleEnglish !== "" ? animeItem.titleEnglish : animeItem.titleRomaji
                            case 2: return animeItem.progress + " / " + animeItem.episodes
                            case 4: return animeItem.type
                            default: return ""
                        }
                    }
                    visible: column !== 0 && column !== 3
                    elide: Text.ElideRight
                    font.family: 'Helvetica'
                    color: "white"
                }

                ComboBox {
                    // Use a different property name to avoid conflicts
                    property var animeData: parent.parent.animeItem
                    anchors.fill: parent
                    visible: column === 3
                    model: ["0", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "9.5", "10"]
                    currentIndex: Math.max(0, (animeData.score / 5 - 1))
                    onActivated: {
                        console.log(animeData.score, currentIndex)
                        //animeData.setData(animeListModel.index(row, column), parseFloat(currentText), 263) // Assuming 263 is the role for score
                    }
                    delegate: ItemDelegate {
                        width: parent.width
                        contentItem: Text {
                            text: modelData
                            color: "black"
                            font: parent.font
                            elide: Text.ElideRight
                            verticalAlignment: Text.AlignVCenter
                        }
                    }
                }
            }
        }
    }

    // Header (unchanged)
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

    // Functions (unchanged)
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
