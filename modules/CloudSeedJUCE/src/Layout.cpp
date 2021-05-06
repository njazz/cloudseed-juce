#include "Layout.hpp"

LayoutValue LayoutValue::Pixels(const float& f)
{
    LayoutValue ret;
    ret.type = LVT_Pixels;
    ret.value = f;
    return ret;
}

LayoutValue LayoutValue::Ratio(const float& f)
{
    LayoutValue ret;
    ret.type = LVT_Ratio;
    ret.value = f;
    return ret;
}

LayoutValue LayoutValue::Percent(const float& f)
{
    LayoutValue ret;
    ret.type = LVT_Percent;
    ret.value = f;
    return ret;
}

// ----------

std::vector<LayoutValue> LayoutData::getFlatVector()
{
    std::vector<LayoutValue> ret;
    for (auto& v : units) {
        ret.push_back(v.spacing);
        ret.push_back(v.size);
    }
    ret.push_back(spacing);
    return ret;
}

size_t LayoutData::getFillCount()
{
    size_t ret = 0;

    for (auto& e : getFlatVector()) {
        if (e.type == LayoutValue::LVT_Fill)
            ret++;
    }

    //if (spacing.type == LayoutValue::LVT_Fill)
    //        ret++;

    return ret;
}

size_t LayoutData::getRatioValueCount()
{
    size_t ret = 0;

    for (auto& e : getFlatVector()) {
        if (e.type == LayoutValue::LVT_Ratio)
            ret++;
    }

    if (spacing.type == LayoutValue::LVT_Ratio)
        ret++;

    return ret;
}

float LayoutData::getRatioValueLength()
{
    float ret = 0;

    for (auto& e : getFlatVector()) {
        if (e.type == LayoutValue::LVT_Ratio)
            ret += e.value;
    }

    if (spacing.type == LayoutValue::LVT_Ratio)
        ret += spacing.value;

    return ret;
}

float LayoutData::getPixelSum(const float& width)
{
    float ret = 0;

    for (auto& e : getFlatVector()) {
        if (e.type == LayoutValue::LVT_Pixels)
            ret += e.value;

        if (e.type == LayoutValue::LVT_Percent)
            ret += e.value / 100. * width;
    }

    if (spacing.type == LayoutValue::LVT_Pixels)
        ret += spacing.value;
    if (spacing.type == LayoutValue::LVT_Percent)
        ret += spacing.value / 100. * width;

    return ret;
}

std::vector<LayoutData::Result> LayoutData::calculate(const float& width)
{
    auto flat = getFlatVector();
    float len = getRatioValueLength();

    float pxSum = getPixelSum(width);
    float pxCoeff = 1.0f;

    // no coeff here:
    float restPx = width - pxSum;

    // if width is not enough:
    if ((pxSum > width) && (pxSum > 0)) {
        pxCoeff = width / pxSum;
        restPx = 0;
    }

    // fills:
    float fc = getFillCount();
    float fillSize = 0;
    if (fc)
        fillSize = restPx / fc;

    float rc = getRatioValueCount();

    // fill with ratio
    if (rc > 0) {
        restPx *= rc / (rc + fc);
    }
    if (fc > 0) {
        fillSize *= fc / (rc + fc);
    }

    // ratio normalization
    float ratioNorm = 1;
    if (len > 0)
        ratioNorm = 1.0 / len;

    //    printf("len %f fc %f pxSum %f pxCoeff %f restPx %f fillSize %f\n", len, fc, pxSum, pxCoeff, restPx, fillSize);

    std::vector<LayoutData::Result> ret;
    for (auto& e : units) {
        LayoutData::Result r;

        if (e.spacing.type == LayoutValue::LVT_Fill)
            r.offset = fillSize;

        if (e.spacing.type == LayoutValue::LVT_Pixels)
            r.offset = e.spacing.value * pxCoeff;

        if (e.spacing.type == LayoutValue::LVT_Percent)
            r.offset = e.spacing.value / 100.0f * width * pxCoeff;

        if (e.spacing.type == LayoutValue::LVT_Ratio)
            r.offset = e.spacing.value * restPx * ratioNorm;

        //
        if (e.size.type == LayoutValue::LVT_Fill)
            r.size = fillSize;

        if (e.size.type == LayoutValue::LVT_Pixels)
            r.size = e.size.value * pxCoeff;

        if (e.size.type == LayoutValue::LVT_Percent)
            r.size = e.size.value / 100.0f * width * pxCoeff;

        if (e.size.type == LayoutValue::LVT_Ratio)
            r.size = e.size.value * restPx * ratioNorm;

        //        printf("%f %f\n", r.offset, r.size);

        ret.push_back(r);
    }

    return ret;
}
