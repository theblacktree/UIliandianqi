#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QTimer>
#include <QMouseEvent>
#include <QKeyEvent>
#include <QDebug>
#include <windows.h>
#include <winuser.h>
#include <QRandomGenerator>
#include <QDateTime>

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
    //点击间隔时间ms
    void Delay_MSec(int msec);

    //点击函数
    void my_mouse_press(int msec);

    //点击按压时长ms
    void press_delay();

    void just_Wait(int, int);
    //控制是否可写
    void setMyDisabled(bool);
private slots:

    void on_pushButton_start_clicked();

private:
    Ui::MainWindow *ui;
    QPoint point;//鼠标光标目前绝对位置
    bool firstFlag = true;
    int randomIntRange_x;//附加的随机位置
    int randomIntRange_y;

    int addRandomIntervalTime;
    int addRandomPressTimeLow;
    int addRandomPressTimeHigh;
    int RandomMoveRange;
    QTime startTime;
    QTime endTime;
    int epochTimeSpan;
    long totalTime;

    volatile bool start=1;//线程标志位
    void closeEvent(QCloseEvent *event);
    void keyReleaseEvent(QKeyEvent* event);
};
#endif // MAINWINDOW_H
