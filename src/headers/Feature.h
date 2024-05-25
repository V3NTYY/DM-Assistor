#include <functional>

#include "Stat.h"
class Feature {
public:
    enum FeatureType {
        CLASS,
        RACIAL,
        NORMAL
    };

    Feature(FeatureType type, std::function<void(Stat&)> applyFunc)
        : type(type), applyFunc(std::move(applyFunc)) {}

    void apply(Stat& statBlock) const {
        applyFunc(statBlock);
    }

private:
    FeatureType type;
    std::function<void(Stat&)> applyFunc;
};
