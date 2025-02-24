#include "Channel.h"

#include <QDir>
#include <QDirIterator>

Channel& Channel::instance() noexcept
{
    static Channel channel;
    return channel;
}

Channel::Channel(QObject* parent) noexcept:
    m_file{new QFile{QDir::tempPath() + "/lolipop.channel", this}},
    m_watcher{new QFileSystemWatcher{this}}
{

}

Channel::~Channel() noexcept
{

}

void Channel::watch() noexcept
{
    m_file->open(QFile::ReadWrite);
    m_watcher->addPath(m_file->fileName());
    connect(m_watcher, &QFileSystemWatcher::fileChanged, this, &Channel::fileChanged);
    m_file->close();
}

bool Channel::detect() const noexcept
{
    return m_file->exists();
}

void Channel::write(const QString& text) noexcept
{
    m_file->open(QFile::WriteOnly);
    m_file->write(text.toUtf8());
    m_file->close();
}


void Channel::remove() noexcept
{
    m_file->remove();
}