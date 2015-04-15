import QtQuick 2.4
import QtQuick.Controls 1.3

Item {
    width: 300
    height: 300
    SpinBox {
        id: countIteration
        x: 200
        y: 100
        minimumValue : 1
        maximumValue : 10
    }
    SpinBox {
        id: windowSearch
        x: 200
        y: 140
        minimumValue : 3
        maximumValue : 10
    }
    SpinBox {
        id: stepGrid
        x: 200
        y: 180
        minimumValue : 5
        maximumValue : 30
    }

    Label {
        id: countIterationLabel
        x: 20
        y: 100
        text: qsTr("Count iteration")
    }

    Label {
        id: labelWindowSearch
        x: 20
        y: 140
        text: qsTr("Size window search")
    }

    Label {
        id: labelStepGrid
        x: 20
        y: 180
        text: qsTr("Step for grid")
    }


    TextField {
        id: loadLeftImage
        x: 20
        y: 20
        placeholderText: qsTr("Path to left image")
    }

    TextField {
        id: loadRightImage
        x: 20
        y: 60
        placeholderText: qsTr("Path to right image")
    }

    Button {
        id: buttonLoadRightImage
        x: 200
        y: 20
        text: qsTr("Browse")
    }

    Button {
        id: buttonLoadLeftImage
        x: 200
        y: 60
        text: qsTr("Browse")
    }

    TextField {
        id: outDirField
        x: 20
        y: 220
        placeholderText: qsTr("Output directory")
    }

    Button {
        id: buttonOutDirField
        x: 200
        y: 220
        text: qsTr("Browse")
    }

    CheckBox {
        id: checkBox1
        x: 200
        y: 260
        text: qsTr("")
        checked: false
    }

    Label {
        id: debugModeON
        x: 20
        y: 260
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
        "-l", loadLeftImage.text,//load left image
        "-r", loadRightImage.text,//load right image
        "-i", countIteration,//count iteration (1 by default)
        "-w", windowSearch,//size window search (3px by default)
        "-g", stepGrid,//step for grid (5px by default)
        "-v", //show version
        "-h", //show help
        "-d", debugModeON,//debug mod on
        "-o", outDirField.text,//output directory
      ];

      return a;
    }

    function outdir() 
    {
      return outDirField.text;
    }

}

