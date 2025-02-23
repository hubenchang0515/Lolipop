#ifndef CONTROL_WIDGET_H
#define CONTROL_WIDGET_H

#include <QWidget>
#include <QPushButton>
#include <QLabel>
#include <QLayout>
#include <QPaintEvent>

#include "ProgressSlider.h"

class ControlWidget : public QWidget
{
    Q_OBJECT
public:
    ControlWidget(QWidget* parent=nullptr) noexcept;
    ~ControlWidget() noexcept;

    void setPlaying(bool v) noexcept;
    void setFullScreen(bool v) noexcept;
    void setDuration(int max) noexcept;
    void setProgress(int n) noexcept;
    void setVolume(int n) noexcept;

signals:
    void playingChanged(bool);
    void progressChanged(int);
    void openClicked();
    void fullClicked();
    void backwardClicked();
    void playClicked();
    void forwardClicked();
    void volumeChanged(int);

protected:
    virtual void paintEvent(QPaintEvent*) override;

private:
    QGridLayout* m_layout;
    QHBoxLayout* m_left;
    QHBoxLayout* m_right;
    ProgressSlider* m_progress;
    QPushButton* m_open;
    QPushButton* m_fullScreen;
    QPushButton* m_backward;
    QPushButton* m_play;
    QPushButton* m_forward;
    ProgressSlider* m_volume;
    QLabel* m_time;

    void formatTime() noexcept;
};

#endif