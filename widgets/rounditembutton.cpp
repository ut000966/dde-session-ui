#include <QtCore/QObject>

#include "rounditembutton.h"

RoundItemButton::RoundItemButton(const QString &text, QWidget* parent)
    : QAbstractButton(parent),
      m_itemIcon(new QLabel(this)),
      m_itemText(new QLabel(this))
{
    m_itemText->setText(text);

    initUI();
    initConnect();
}

RoundItemButton::~RoundItemButton()
{
}

void RoundItemButton::setChecked(bool checked)
{
    if (checked)
        updateState(Checked);
    else
        updateState(Normal);
}

void RoundItemButton::initConnect()
{
    connect(this, &RoundItemButton::stateChanged, this, &RoundItemButton::setState, Qt::DirectConnection);
    connect(this, &RoundItemButton::stateChanged, this, &RoundItemButton::updateIcon);
    connect(this, &RoundItemButton::stateChanged, this, static_cast<void (RoundItemButton::*)()>(&RoundItemButton::update));
    connect(this, &RoundItemButton::iconChanged, this, &RoundItemButton::updateIcon);
    connect(this, &RoundItemButton::toggled, this, &RoundItemButton::setChecked);
    connect(signalManager, &SignalManager::setButtonHover, this, &RoundItemButton::setUnhovered);
}

void RoundItemButton::initUI() {
    m_itemIcon->setFocusPolicy(Qt::NoFocus);

    m_itemText->setStyleSheet("color: rgba(255, 255, 255, 255); "
                              "font-size:16px; text-align:center;");
    m_itemText->setFixedHeight(30);
    m_itemText->setAlignment(Qt::AlignHCenter);

    QVBoxLayout *mainLayout = new QVBoxLayout;
    mainLayout->setMargin(0);
    mainLayout->addSpacing(4);
    mainLayout->addWidget(m_itemIcon);
    mainLayout->setAlignment(m_itemIcon, Qt::AlignHCenter);
    mainLayout->setSpacing(2);
    mainLayout->addWidget(m_itemText);
    mainLayout->addStretch(0);

    setFocusPolicy(Qt::NoFocus);
    setFocusPolicy(Qt::StrongFocus);
    setLayout(mainLayout);
    setFixedSize(QSize(120, 120));
    setCheckable(true);

    QGraphicsDropShadowEffect *nameShadow = new QGraphicsDropShadowEffect;
    nameShadow->setBlurRadius(16);
    nameShadow->setColor(QColor(0, 0, 0, 85));
    nameShadow->setOffset(0, 4);
    m_itemText->setGraphicsEffect(nameShadow);
}

void RoundItemButton::enterEvent(QEvent* event)
{
    Q_UNUSED(event)
    if (m_state == Normal)
        updateState(Hover);

    emit signalManager->setButtonHover(m_itemText->text());
}

void RoundItemButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    updateState(Normal);
}

void RoundItemButton::mousePressEvent(QMouseEvent* event) {
    qDebug() << "RoundItemButton pressed";
    Q_UNUSED(event);
    updateState(Pressed);
}

void RoundItemButton::mouseReleaseEvent(QMouseEvent* e)
{
    Q_UNUSED(e);

    if (m_state == Checked)
        updateState(Hover);
    else
        updateState(Pressed);

    emit clicked();
}

void RoundItemButton::paintEvent(QPaintEvent* event)
{
    QWidget::paintEvent(event);

    if (m_state != Checked)
        return;

    QPainter painter(this);
    painter.setPen(QPen(QColor(255, 255, 255, 51), 2));
    painter.setBrush(QColor(0, 0 , 0, 76));
    painter.setRenderHint(QPainter::Antialiasing, true);
    painter.drawRoundedRect(QRect(2, 2, 116, 116), 10, 10, Qt::RelativeSize);
}

void RoundItemButton::updateIcon()
{
    QPixmap pixmap;
    switch (m_state)
    {
    case Normal:    pixmap.load(m_normalIcon);      break;
    case Hover:     pixmap.load(m_hoverIcon);       break;
    case Checked:   pixmap.load(m_normalIcon);      break;
    case Pressed:   pixmap.load(m_pressedIcon);     break;
    }

    if (!pixmap.isNull()) {
        m_itemIcon->setPixmap(pixmap);
    }
}

void RoundItemButton::updateState(const RoundItemButton::State state) {

    if (m_state != state) {
        m_state = state;
        emit stateChanged(state);
    }

    QAbstractButton::setChecked(m_state == Checked);
    return updateIcon();
}

void RoundItemButton::setUnhovered(QString text) {
    if (m_itemText->text()!=text) {
        updateState(Normal);
    }
}