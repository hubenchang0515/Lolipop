#ifndef CHANNEL_H
#define CHANNEL_H

#include <QObject>
#include <QFileSystemWatcher>
#include <QFile>

class Channel : public QObject
{
    Q_OBJECT
public:
    static Channel& instance() noexcept;

    Channel(QObject* parent=nullptr) noexcept;
    ~Channel() noexcept;

    void watch() noexcept;
    bool detect() const noexcept;
    void write(const QString& text) noexcept;
    void remove() noexcept;

signals:
    void fileChanged(const QString& path);

private:
    QFile* m_file;
    QFileSystemWatcher* m_watcher;
};

#endif