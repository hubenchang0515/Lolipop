#include "UrlDialog.h"

UrlDialog::UrlDialog(QWidget* parent) noexcept:
    QDialog{parent},
    m_url{new QLineEdit},
    m_ok{new QPushButton{tr("OK")}},
    m_cancel{new QPushButton{tr("CANCEL")}},
    m_layout{new QGridLayout}
{
    m_layout->addWidget(m_url, 0, 0, 1, -1);
    m_layout->addWidget(m_ok, 1, 0);
    m_layout->addWidget(m_cancel, 1, 1);
    setLayout(m_layout);

    connect(m_ok, &QPushButton::clicked, this, &UrlDialog::confirmUrl);
    connect(m_cancel, &QPushButton::clicked, this, &QDialog::close);

    setFixedWidth(240);
    m_url->setPlaceholderText("http://example.org/xxx.mp4");
    m_ok->setDefault(true);
    m_ok->setProperty("Color", "Primary");
}

UrlDialog::~UrlDialog() noexcept
{

}

void UrlDialog::confirmUrl()
{
    emit urlConfirmed(m_url->text());
    close();
}