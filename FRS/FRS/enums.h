#ifndef FRS_ENUMS_H
#define FRS_ENUMS_H

namespace frs {
    enum FrameSource { 
        None = 0,
        ImageFile, 
        VideoFile, 
        VideoWebcam,
        ImageWebcam
    };

    enum ObjectDetectionAlgorithm {
        HaarCascade,
        LbpCascade
    };

    enum DetectableObjectType {
        Face
    };

    enum FaceRecognitionAlgorithm {
        Eigenfaces,
        Fisherfaces,
        Lbph
    };

    namespace native {
        enum FileType {
            Image, 
            Video 
        };
    }
}

#endif //FRS_ENUMS_H
