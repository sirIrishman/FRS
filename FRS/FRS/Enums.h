#ifndef FRS_ENUMS_H
#define FRS_ENUMS_H

namespace FRS {
    enum FrameSource { 
        None = 0,
        ImageFile, 
        VideoFile, 
        VideoWebcam,
        ImageWebcam
    };

    enum RecognitionAlgorithm {
        HaarCascade,
        LbpCascade
    };

    enum RecognizableObjectType {
        Face
    };

    namespace Native {
        enum FileType {
            Image, 
            Video 
        };
    }
}

#endif //FRS_ENUMS_H
