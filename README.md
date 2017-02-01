# Face Recognition Software (FRS) x64

It's my diploma C++ project I initially made in 2012 for the specialist degree of "Software for computers and computer-aided systems" in Tula State University (TSU). And recently I've performed small refresh: updated dependencies to the latest versions and made small refactoring.

![Screenshot](http://i.imgur.com/z1sF7xq.png)

## Features
* Loading images and videos;
* Capturing images and videos from a webcam;
* 2 object (faces) detection methods:
  1. The Haar Cascades method;
  2. The Local Binary Patterns (LBP) Cascades method;
* 3 face recognition methods (require prepared training data):
  1. The Eigenfaces method;
  2. The Fisherfaces method;
  3. The Local Binary Patterns Histograms (LBPH) method;
* Built-in help system.

## Dependencies
* [Visual Studio 2015](https://www.visualstudio.com/downloads/)+;
* [Qt 5.6.2](https://www.qt.io/download/);
* [OpenCV 3.2.0](https://github.com/opencv/opencv/releases/tag/3.2.0) + [OpenCV Contrib 3.2.0](https://github.com/opencv/opencv_contrib/releases/tag/3.2.0) (you will need to build by yourself).

## How to build
1. Make sure that you have all above dependencies installed on your machine.
2. Open the [opencv.props](https://github.com/sirIrishman/FRS/blob/master/src/FRS/opencv.props) file and set up all OpenCV dependencies path properties according to their arrangement at your machine.
3. Check that you set up the Qt artifacts path (the `QTDIR` macros) within [Qt VS Tools](https://marketplace.visualstudio.com/items?itemName=TheQtCompany.QtVisualStudioTools2015).
4. Build the [FRS.sln](https://github.com/sirIrishman/FRS/blob/master/src/FRS.sln) solution file.
