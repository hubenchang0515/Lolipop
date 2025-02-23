#include "ProgressSliderStyle.h"

ProgressSliderStyle::ProgressSliderStyle(QStyle* style) noexcept:
    QProxyStyle{style}
{

}

ProgressSliderStyle::~ProgressSliderStyle()
{

}

int ProgressSliderStyle::styleHint(QStyle::StyleHint hint, const QStyleOption *option, const QWidget *widget , QStyleHintReturn *returnData) const
{
    if (hint == QStyle::SH_Slider_AbsoluteSetButtons)
    {
        return Qt::LeftButton;
    }

    return QProxyStyle::styleHint(hint, option, widget, returnData);
}