#ifndef MAIN_WINDOW_H
#define MAIN_WINDOW_H

#include <QMainWindow>
#include <QMenuBar>
#include <QToolBar>
#include <QLayout>

#include "VideoView.h"
#include "ControlWidget.h"
#include "UrlDialog.h"

class MainWindow : public QMainWindow
{
    Q_OBJECT
public:
    MainWindow(QWidget* parent=nullptr) noexcept;
    ~MainWindow() noexcept;

    virtual bool eventFilter(QObject *watched, QEvent *event) override;

    void openFile() noexcept;
    void openLink() noexcept;
    void setFullScreen(bool v) noexcept;
    void toggleFullScreen() noexcept;
    void showError(const QString& error) noexcept;

signals:
    void fileChanged(const QString&);
    void linkChanged(const QString&);
    void dataChanged(const QByteArray&);

private:
    UrlDialog* m_url;
    QMenuBar* m_menus;
    QToolBar* m_tools;
    VideoView* m_view;
    ControlWidget* m_control;
    QWidget* m_main;
    QVBoxLayout* m_viewLayout;
    QVBoxLayout* m_mainLayout;


    void initMenuBar() noexcept;
};

#endif