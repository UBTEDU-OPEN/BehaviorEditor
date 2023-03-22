#include "application.h"

#include <QEvent>
#include <QKeyEvent>

Application::Application(int &argc, char **argv, int flags)
    : QApplication(argc,argv,flags)
{

}

bool Application::notify(QObject *obj, QEvent *e)
{
    if(e->type() == QEvent::KeyRelease) {
        auto keyEvent = static_cast<QKeyEvent*>(e);
        if(keyEvent->modifiers().testFlag(Qt::ControlModifier)) {
            emit ctrlKeyEvent(keyEvent->key());
        } else {
            emit normalKeyEvent(keyEvent->key());
        }
        return true;
    }
    return QApplication::notify(obj, e);
}
