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
                    source: model.imageLink
                    width: 50
                    height: 75
                    fillMode: Image.PreserveAspectFit
                }
                Text {
                    text: model.titleRomaji
                    wrapMode: Text.WordWrap
                    width: parent.width - 70
                }
            }
        }
    }

    function updateList(animeData) {
        animeListModel.clear();
        for (var i = 0; i < animeData.length; i++) {
            animeListModel.append(animeData[i]);
        }
    }
}
