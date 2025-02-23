#ifndef URL_DIALOG_H
#define URL_DIALOG_H

#include <QDialog>
#include <QLineEdit>
#include <QPushButton>
#include <QLayout>

class UrlDialog : public QDialog
{
    Q_OBJECT
public:
    UrlDialog(QWidget* parent=nullptr) noexcept;
    ~UrlDialog() noexcept;

    void confirmUrl();

signals:
    void urlConfirmed(const QString&);

private:
    QLineEdit* m_url;
    QPushButton* m_ok;
    QPushButton* m_cancel;
    QGridLayout* m_layout;
};

#endif