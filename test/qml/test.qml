import QtQuick 2.0
import QtQuick.Window 2.0
import QtQuick.Controls 1.4
import QtQuick.Layouts 1.0
import MyTableViewModel 1.0
import "./"
import "Controls/"

//Window{
Rectangle {


    MyComboBox {
        id: comboBoxButton
//        text:qsTr("text")
        model:fruitModel
        currentIndex:1
//        anchors.fill: parent;
//        text: typeof(listModel) == "undefined"? "":typeof( listModel.get(currentIndex).attributes)=="undefined"?listModel.get(comboBox.currentIndex).name:listModel.get(currentIndex).attributes.get(currentIndex2).description
//        textSize: parent.height-4
//        textColor: comboBox.activeFocus ? comboBox.focus_textColor : comboBox.textColor
//        borderColor: comboBox.activeFocus ? comboBox.focus_borderColor : comboBox.borderColor
//        backgroundColor: comboBox.activeFocus ? comboBox.focus_backgroundColor : comboBox.backgroundColor
//        onClicked: comboxClick()
    }

    ListModel {
            id: fruitModel

            ListElement { name: "Core" }
            ListElement { name: "Deciduous" }

    }
//    Mycombobox{
//        listModel: fruitModel;anchors.centerIn: parent
//    }
//    ListModel {
//        id: fruitModel
//        ListElement { name: "Apple"
//            attributes: [
//                ListElement { description: "Core" },
//                ListElement { description: "Deciduous" }
//            ]
//        }
//        ListElement {
//            name: "Orange"
//            attributes: [
//                ListElement { description: "Citrus" }
//            ]
//        }
//        ListElement {
//            name: "mango"
//        }
//        ListElement {
//            name: "Banana"
//            attributes: [
//                ListElement { description: "Tropical" },
//                ListElement { description: "Seedless" },
//                ListElement { description: "Yellow" }
//            ]
//        }
//    }
}




