import QtQuick 2.15
import QtQuick.Controls 2.15
import QtQuick.Layouts 1.15
import QtQuick.Window 2.15

Window {
    id: detailWindow
    width: 800
    height: 600
    title: animeData ? (animeData.titleEnglish || animeData.titleRomaji) : ""
    visible: false
    property var animeData: null
    property var updateProgressFunction: null

    function updateAnimeData(newData) {
        animeData = newData;
        updateDetails();
    }

    function updateDetails() {
        if (animeData) {
            titleText.text = animeData.titleEnglish || animeData.titleRomaji
            episodesSpinBox.value = animeData.progress;

            scoreComboBox.currentIndex = Math.max(0, (animeData.score / 5) - 1);
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
            }
        }

        ComboBox {
            id: scoreComboBox
            model: ["-", "1", "1.5", "2", "2.5", "3", "3.5", "4", "4.5", "5", "5.5", "6", "6.5", "7", "7.5", "8", "8.5", "9", "9.5", "10"]
        }

        Button {
            id: okButton
            text: "OK"
            font.pixelSize: 16
            font.bold: true
            onClicked: {
                console.log("OK button clicked")
                if (updateProgressFunction && animeData) {
                    console.log("Calling updateProgressFunction with:", animeData.entryId, episodesSpinBox.value)
                    updateProgressFunction(animeData.entryId, episodesSpinBox.value)
                } else {
                    console.log("updateProgressFunction or animeData is null:", updateProgressFunction, animeData)
                }
                detailWindow.close()
            }
        }
    }

    onAnimeDataChanged: updateDetails()
}
