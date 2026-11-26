#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QFile>
#include <QTextStream>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void changePosition();
    void changeForm();
    void startTimers();
    void stopTimers();

private:
    void readSettings();
    void writeLog(const QString &text);
    QString applyForm();

    Ui::MainWindow *ui;
    QTimer *positionTimer;
    QTimer *formTimer;
    QFile logFile;

    int positionInterval;
    int formInterval;
    int step;
    int direction;
    int formIndex;
};

#endif // MAINWINDOW_H
