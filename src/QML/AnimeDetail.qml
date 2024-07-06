import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    id: detailWindow
    width: 800
    height: 600
    title: animeData ? (animeData.titleEnglish || animeData.titleRomaji) : ""
    visible: false  // Start hidden, we'll show it manually
    property var animeData: null
    property var updateProgressFunction: null

    function updateAnimeData(newData) {
        animeData = newData;
        updateDetails();
    }

    function updateDetails() {
        if (animeData) {
            titleText.text = animeData.titleEnglish || animeData.titleRomaji
            episodesSpinBox.updating = true;
            episodesSpinBox.value = animeData.progress;
            episodesSpinBox.updating = false;
            scoreComboBox.updating = true;
            scoreComboBox.currentIndex = Math.max(0, (animeData.score / 5) - 1);
            scoreComboBox.updating = false;
            // Update other fields...
        }
    }

    ColumnLayout {
        anchors.fill: parent
        anchors.margins: 10

        Text {
            id: titleText
            font.pixelSize: 24
            font.bold: true
        }

        RowLayout {
            Text { text: "Episodes watched:" }
            SpinBox {
                id: episodesSpinBox
                from: 0
                to: animeData ? animeData.episodes : 0
                value: animeData ? animeData.progress : 0
                property bool updating: false

                onValueChanged: {
                    if (!updating && animeData && updateProgressFunction) {
                        updateProgressFunction(animeData.entryId, value)
                    }
                }
            }
        }

        ComboBox {
            id: scoreComboBox
            model: ["-", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "9.5", "10"]
            property bool updating: false
            onCurrentIndexChanged: {
                if (!updating && animeData) {
                    var newScore = currentIndex !== 0 ? parseFloat(model[currentIndex]) * 10 : 0
                    console.log("Score value changed")
                    // Call your update function here
                    // updateScore(animeData.entryId, newScore)
                }
            }
        }

        Button {
            id: okButton
            text: "OK"
            font.pixelSize: 16
            font.bold: true
            onClicked: {
                if (updateProgressFunction && animeData) {
                    updateProgressFunction(animeData.entryId, episodesSpinBox.value)
                }
                detailWindow.close()
            }
        }
    }

    onAnimeDataChanged: updateDetails()
}
