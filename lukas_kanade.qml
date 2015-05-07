import QtQuick 2.4
import QtQuick.Controls 1.3

Item {
    width: 300
    height: 240
    SpinBox {
        id: countIteration
        x: 200
        y: 40
        minimumValue : 1
        maximumValue : 10
    }
    SpinBox {
        id: windowSearch
        x: 200
        y: 80
        minimumValue : 2
        maximumValue : 1000
        value : 16
    }
    SpinBox {
        id: stepGrid
        x: 200
        y: 120
        minimumValue : 1
        maximumValue : 500
        value : 16
    }

    Label {
        id: countIterationLabel
        x: 20
        y: 40
        text: qsTr("Count iteration")
    }

    Label {
        id: labelWindowSearch
        x: 20
        y: 80
        text: qsTr("Size window search")
    }

    Label {
        id: labelStepGrid
        x: 20
        y: 120
        text: qsTr("Step for grid")
    }

    TextField {
        id: outDirField
        objectName: "outDirField"
        x: 20
        y: 160
        placeholderText: qsTr("Output directory")
    }

    Button {
        id: buttonOutDirField
        x: 200
        y: 160
        text: qsTr("Browse")
    }

    CheckBox {
        id: checkBox1
        x: 200
        y: 200
        text: qsTr("")
        checked: false
    }

    Label {
        id: debugModeON
        x: 20
        y: 200
        text: qsTr("Debug mode ON")
    }


    function getcmd()
    {
      return "lukas_kanade_qt";
    }

    function getarg()
    {
      var a = 
      [
        "-l", outDirField.text + "/list",//load list image
        "-o", outDirField.text,//output directory
        "-i", countIteration.value,//count iteration (1 by default)
        "-w", windowSearch.value,//size window search (3px by default)
        "-g", stepGrid.value,//step for grid (5px by default)
        /*"-h", //show help
        "-d", debugModeON,//debug mod on
        "-v" //show version*/
      ];

      return a;
    }

    function outdir() 
    {
      return outDirField.text;
    }

}

