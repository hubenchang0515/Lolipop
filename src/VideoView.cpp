#include "VideoView.h"

VideoView::VideoView(QWidget* parent) noexcept:
    QGraphicsView{parent},
    m_scene{new QGraphicsScene{this}},
    m_item{new QGraphicsVideoItem},
    m_player{new QMediaPlayer{this}},
    m_audio{new QAudioOutput},
    m_data{new QBuffer{this}}
{
    m_data->open(QBuffer::ReadWrite);
    m_audio->setParent(this);
    m_scene->addItem(m_item);
    setScene(m_scene);
    m_player->setVideoOutput(m_item);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    connect(m_player, QOverload<QMediaPlayer::Error>::of(&QMediaPlayer::error), [this](){ emit errorOccurred(m_player->errorString()); });
    connect(m_player, &QMediaPlayer::stateChanged, [this](QMediaPlayer::State state){emit playingChanged(state == QMediaPlayer::State::PlayingState);});
    connect(m_player, &QMediaPlayer::volumeChanged, this, &VideoView::volumeChanged);
#else
    m_player->setAudioOutput(m_audio);
    connect(m_player, &QMediaPlayer::errorOccurred, [this](){ emit errorOccurred(m_player->errorString()); });
    connect(m_player, &QMediaPlayer::playbackStateChanged, [this](QMediaPlayer::PlaybackState state){emit playingChanged(state == QMediaPlayer::PlayingState);});
    connect(m_audio, &QAudioOutput::volumeChanged, this, &VideoView::volumeChanged);
#endif

    connect(m_player, &QMediaPlayer::positionChanged, this, &VideoView::positionChanged);
    connect(m_player, &QMediaPlayer::durationChanged, this, &VideoView::durationChanged);
}

VideoView::~VideoView() noexcept
{

}

QSize VideoView::sizeHint() const
{
    return QSize(1280, 720);
}

void VideoView::resizeEvent(QResizeEvent* event)
{
    QGraphicsView::resizeEvent(event);
    m_item->setSize(event->size());
    m_scene->setSceneRect(0, 0, event->size().width(), event->size().height());
}

qint64 VideoView::position() const noexcept
{
    return m_player->position();
}

qint64 VideoView::duration() const noexcept
{
    return m_player->duration();
}

void VideoView::play() noexcept
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    if (m_player->media().isNull())
#else
    if (m_player->source().isEmpty())
#endif
    {
        emit empty();
    }

    m_player->play();
}

void VideoView::pause() noexcept
{
    m_player->pause();
}


void VideoView::toggle() noexcept
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    bool playing = m_player->state() == QMediaPlayer::State::PlayingState;
#else
    bool playing = m_player->playbackState() == QMediaPlayer::PlayingState;
#endif

    if (playing)
        pause();
    else
        play();
}

void VideoView::setFile(const QString& src) noexcept
{
    QUrl url = QUrl::fromLocalFile(src);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_player->setMedia(url);
#else
    m_player->setSource(url);
#endif
    m_player->play();
}


void VideoView::setLink(const QString& src) noexcept
{
    QUrl url{src};
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_player->setMedia(url);
#else
    m_player->setSource(url);
#endif
    m_player->play(); 
}


void VideoView::setData(const QByteArray& data) noexcept
{
    m_data->reset();
    m_data->write(data);
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_player->setMedia(QUrl{}, m_data);
#else
    m_player->setSourceDevice(m_data);
#endif
    m_player->play();
}

void VideoView::setPosition(qint64 n) noexcept
{
    if (n < 0)
        m_player->setPosition(0);
    else if (n > m_player->duration())
        m_player->setPosition(m_player->duration());
    else
        m_player->setPosition(n);
}


void VideoView::setVolume(int volume) noexcept
{
#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    m_player->setVolume(volume);
#else
    m_audio->setVolume(volume);
#endif
}

void VideoView::backward() noexcept
{
    m_player->setPosition(m_player->position() - 10000);
}

void VideoView::forward() noexcept
{
    m_player->setPosition(m_player->position() + 10000);
}