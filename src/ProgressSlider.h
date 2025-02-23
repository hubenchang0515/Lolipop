#ifndef PROGRESS_SLIDER_H
#define PROGRESS_SLIDER_H

#include <QSlider>

class ProgressSlider : public QSlider
{
    Q_OBJECT
public:
    ProgressSlider(QWidget* parent=nullptr) noexcept;
    ~ProgressSlider() noexcept;
};

#endif