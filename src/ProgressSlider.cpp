#include "ProgressSlider.h"
#include "ProgressSliderStyle.h"

ProgressSlider::ProgressSlider(QWidget* parent) noexcept:
    QSlider{Qt::Horizontal, parent}
{
    auto style = new ProgressSliderStyle(this->style());
    setStyle(style);
}

ProgressSlider::~ProgressSlider() noexcept
{

}
