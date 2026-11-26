#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <QCoreApplication>
#include <QTime>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , positionTimer(new QTimer(this))
    , formTimer(new QTimer(this))
    , positionInterval(300)
    , formInterval(2000)
    , step(5)
    , direction(1)
    , formIndex(0)
{
    ui->setupUi(this);

    logFile.setFileName(QCoreApplication::applicationDirPath() + "/log.txt");
    bool logOpened = logFile.open(QIODevice::WriteOnly | QIODevice::Text);

    readSettings();
    if (!logOpened) writeLog("Не вдалося відкрити log.txt для запису");
    applyForm();

    connect(positionTimer, &QTimer::timeout, this, &MainWindow::changePosition);
    connect(formTimer, &QTimer::timeout, this, &MainWindow::changeForm);
    connect(ui->startButton, &QPushButton::clicked, this, &MainWindow::startTimers);
    connect(ui->stopButton, &QPushButton::clicked, this, &MainWindow::stopTimers);
}

MainWindow::~MainWindow()
{
    logFile.close();
    delete ui;
}

void MainWindow::readSettings()
{
    QFile file(QCoreApplication::applicationDirPath() + "/input.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
        writeLog("Файл input.txt не знайдено, взято значення за замовчуванням");
        return;
    }

    QTextStream in(&file);
    while (!in.atEnd()) {
        QStringList parts = in.readLine().split('=');
        if (parts.size() != 2) continue;

        QString key = parts[0].trimmed();
        int value = parts[1].trimmed().toInt();

        if (key == "positionInterval") positionInterval = value;
        else if (key == "formInterval") formInterval = value;
        else if (key == "step") step = value;
    }
    file.close();

    writeLog(QString("Прочитано з input.txt: positionInterval=%1, formInterval=%2, step=%3")
                 .arg(positionInterval).arg(formInterval).arg(step));
}

void MainWindow::writeLog(const QString &text)
{
    QString line = QTime::currentTime().toString("hh:mm:ss") + "  " + text;

    ui->logList->addItem(line);
    ui->logList->scrollToBottom();

    if (logFile.isOpen()) {
        QTextStream out(&logFile);
        out << line << "\n";
        out.flush();
    }
}

void MainWindow::changePosition()
{
    int value = ui->scrollBar->value() + step * direction;

    if (value >= ui->scrollBar->maximum()) {
        value = ui->scrollBar->maximum();
        direction = -1;
    } else if (value <= ui->scrollBar->minimum()) {
        value = ui->scrollBar->minimum();
        direction = 1;
    }

    ui->scrollBar->setValue(value);
    ui->valueLabel->setText(QString("Положення повзунка: %1").arg(value));
}

void MainWindow::changeForm()
{
    formIndex = (formIndex + 1) % 4;
    QString text = applyForm();

    writeLog(QString("Форма %1: %2, положення повзунка %3")
                 .arg(formIndex + 1).arg(text).arg(ui->scrollBar->value()));
}

QString MainWindow::applyForm()
{
    QString text;

    if (formIndex == 0) {
        ui->scrollBar->setOrientation(Qt::Horizontal);
        ui->scrollBar->setGeometry(30, 60, 400, 24);
        ui->scrollBar->setStyleSheet("QScrollBar { background: #e0e0e0; }"
                                     "QScrollBar::handle { background: #2e7d32; min-width: 30px; }");
        text = "горизонтальна, 400x24, колір зелений";
    } else if (formIndex == 1) {
        ui->scrollBar->setOrientation(Qt::Vertical);
        ui->scrollBar->setGeometry(220, 30, 30, 220);
        ui->scrollBar->setStyleSheet("QScrollBar { background: #fff3e0; }"
                                     "QScrollBar::handle { background: #ef6c00; min-height: 30px; }");
        text = "вертикальна, 30x220, колір оранжевий";
    } else if (formIndex == 2) {
        ui->scrollBar->setOrientation(Qt::Horizontal);
        ui->scrollBar->setGeometry(30, 120, 440, 44);
        ui->scrollBar->setStyleSheet("QScrollBar { background: #e3f2fd; }"
                                     "QScrollBar::handle { background: #1565c0; min-width: 40px; }");
        text = "горизонтальна, 440x44, колір синій";
    } else {
        ui->scrollBar->setOrientation(Qt::Vertical);
        ui->scrollBar->setGeometry(400, 30, 44, 160);
        ui->scrollBar->setStyleSheet("QScrollBar { background: #fce4ec; }"
                                     "QScrollBar::handle { background: #c62828; min-height: 30px; }");
        text = "вертикальна, 44x160, колір червоний";
    }

    return text;
}

void MainWindow::startTimers()
{
    positionTimer->start(positionInterval);
    formTimer->start(formInterval);
    writeLog("Таймери запущено");
}

void MainWindow::stopTimers()
{
    positionTimer->stop();
    formTimer->stop();
    writeLog(QString("Таймери зупинено, положення повзунка %1").arg(ui->scrollBar->value()));
}
