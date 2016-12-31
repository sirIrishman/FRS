#ifndef FRS_TRAININGDATA_H
#define FRS_TRAININGDATA_H

#include <opencv2/core.hpp>
#include <vector>

namespace frs {
    namespace native {
        class TrainingData sealed {
        public:
            ~TrainingData() {
                release();
            }

            std::vector<cv::Mat> imageCollection() const {
                return _imageCollection;
            }
            std::vector<int> classCollection() const {
                return _classCollection;
            }
            void add(cv::Mat const& image, int imageClass) {
                _imageCollection.push_back(image);
                _classCollection.push_back(imageClass);
            }
            void release() {
                for(int i = 0; i < _imageCollection.size(); i++)
                    _imageCollection[i].release();
                _imageCollection.clear();
                _classCollection.clear();
            }
            int size() const {
                return _imageCollection.size();
            }
            bool empty() const {
                return _imageCollection.size() == 0;
            }

        private:
            std::vector<cv::Mat> _imageCollection;
            std::vector<int> _classCollection;
        };
    }
}

#endif //FRS_TRAININGDATA_H
