#include "ControlWidget.h"

#include <QStyleOption>
#include <QPainter>

ControlWidget::ControlWidget(QWidget* parent) noexcept:
    QWidget{parent},
    m_layout{new QGridLayout},
    m_left{new QHBoxLayout},
    m_right{new QHBoxLayout},
    m_progress{new ProgressSlider{}},
    m_open{new QPushButton{QIcon{":/icon/open.svg"}, ""}},
    m_fullScreen{new QPushButton{QIcon{":/icon/fullscreen.svg"}, ""}},
    m_backward{new QPushButton{QIcon{":/icon/backward.svg"}, ""}},
    m_play{new QPushButton{QIcon{":/icon/play.svg"}, ""}},
    m_forward{new QPushButton{QIcon{":/icon/forward.svg"}, ""}},
    m_volume{new ProgressSlider{}},
    m_time{new QLabel{"--:--/--:--"}}
{
    m_volume->setRange(0, 100);
    m_volume->setValue(100);

    m_left->addWidget(m_open);
    m_left->addWidget(m_fullScreen);
    m_left->addSpacerItem(new QSpacerItem{0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred});

    m_right->addSpacerItem(new QSpacerItem{0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred});
    m_right->addWidget(m_volume);
    m_right->addSpacerItem(new QSpacerItem{0, 0, QSizePolicy::Expanding, QSizePolicy::Preferred});
    m_right->addWidget(m_time);

    m_layout->addWidget(m_progress, 0, 0, 1, -1);
    m_layout->addLayout(m_left, 1, 0);
    m_layout->addWidget(m_backward, 1, 2);
    m_layout->addWidget(m_play, 1, 3);
    m_layout->addWidget(m_forward, 1, 4);
    m_layout->addLayout(m_right, 1, 5);
    setLayout(m_layout);
    
    m_progress->setProperty("Color", "Success");
    m_open->setProperty("Color", "Primary");
    m_fullScreen->setProperty("Color", "Secondary");
    m_backward->setProperty("Color", "Secondary");
    m_play->setProperty("Color", "Primary");
    m_forward->setProperty("Color", "Secondary");
    m_volume->setProperty("Color", "Secondary");
    m_time->setProperty("Color", "Secondary");
    setStyleSheet("ControlWidget{background:rgba(0,0,0,0.4);}");

    connect(m_progress, &QSlider::valueChanged, this, &ControlWidget::progressChanged);
    connect(m_open, &QPushButton::clicked, this, &ControlWidget::openClicked);
    connect(m_fullScreen, &QPushButton::clicked, this, &ControlWidget::fullClicked);
    connect(m_backward, &QPushButton::clicked, this, &ControlWidget::backwardClicked);
    connect(m_play, &QPushButton::clicked, this, &ControlWidget::playClicked);
    connect(m_forward, &QPushButton::clicked, this, &ControlWidget::forwardClicked);
    connect(m_volume, &QSlider::valueChanged, this, &ControlWidget::volumeChanged);
}

ControlWidget::~ControlWidget() noexcept
{

}

void ControlWidget::paintEvent(QPaintEvent*)
{
    QStyleOption opt;
    opt.initFrom(this);
    QPainter p(this);
    style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

void ControlWidget::setPlaying(bool v) noexcept
{
    if (v)
    {
        m_play->setIcon(QIcon(":/icon/pause.svg"));
    }
    else
    {
        m_play->setIcon(QIcon(":/icon/play.svg"));
    }
}


void ControlWidget::setFullScreen(bool v) noexcept
{
    if (v)
    {
        m_fullScreen->setIcon(QIcon(":/icon/original.svg"));
    }
    else
    {
        m_fullScreen->setIcon(QIcon(":/icon/fullscreen.svg"));
    }
}

void ControlWidget::setDuration(int max) noexcept
{
    auto v = m_progress->blockSignals(true);
    m_progress->setRange(0, max);
    m_progress->blockSignals(v);
    formatTime();
}

void ControlWidget::setProgress(int n) noexcept
{
    auto v = m_progress->blockSignals(true);
    m_progress->setValue(n);
    m_progress->blockSignals(v);
    formatTime();
}

void ControlWidget::setVolume(int n) noexcept
{
    auto v = m_volume->blockSignals(true);
    m_volume->setValue(n);
    m_volume->blockSignals(v);
}

void ControlWidget::formatTime() noexcept
{
    int pos = m_progress->value();
    int pmin = pos / 1000 / 60;
    int psec = pos / 1000 - pmin * 60;

    int max = m_progress->maximum();
    int mmin = max / 1000 / 60;
    int msec = max / 1000 - mmin * 60;

    m_time->setText(QString::asprintf("%02d:%02d/%02d:%02d", pmin, psec, mmin, msec));
}