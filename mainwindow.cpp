#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    ui->plainTextEdit_tip->setEnabled(false);
    ui->plainTextEdit->setEnabled(false);
    point.setX(0),point.setY(0);
    randomIntRange_x=0;
    randomIntRange_y=0;
    addRandomIntervalTime = 0;
    addRandomPressTimeLow = 0;
    addRandomPressTimeHigh=0;
    RandomMoveRange = 0;

    epochTimeSpan = 0;
    totalTime = 0;
    this->resize(800,580);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    start = 0;//退出线程
}

void MainWindow::keyReleaseEvent(QKeyEvent* event)
{
    if (event->key() == Qt::Key_F6)
    {
        ui->plainTextEdit_tip->clear();
        start = 0;//退出线程
    }
}
void MainWindow::Delay_MSec(int msec)
{

    QEventLoop loop;//定义一个新的事件循环
    int randomInt = QRandomGenerator::global()->bounded(0, addRandomIntervalTime);
    QTimer::singleShot(msec+randomInt, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数

    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void MainWindow::press_delay()
{
    QEventLoop loop;//定义一个新的事件循环
    int randomInt = QRandomGenerator::global()->bounded(addRandomPressTimeLow,addRandomPressTimeHigh);
    QTimer::singleShot(randomInt, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数

    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}

void MainWindow::just_Wait(int low,int high)
{
    QEventLoop loop;//定义一个新的事件循环
    int randomInt = QRandomGenerator::global()->bounded(low, high);
    QTimer::singleShot(randomInt, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数

    loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
}
void MainWindow::my_mouse_press(int msec)
{
    Delay_MSec(msec);//已进行加随机数处理
    //设置点击随机短距离移动
    randomIntRange_x = QRandomGenerator::global()->bounded(-RandomMoveRange,RandomMoveRange);
    randomIntRange_y = QRandomGenerator::global()->bounded(-RandomMoveRange,RandomMoveRange);
    mouse_event(MOUSEEVENTF_MOVE, randomIntRange_x,randomIntRange_y,0,0);
    //点击鼠标左键
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTDOWN, point.x()+randomIntRange_x,point.y()+randomIntRange_y,0,0);
    press_delay();
    mouse_event(MOUSEEVENTF_ABSOLUTE|MOUSEEVENTF_LEFTUP, point.x()+randomIntRange_x,point.y()+randomIntRange_y,0,0);
    mouse_event(MOUSEEVENTF_MOVE, -randomIntRange_x,-randomIntRange_y,0,0);

}

void MainWindow::setMyDisabled(bool flag)
{
    ui->lineEdit_1->setReadOnly(flag);
    ui->lineEdit_2->setReadOnly(flag);
    ui->lineEdit_3->setReadOnly(flag);
    ui->lineEdit_circle->setReadOnly(flag);
    ui->lineEdit->setReadOnly(flag);
    ui->lineEdit_4->setReadOnly(flag);
    ui->lineEdit_5->setReadOnly(flag);
    ui->lineEdit_6->setReadOnly(flag);
}

void MainWindow::on_pushButton_start_clicked()
{
    //读取参数
    int timespan1 = int(ui->lineEdit_1->text().toInt());
    int timespan2 = int(ui->lineEdit_2->text().toInt());
    int timespan3 = int(ui->lineEdit_3->text().toInt());
    int timespan_circle = int(ui->lineEdit_circle->text().toInt());
    addRandomIntervalTime = int(ui->lineEdit_5->text().toInt());
    addRandomPressTimeLow = int(ui->lineEdit->text().toInt());
    addRandomPressTimeHigh = int(ui->lineEdit_6->text().toInt());
    RandomMoveRange = int(ui->lineEdit_4->text().toInt());
    point = QCursor().pos();

    //提示区写东西
    if ((timespan1<=0)||(timespan2<=0)||(timespan3<=0)||(timespan_circle<=0)||\
        (addRandomIntervalTime<=0)||(addRandomPressTimeLow<=0)||(addRandomPressTimeHigh<=0)||(RandomMoveRange<=0))
    {
        ui->plainTextEdit_tip->setEnabled(true);
        ui->plainTextEdit_tip->appendPlainText("请输入正整数值！");
        ui->plainTextEdit_tip->setEnabled(false);
        setMyDisabled(false);
        return ;
    }
    else if (addRandomPressTimeLow >= addRandomPressTimeHigh)
    {
        ui->plainTextEdit_tip->setEnabled(true);
        ui->plainTextEdit_tip->appendPlainText("请检查鼠标按压时长范围是否正确！");
        ui->plainTextEdit_tip->setEnabled(false);
        setMyDisabled(false);
        return ;
    }
    else if (RandomMoveRange>10)
    {
        ui->plainTextEdit_tip->setEnabled(true);
        ui->plainTextEdit_tip->appendPlainText("警告！随机点跨越太大可能导致位移偏差！");
        ui->plainTextEdit_tip->setEnabled(false);
    }
    else if (timespan2<=4000)
    {
        ui->plainTextEdit_tip->setEnabled(true);
        ui->plainTextEdit_tip->appendPlainText("点击间隔过低，请慎重！");
        ui->plainTextEdit_tip->setEnabled(false);
    }
    else
    {

    }
    start = 1;//开始
    //设置不可写
    setMyDisabled(true);
    //等待5秒，等待用户将鼠标移动到指定位置
    if (firstFlag)
    {
        ui->plainTextEdit_tip->setEnabled(true);
        ui->plainTextEdit_tip->appendPlainText("点击开始后您有5秒+第一次间隔的时间将您的鼠标移动到指定位置！");
        ui->plainTextEdit_tip->setEnabled(false);
        QEventLoop loop;
        QTimer::singleShot(5000, &loop, SLOT(quit()));//创建单次定时器，槽函数为事件循环的退出函数
        loop.exec();//事件循环开始执行，程序会卡在这里，直到定时时间到，本循环被退出
        firstFlag = false;
    }



//计算总时长
    int oneEpochTime = (timespan1 + timespan2 + timespan3 + (addRandomIntervalTime>>1) + \
                        addRandomPressTimeLow + ((addRandomPressTimeHigh - addRandomPressTimeLow)>>1));
    totalTime = timespan_circle * oneEpochTime;//+ 45000 * (timespan_circle/60);
    int timesec=totalTime/1000;
    int timemin = timesec/60;
    int ptimehour=timemin/60;
    int ptimemin=timemin%60;
    int ptimesec=timesec%60;
    ui->label_15->setText(" 预估结束时间："+QString::number(ptimehour)+" h"+ \
                          QString::number(ptimemin)+" m"+QString::number(ptimesec)+" s");

    //开始循环
    while (timespan_circle>0)
    {
        if (start == 0)
            break;
        //时间更新计算,包含随机延时的30~60s
        startTime = QTime::currentTime();
        /*if (timespan_circle % 60 == 0)
        {
            ui->plainTextEdit_tip->setEnabled(true);
            ui->plainTextEdit_tip->appendPlainText("现在是等待时间，请耐心等待30s~60s！等待结束自动恢复");
            ui->plainTextEdit_tip->setEnabled(false);
            just_Wait(5000,7000);
            ui->plainTextEdit_tip->setEnabled(true);
            ui->plainTextEdit_tip->appendPlainText("等待时间结束,已自动恢复");
            ui->plainTextEdit_tip->setEnabled(false);

        }*/

        my_mouse_press(timespan1);//必须点击战斗
        if (start == 0)
            break;
        my_mouse_press(timespan2);//可以移动位置点击
        if (start == 0)
            break;
        my_mouse_press(timespan3);//可以在游戏界面随便点
        //时间更新计算
        endTime = QTime::currentTime();
        epochTimeSpan = startTime.msecsTo(endTime);        // 结果为ms
        ui->label_3->setText("本次循环所用时间："+QString::number(epochTimeSpan)+"ms");
        totalTime -= epochTimeSpan;
        timesec=totalTime/1000;
        timemin = timesec/60;
        ptimehour=timemin/60;
        ptimemin=timemin%60;
        ptimesec=timesec%60;
        ui->label_15->setText(" 预估结束时间："+QString::number(ptimehour)+" h"+ \
                              QString::number(ptimemin)+" m"+QString::number(ptimesec)+" s");

        if ((--timespan_circle)==0)
        {
            ui->label_15->setText(" 预估结束时间：0 h 0 m 0 s");
        }

        ui->lineEdit_circle->setReadOnly(false);
        ui->lineEdit_circle->setText(QString::number(timespan_circle));
        ui->lineEdit_circle->setReadOnly(true);
    }

    //循环结束
    setMyDisabled(false);
    this->showMaximized(); // Qt最大化显示函数
}


