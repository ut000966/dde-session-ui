#include "userframe.h"

#include "userbutton.h"
#include "sessionbasemodel.h"
#include "userinfo.h"

#define USER_WIDGET_HEIGHT 360
#define USER_HEIGHT 180

UserFrame::UserFrame(QWidget *parent)
    : QFrame(parent)
    , m_isExpansion(false)
{
    setFixedHeight(500);
}

void UserFrame::setModel(SessionBaseModel *model)
{
    m_model = model;

    connect(model, &SessionBaseModel::onUserAdded, this, &UserFrame::userAdded);
    connect(model, &SessionBaseModel::onUserRemoved, this, &UserFrame::userRemoved);

    QList<std::shared_ptr<User>> userList = m_model->userList();

    for (auto user : userList) {
        userAdded(user);
    }
}

void UserFrame::resizeEvent(QResizeEvent *event)
{
    QTimer::singleShot(0, this, &UserFrame::refreshPosition);

    return QFrame::resizeEvent(event);
}

void UserFrame::mouseReleaseEvent(QMouseEvent *event)
{
    emit hideFrame();

    expansion(false);

    hide();

    return QFrame::mouseReleaseEvent(event);
}

void UserFrame::hideEvent(QHideEvent *event)
{
    releaseKeyboard();

    return QFrame::hideEvent(event);
}

void UserFrame::userAdded(std::shared_ptr<User> user)
{
    UserButton *button = new UserButton(user, this);
    m_userBtns[button] = user->uid();
    button->setLoginIconVisible(true);

    connect(button, &UserButton::clicked, this, &UserFrame::onUserClicked);

    refreshPosition();
}

void UserFrame::userRemoved(const uint uid)
{
    UserButton * button = m_userBtns.key(uid);
    m_userBtns.remove(button);
    button->deleteLater();

    refreshPosition();
}

void UserFrame::expansion(bool expansion)
{
    m_isExpansion = expansion;

    refreshPosition();
}

void UserFrame::refreshPosition()
{
    if (m_isExpansion) {
        const int cout = m_userBtns.size();
        const int maxLineCap = width() / USER_ICON_WIDTH - 1; // 1 for left-offset and right-offset.
        const int offset = (width() - USER_ICON_WIDTH * std::min(cout, maxLineCap)) / 2;

        int row = 0;
        int index = 0;
        std::shared_ptr<User> currentUser = m_model->currentUser();
        for (auto it = m_userBtns.constBegin(); it != m_userBtns.constEnd(); ++it) {
            UserButton *button = it.key();
            button->setSelected(it.value() == currentUser->uid());
            button->stopAnimation();
            button->show();
            button->setImageSize(UserButton::AvatarSmallSize);
            // If the current value is the maximum, need to change the line.
            if (index >= maxLineCap) {
                index = 0;
                ++row;
            }

            button->move(QPoint(offset + index * USER_ICON_WIDTH, USER_HEIGHT * row));
            index++;
        }

        setFocus();
        QTimer::singleShot(100, this, &UserFrame::grabKeyboard);
    }
}

void UserFrame::onUserClicked()
{
    UserButton *button = static_cast<UserButton*>(sender());

    releaseKeyboard();

    expansion(false);

    emit requestSwitchUser(m_model->findUserByUid(m_userBtns[button]));
    emit hideFrame();
}
