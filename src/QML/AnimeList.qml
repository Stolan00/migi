import QtQuick 2.15
import QtQuick.Controls 2.15
import com.migi.models

Item {
    id: animeListContainer
    width: parent.width
    height: parent.height

    Rectangle {
        id: tableContainer
        anchors.fill: parent
        anchors.topMargin: header.height
        clip: true

        TableView {
            id: animeTableView
            anchors.fill: parent
            anchors.topMargin: 0
            boundsBehavior: Flickable.StopAtBounds

            columnWidthProvider: function (column) {
                switch (column) {
                    case 0: return 60  // Image column
                    case 1: return tableContainer.width * 0.4  // Anime title
                    case 2: return tableContainer.width * 0.25 // Progress
                    case 3: return tableContainer.width * 0.15 // Score
                    case 4: return tableContainer.width * 0.1  // Type
                    default: return 0
                }
            }
            rowHeightProvider: function (row) { return rowHeight; }

            model: AnimeListModel {
                id: animeListModel
            }

            Connections {
                target: animeListModel
                function onDataChanged(topLeft, bottomRight, roles) {
                    console.log("Data changed for rows", topLeft.row, "to", bottomRight.row)
                    refreshView()
                }
            }

            delegate: Rectangle {
                implicitHeight: 80
                implicitWidth: animeTableView.width
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
                        property var animeData: parent.parent.animeItem
                        property int currentRow: row
                        property int currentColumn: column
                        visible: column === 3
                        model: ["0", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "9.5", "10"]
                        currentIndex: Math.max(0, (animeData.score / 10) * 2 - 1)
                        onActivated: {
                                var newScore = parseFloat(model[currentIndex]) * 10;
                                var modelIndex = animeListModel.index(currentRow, currentColumn);
                                var scoreRole = 262; // Assuming 262 is the role for score

                                // Update the model with the new score
                                animeListModel.setData(modelIndex, newScore, scoreRole);
                                console.log("Updating score for row", currentRow, "to", newScore);

                                // Force refresh of the view
                                refreshView()
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
    }

    Row {
        id: header
        width: tableContainer.width
        height: 30
        z: 2
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
        refreshView()
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

    function refreshView() {
        animeTableView.contentY = 0
        animeTableView.forceLayout()
    }
}
