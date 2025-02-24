#include "MainWindow.h"

#include <QFile>
#include <QAction>
#include <QMenu>
#include <QFileDialog>
#include <QMessageBox>
#include <QTemporaryFile>
#include <QSettings>
#include <QApplication>

MainWindow::MainWindow(bool singleton, QWidget* parent) noexcept:
    QMainWindow{parent},
    m_url{new UrlDialog{this}},
    m_menus{new QMenuBar},
    m_tools{new QToolBar},
    m_view{new VideoView},
    m_control{new ControlWidget},
    m_main{new QWidget},
    m_viewLayout{new QVBoxLayout},
    m_mainLayout{new QVBoxLayout}
{
    setMenuBar(m_menus);
    addToolBar(m_tools);
    m_tools->setMovable(false);

    m_viewLayout->addSpacerItem(new QSpacerItem(0, 0, QSizePolicy::Expanding, QSizePolicy::Expanding));
    m_view->setLayout(m_viewLayout);
    m_mainLayout->addWidget(m_view);
    m_mainLayout->addWidget(m_control);
    m_mainLayout->setContentsMargins(0, 0, 0, 0);
    m_main->setLayout(m_mainLayout);
    setCentralWidget(m_main);
    
    initMenuBar();
    installEventFilter(this);

    if (singleton)
    {
        initSingleton();
    }

    connect(m_view, &VideoView::empty, this, &MainWindow::openFile);
    connect(m_view, &VideoView::playingChanged, m_control, &ControlWidget::setPlaying);
    connect(m_view, &VideoView::durationChanged, m_control, &ControlWidget::setDuration);
    connect(m_view, &VideoView::positionChanged, m_control, &ControlWidget::setProgress);
    connect(m_view, &VideoView::volumeChanged, m_control, &ControlWidget::setVolume);
    connect(m_view, &VideoView::errorOccurred, this, &MainWindow::showError);

    connect(this, &MainWindow::fileChanged, m_view, &VideoView::setFile);
    connect(this, &MainWindow::dataChanged, m_view, &VideoView::setData);
    connect(m_url, &UrlDialog::urlConfirmed, m_view, &VideoView::setLink);
    connect(m_control, &ControlWidget::progressChanged, m_view, &VideoView::setPosition);
    connect(m_control, &ControlWidget::openClicked, this, &MainWindow::openFile);
    connect(m_control, &ControlWidget::fullClicked, this, &MainWindow::toggleFullScreen);
    connect(m_control, &ControlWidget::backwardClicked, m_view, &VideoView::backward);
    connect(m_control, &ControlWidget::forwardClicked, m_view, &VideoView::forward);
    connect(m_control, &ControlWidget::playClicked, m_view, &VideoView::toggle);
    connect(m_control, &ControlWidget::volumeChanged, m_view, &VideoView::setVolume);

#ifdef Q_OS_WASM
    setFullScreen(true);
    m_view->setLink("https://interactive-examples.mdn.mozilla.net/media/cc0-videos/flower.webm");
#endif

    QFile qss{":/QtTheme/theme/Flat/Dark/Indigo/Teal.qss"};
    qss.open(QFile::ReadOnly);
    setStyleSheet(qss.readAll());
}

MainWindow::~MainWindow() noexcept
{
    Channel::instance().remove();
}

bool MainWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (!isFullScreen() || event->type() != QEvent::HoverMove)
        return false;

    QHoverEvent* hover = dynamic_cast<QHoverEvent*>(event);

#if QT_VERSION < QT_VERSION_CHECK(6, 0, 0)
    QPoint pos = hover->pos();
#else
    QPoint pos = hover->position().toPoint();
#endif
    if (pos.y() > size().height() * 0.75)
    {
        m_control->show();
    }
    else
    {
        m_control->hide();
    }
    return false;
}

void MainWindow::open(const QString& url) noexcept
{
    if (QFileInfo::exists(url))
        m_view->setFile(url);
    else
        m_view->setLink(url);
    m_view->play();
}

void MainWindow::openFile() noexcept
{
    auto file = QFileDialog::getOpenFileName(this);
    if (!file.isEmpty())
    {
        emit fileChanged(file);
    }
}

void MainWindow::openLink() noexcept
{
    m_url->show();
}


void MainWindow::openChannel(const QString& file) noexcept
{
    QFile channel{file};
    channel.open(QFile::ReadOnly);
    QString url = channel.readAll();
    open(url);
}

void MainWindow::setFullScreen(bool v) noexcept
{
    if (v == isFullScreen())
        return;

    if (v)
    {
        ControlWidget* control = dynamic_cast<ControlWidget*>(m_mainLayout->takeAt(1)->widget());
        m_viewLayout->addWidget(control);
    #ifndef Q_OS_WASM
        m_menus->hide();
        m_tools->hide();
    #endif
        setWindowState(Qt::WindowFullScreen);
    }
    else
    {
        ControlWidget* control = dynamic_cast<ControlWidget*>(m_viewLayout->takeAt(1)->widget());
        m_mainLayout->addWidget(control);
    #ifndef Q_OS_WASM
        m_menus->show();
        m_tools->show();
    #endif
        showNormal();
    }
    m_control->setFullScreen(v);
}

void MainWindow::toggleFullScreen() noexcept
{
    if (isFullScreen())
    {
        setFullScreen(false);
    }
    else
    {
        setFullScreen(true);
    }
}


void MainWindow::showError(const QString& error) noexcept
{
    QMessageBox::critical(this, "Error", error);
}

bool MainWindow::regApp() noexcept
{
    QSettings regLolipop("HKEY_LOCAL_MACHINE\\Software\\Microsoft\\Windows\\CurrentVersion\\App Paths\\lolipop.exe", QSettings::NativeFormat);
    if (!regLolipop.isWritable())
    {
        return false;
    }

    regLolipop.setValue("Default", QDir::toNativeSeparators(qApp->applicationFilePath()));
    regLolipop.setValue("Path", QDir::toNativeSeparators(qApp->applicationDirPath()));
    return true;
}

bool MainWindow::regFileTypes() noexcept
{
    QSettings regLolipop("HKEY_CLASSES_ROOT\\Applications\\lolipop.exe", QSettings::NativeFormat);
    QSettings regLolipopOpen("HKEY_CLASSES_ROOT\\Applications\\lolipop.exe\\shell\\open", QSettings::NativeFormat);
    QSettings regLolipopCmd("HKEY_CLASSES_ROOT\\Applications\\lolipop.exe\\shell\\open\\command", QSettings::NativeFormat);
    QSettings regLolipopTypes("HKEY_CLASSES_ROOT\\Applications\\lolipop.exe\\SupportedTypes", QSettings::NativeFormat);

    if (!regLolipop.isWritable())
    {
        return false;
    }

    regLolipop.setValue("FriendlyAppName", "Lolipop Media Player");
    regLolipopOpen.setValue("Default", "Open by Lolipop Media Player");
    regLolipopCmd.setValue("Default", "\"" + QDir::toNativeSeparators(qApp->applicationFilePath()) + "\" \"%1\"");

    QStringList exts = {".mp3", ".mp4"};
    for (auto& ext : exts)
    {
        regLolipopTypes.setValue(ext, "");
    }

    return true;
}


void MainWindow::reg() noexcept
{
#ifdef Q_OS_WIN
    if (regApp() && regFileTypes())
    {
        QMessageBox::information(this, tr("Info"), tr("Success."));
    }
    else
    {
        QMessageBox::critical(this, "Error", "No permission, please run as administrator.");
    }
#else
    QMessageBox::information(this, tr("Info"), tr("Only for Windows."));
#endif
}


void MainWindow::initMenuBar() noexcept
{
    {
        auto menu = new QMenu{tr("File"), this};
        m_menus->addMenu(menu);

        auto file = menu->addAction(QIcon{":/icon/open.svg"}, tr("Open Local File"));
        connect(file, &QAction::triggered, this, &MainWindow::openFile);

        auto link = menu->addAction(QIcon{":/icon/link.svg"}, tr("Open &Remote Link"));
        connect(link, &QAction::triggered, this, &MainWindow::openLink);

        menu->addSeparator();

        auto exit = menu->addAction(QIcon{":/icon/exit.svg"}, tr("Exit"));
        connect(exit, &QAction::triggered, this, &MainWindow::close);

        m_tools->addActions({file, link});
        m_tools->addSeparator();
    }

    {
        auto menu = new QMenu{tr("Control"), this};
        m_menus->addMenu(menu);

        auto play = menu->addAction(QIcon{":/icon/play.svg"}, tr("Play"));
        connect(play, &QAction::triggered, m_view, &VideoView::play);

        auto pause = menu->addAction(QIcon{":/icon/pause.svg"}, tr("Pause"));
        connect(pause, &QAction::triggered, m_view, &VideoView::pause);

        auto backward = menu->addAction(QIcon{":/icon/backward.svg"}, tr("Backward"));
        connect(backward, &QAction::triggered, m_view, &VideoView::backward);

        auto forward = menu->addAction(QIcon{":/icon/forward.svg"}, tr("Forward"));
        connect(forward, &QAction::triggered, m_view, &VideoView::forward);

        m_tools->addActions({play, pause, backward, forward});
        m_tools->addSeparator();
    }

    {
        auto menu = new QMenu{tr("View"), this};
        m_menus->addMenu(menu);

        auto fullscreen = menu->addAction(QIcon{":/icon/fullscreen.svg"}, tr("Full Screen"));
        connect(fullscreen, &QAction::triggered, [this](){setFullScreen(true);});

        auto original = menu->addAction(QIcon{":/icon/original.svg"}, tr("Original"));
        connect(original, &QAction::triggered, [this](){setFullScreen(false);});
        menu->addSeparator();

        auto toggleTools = m_tools->toggleViewAction();
        toggleTools->setText(tr("Tool Bar"));
        menu->addAction(toggleTools);

        m_tools->addActions({fullscreen, original});
    }

    {
        auto menu = new QMenu{tr("Help"), this};
        m_menus->addMenu(menu);

        auto reg = menu->addAction(tr("Register"));
        connect(reg, &QAction::triggered, this, &MainWindow::reg);

        auto about = menu->addAction(tr("About"));
    }
}


void MainWindow::initSingleton() noexcept
{
    Channel::instance().watch();
    connect(&Channel::instance(), &Channel::fileChanged, this, &MainWindow::openChannel);
}