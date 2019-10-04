#ifndef HF_COLOR_H
#define HF_COLOR_H

namespace hf {
    class Color {
    public:
        Color();

        Color(const float grayscale, const float a = 255.f);

        Color(const float r, const float g, const float b, const float a = 255.f);

        float r;
        float g;
        float b;
        float a;

        /**
         * \brief Compares two colors using euclidean distance, and returns true if their are similar enough given a treshold
         * \param other The color to be compared against
         * \threshold The distance the colors need to be to be considered similar
         */
        bool is_similar_to(const Color& other, const float threshold) const;
    };
}

#endif //HF_VEC_H
