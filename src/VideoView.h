#ifndef VIDEO_VIEW_H
#define VIDEO_VIEW_H

#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsVideoItem>
#include <QMediaPlayer>
#include <QAudioOutput>
#include <QResizeEvent>
#include <QLayout>
#include <QBuffer>

class VideoView : public QGraphicsView
{
    Q_OBJECT
public:
    VideoView(QWidget* parent=nullptr) noexcept;
    ~VideoView() noexcept;

    virtual QSize sizeHint() const override;
    virtual void resizeEvent(QResizeEvent* event) override;

    qint64 position() const noexcept;
    qint64 duration() const noexcept;

    void play() noexcept;
    void pause() noexcept;
    void toggle() noexcept;
    void setSource(const QString& src) noexcept;
    void setData(const QByteArray& data) noexcept;
    void setPosition(qint64 n) noexcept;
    void setVolume(int volume) noexcept;

    void backward() noexcept;
    void forward() noexcept;

signals:
    void empty();
    void errorOccurred(QMediaPlayer::Error);
    void playingChanged(bool);
    void positionChanged(qint64);
    void durationChanged(qint64);
    void volumeChanged(int);

private:
    QGraphicsScene* m_scene;
    QGraphicsVideoItem* m_item;
    QMediaPlayer* m_player;
    QAudioOutput* m_audio;
    QBuffer* m_data;
};

#endif