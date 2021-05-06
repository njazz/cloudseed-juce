///> @file classes for layout

#pragma once

#include <vector>

class LayoutValue {
public:
    LayoutValue() { }

    enum Type {
        LVT_Fill,
        LVT_Pixels,
        LVT_Ratio,
        LVT_Percent
    };

    Type type = LVT_Fill;
    float value = 0;

    static LayoutValue Pixels(const float& f);
    static LayoutValue Ratio(const float& f);
    static LayoutValue Percent(const float& f);

    static LayoutValue Fill() { return LayoutValue(); }
};

struct LayoutData {
    struct Unit {
        LayoutValue spacing;
        LayoutValue size;

        Unit(const LayoutValue& spacing_ = LayoutValue(), const LayoutValue& size_ = LayoutValue())
            : spacing(spacing_)
            , size(size_) {};
    };

    std::vector<Unit> units = {};
    LayoutValue spacing = LayoutValue();

    struct Result {
        float offset = 0;
        float size = 0;
    };

    std::vector<LayoutValue> getFlatVector();
    size_t getFillCount();
    size_t getRatioValueCount();

    float getRatioValueLength(); //sum of 'Ratio' values
    float getPixelSum(const float &width); // sum of 'Pixels' values

    std::vector<Result> calculate(const float& width);
};

// -----------

//
#define CSJ_LAYOUT_BEGIN(__x) \
    const auto __x = []() -> LayoutData { \
    LayoutData _layoutData; \
    _layoutData.units = {

//
#define CSJ_LAYOUT_MARGIN_FILL() LayoutData::Unit(\
LayoutValue::Fill(),

#define CSJ_LAYOUT_MARGIN_PX(__x) LayoutData::Unit(\
LayoutValue::Pixels(__x),

#define CSJ_LAYOUT_MARGIN_PERCENT(__x) LayoutData::Unit(\
LayoutValue::Percent(__x),

#define CSJ_LAYOUT_MARGIN_RATIO(__x) LayoutData::Unit(\
LayoutValue::Ratio(__x),

//
#define CSJ_LAYOUT_FILL() \
LayoutValue::Fill()),

#define CSJ_LAYOUT_PX(__x) \
LayoutValue::Pixels(__x)),

#define CSJ_LAYOUT_PERCENT(__x) \
LayoutValue::Percent(__x)),

#define CSJ_LAYOUT_RATIO(__x) \
LayoutValue::Ratio(__x)),

//
#define CSJ_LAYOUT_END_PX(__x)                       \
    }                                               \
    ;                                               \
    _layoutData.spacing = LayoutValue::Pixels(__x); \
return _layoutData; \
    }                                               \
    ;

#define CSJ_LAYOUT_END()                        \
    }                                          \
    ;                                          \
    _layoutData.spacing = LayoutValue::Fill(); \
return _layoutData; \
    }                                          \
    ;

#define CSJ_LAYOUT_END_PERCENT(__x)                       \
    }                                                \
    ;                                                \
    _layoutData.spacing = LayoutValue::Percent(__x); \
return _layoutData; \
    }                                                \
    ;

#define CSJ_LAYOUT_END_RATIO(__x)                       \
    }                                              \
    ;                                              \
    _layoutData.spacing = LayoutValue::Ratio(__x); \
return _layoutData; \
    }                                              \
    ;
