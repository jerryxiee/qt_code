import QtQuick 2.0
//import QtQuick.Window 2.0
//import QtQuick.Controls 1.2
import QtQuick.Controls 2.2
import QtQuick.Layouts 1.3
import MyTableViewModel 1.0
import QtQuick.Controls.Styles 1.2
import "./"
import "Controls/"

//Window{
Rectangle {

    color:"black"


//    MyComboBox {
//        id: comboBoxButton
////        text:qsTr("text")
//        model:fruitModel
//        backgroundColor:"black"
//        textColor:"white"
////        currentIndex:0
////        anchors.fill: parent;
////        text: typeof(listModel) == "undefined"? "":typeof( listModel.get(currentIndex).attributes)=="undefined"?listModel.get(comboBox.currentIndex).name:listModel.get(currentIndex).attributes.get(currentIndex2).description
////        textSize: parent.height-4
////        textColor: comboBox.activeFocus ? comboBox.focus_textColor : comboBox.textColor
////        borderColor: comboBox.activeFocus ? comboBox.focus_borderColor : comboBox.borderColor
////        backgroundColor: comboBox.activeFocus ? comboBox.focus_backgroundColor : comboBox.backgroundColor
////        onClicked: comboxClick()
//    }

//    ListModel {
//            id: fruitModel

//            ListElement { name: "Core" }
//            ListElement { name: "Deciduous" }

//    }
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


    TabBar{
            id:bar;
            width:100*count

            onCurrentIndexChanged: console.log("index "+currentIndex)



            TabButton{
                text:qsTr("Home");

            }
            TabButton{

                text:qsTr("Discover");
            }
            TabButton{
                text:qsTr("Activity");
            }
        }


        StackLayout{
            width:parent.width;
            height: parent.height - bar.height
            anchors.top: bar.bottom
            currentIndex:bar.currentIndex;
            Item{
                id:homeTab
//                Button{
//                    style:ButtonStyle{
//                        label:Text {
//                            id: name
//                            text: qsTr("text")
//                            color: control.pressed?"red":"blue"
//                        }
//                    }
//                }
                Text {
                    anchors.centerIn: parent
                    color: "white"

                    text: qsTr("text")
                    font.pixelSize: 24
                }
            }
            Item{
                id:discoverTab
                Text {
                    anchors.centerIn: parent
                    color: "white"
                    text: qsTr("text1")
                    font.pixelSize: 24
                }
            }
            Item{
                id:activityTab
                Text {
                    anchors.centerIn: parent
                    color: "white"
                    text: qsTr("text2")
                    font.pixelSize: 24
                }
            }

        }

//    TabBar{
//        id:bar
//        width:parent.width
//        Repeater{
//            model:["First","Second","Third","Fourth","Fifth","Fifth","Fifth","Fifth","Fifth"]
//            TabButton{
//                text:modelData;
//                width:Math.max(100,bar.width/5);
//            }
//        }
//    }

}




