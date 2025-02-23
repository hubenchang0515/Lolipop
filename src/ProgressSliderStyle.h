#ifndef PROGRESS_SLIDER_STYLE_H
#define PROGRESS_SLIDER_STYLE_H

#include <QProxyStyle>

class ProgressSliderStyle : public QProxyStyle
{
public:
    ProgressSliderStyle(QStyle* style=nullptr) noexcept;
    ~ProgressSliderStyle();

    virtual int styleHint(QStyle::StyleHint hint, const QStyleOption *option = nullptr, const QWidget *widget = nullptr, QStyleHintReturn *returnData = nullptr) const override;
};

#endif