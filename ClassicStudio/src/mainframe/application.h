#ifndef APPLICATION_H
#define APPLICATION_H

#include <QApplication>

class Application : public QApplication
{
    Q_OBJECT
public:
    Application(int &argc, char **argv, int = ApplicationFlags);

    bool notify(QObject *, QEvent *) override;

signals:
    void ctrlKeyEvent(int);
    void normalKeyEvent(int);
};

#endif // APPLICATION_H
