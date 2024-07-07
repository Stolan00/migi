import QtQuick 2.15
import QtQuick.Controls 2.15
import com.migi.models
import com.migi.anilist

Item {
    id: animeListContainer
    width: parent.width
    height: parent.height

    property var animeDetailWindow: null

    Timer {
        id: updateTimer
        interval: 1000
        repeat: false
        property var updateQueue: ({})
        onTriggered: {
            for (var entryId in updateQueue) {
                performProgressUpdate(entryId, updateQueue[entryId])
            }
            updateQueue = ({})
        }
    }

    Rectangle {
        id: tableContainer
        anchors.fill: parent
        anchors.topMargin: header.height
        color: "lightslategray"
        clip: true

        TableView {
            id: animeTableView
            anchors.fill: parent
            anchors.topMargin: 0
            boundsBehavior: Flickable.StopAtBounds

            columnWidthProvider: function (column) {
                var totalWidth = tableContainer.width
                var imageWidth = 60
                var remainingWidth = totalWidth - imageWidth

                switch (column) {
                    case 0: return imageWidth  // Image column
                    case 1: return remainingWidth * 0.5  // Anime title (50% of remaining)
                    case 2: return remainingWidth * 0.25 // Progress (25% of remaining)
                    case 3: return remainingWidth * 0.15 // Score (15% of remaining)
                    case 4: return remainingWidth * 0.10 // Type (10% of remaining)
                    default: return 0
                }
            }

            rowHeightProvider: function (row) { return 80 }

            model: AnimeListModel {
                id: animeListModel
            }

            // Connections {
            //     target: animeListModel
            //     function onDataChanged(topLeft, bottomRight, roles) {
            //         console.log("Data changed for rows", topLeft.row, "to", bottomRight.row)
            //         refreshView()
            //     }
            // }

            delegate: Rectangle {
                implicitHeight: 80
                implicitWidth: animeTableView.width
                color: row % 2 === 0 ? "lightslategray" : "slategray"

                property var animeItem: model
                property int currentProgress: animeItem.progress

                MouseArea {
                    anchors.fill: parent
                    onDoubleClicked: {
                        openAnimeDetail(animeItem, animeListModel.index(row, 2))
                    }

                    onClicked: function(mouse) {
                        mouse.accepted = true
                    }
                }

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
                                case 1: return animeItem.titleEnglish || animeItem.titleRomaji
                                case 4: return animeItem.type
                                default: return ""
                            }
                        }

                        visible: column !== 0 && column !== 2 && column !== 3
                        elide: Text.ElideRight
                        font.family: 'Helvetica'
                        color: "white"
                    }

                    Item {
                        anchors.fill: parent
                        visible: column === 2

                        Button {
                            id: minusButton
                            width: 20
                            height: 20
                            anchors.left: parent.left
                            anchors.verticalCenter: parent.verticalCenter
                            text: "-"
                            font.pixelSize: 16
                            font.bold: true

                            onClicked: {
                                if (currentProgress > 0) {
                                    currentProgress--
                                    //animeListModel.setData(animeListModel.index(row, 2), currentProgress, 261)
                                    updateProgress(model.entryId, currentProgress, animeListModel.index(row, 2))
                                }
                            }
                        }

                        ProgressBar {
                            id: progressBar
                            anchors.left: minusButton.right
                            anchors.right: plusButton.left
                            anchors.verticalCenter: parent.verticalCenter
                            anchors.margins: 5
                            height: 20
                            to: animeItem.episodes
                            value: currentProgress
                        }

                        Text {
                            anchors.centerIn: progressBar
                            text: currentProgress + " / " + animeItem.episodes
                            color: "black"
                            font.pixelSize: 12
                            font.bold: true
                        }

                        Button {
                                id: plusButton
                                width: 20
                                height: 20
                                anchors.right: parent.right
                                anchors.verticalCenter: parent.verticalCenter
                                text: "+"
                                font.pixelSize: 16
                                font.bold: true

                                onClicked: {
                                    if (currentProgress < animeItem.episodes) {
                                        currentProgress++
                                        //animeListModel.setData(animeListModel.index(row, 2), currentProgress, 261)
                                        updateProgress(model.entryId, currentProgress, animeListModel.index(row, 2))
                                    }
                                }
                            }
                        }
                    }

                    ComboBox {
                        property var animeData: parent.parent.animeItem
                        property int currentRow: row
                        property int currentColumn: column
                        visible: column === 3
                        model: ["-", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "9.5", "10"]
                        currentIndex: Math.max(0, (animeItem.score / 5) - 1)
                        onActivated: {
                            var newScore = currentIndex != 0 ? parseFloat(model[currentIndex]) * 10 : 0
                            var modelIndex = animeListModel.index(currentRow, currentColumn)
                            var scoreRole = 263 // Assuming 263 is the role for score

                            // Update the model with the new score
                            animeListModel.setData(modelIndex, newScore, scoreRole)
                            console.log("Updating score for row", currentRow, "to", newScore)

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

                // Connections {
                //     target: animeListModel
                //     function onDataChanged(topLeft, bottomRight, roles) {
                //         if (roles.indexOf(261) !== -1 && // 261 is the progress role
                //             topLeft.row <= row && row <= bottomRight.row &&
                //             topLeft.column <= 2 && 2 <= bottomRight.column) {
                //             currentProgress = animeItem.progress
                //         }
                //     }
                // }
            }

        Connections {
            target: animeListModel
            function onDataChanged(topLeft, bottomRight, roles) {
                console.log("Data changed signal received in QML")
                console.log("Rows:", topLeft.row, "to", bottomRight.row)
                console.log("Roles:", roles)
                refreshView()
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

    function setStatusFilter(id) {
        animeListModel.setStatusFilter(id)
        refreshView()
    }

    function getAnimeImageSource(id) {
        if (anilist === null || anilist === undefined) {
            return "path/to/placeholder.png"
        }
        return anilist.getAnimeImage(id)
    }

    function updateList(animeData) {
        animeListModel.clear()
        for (var i = 0; i < animeData.length; i++) {
            animeListModel.append(animeData[i])
        }
        console.log("Updated list with data: ", animeData)
    }

    function refreshView() {
        console.log("Refreshing view")
        animeListModel.forceUpdate()  // Call forceUpdate on the model, not the view
        animeTableView.forceLayout()
        console.log("View refreshed")
    }

    function updateProgress(entryId, newProgress, modelIndex) {
        console.log("updateProgress called with:", entryId, newProgress, modelIndex)

        // Update the model
        if (modelIndex !== undefined) {
            animeListModel.setData(modelIndex, newProgress, 261) // Assuming 261 is the ProgressRole
        }

        // Queue the update for the server
        updateTimer.updateQueue[entryId] = newProgress
        updateTimer.restart()

        console.log("Calling refreshView")
        refreshView()
    }

    function performProgressUpdate(entryId, newProgress) {
        console.log("performProgressUpdate called with:", entryId, newProgress)
        const queryText = `
        mutation ($id: Int, $progress: Int) {
            SaveMediaListEntry (id: $id, progress: $progress) {
                id
                progress
            }
        }`
        const variables = {
            "id": entryId,
            "progress": newProgress
        }
        console.log("Updating progress for entry", entryId, "to", newProgress)
        anilist.updateAnimeEntryOnAnilist(queryText, true, variables, Anilist.RequestType.UpdateAnimeEntryOnAnilist)

        console.log("Calling refreshView from performProgressUpdate")
        refreshView()
    }

    function openAnimeDetail(animeItem, modelIndex) {
        if (!animeDetailWindow) {
            var component = Qt.createComponent("AnimeDetail.qml")
            if (component.status === Component.Ready) {
                animeDetailWindow = component.createObject(animeListContainer, {
                    "animeData": animeItem,
                    "updateProgressFunction": function(entryId, newProgress) {
                        console.log("updateProgressFunction called from AnimeDetail")
                        updateProgress(entryId, newProgress, modelIndex)
                    }
                })
                if (animeDetailWindow) {
                    animeDetailWindow.visible = true
                } else {
                    console.error("Error creating AnimeDetail window:", component.errorString())
                }
            } else if (component.status === Component.Error) {
                console.error("Error loading AnimeDetail.qml:", component.errorString())
            }
        } else {
            animeDetailWindow.updateAnimeData(animeItem)
            animeDetailWindow.updateProgressFunction = function(entryId, newProgress) {
                updateProgress(entryId, newProgress, modelIndex)
            }
            animeDetailWindow.visible = true
        }
    }
}
