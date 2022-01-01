#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_pushButton_clicked();

    void on_pushButton_2_clicked();

    void on_angle200_clicked();

    void on_pushButton_4_clicked();

    void on_angle400_clicked();

    void on_angle500_clicked();

    void on_angle600_clicked();

    void on_angle700_clicked();

    void on_angle800_clicked();

    void on_angle900_clicked();

    void on_angle1000_clicked();

    void on_angle1100_clicked();

    void on_angle1200_clicked();

    void on_angle1300_clicked();

    void on_angle1400_clicked();

    void on_angle1500_clicked();

    void on_angle1600_clicked();

    void on_angle1700_clicked();

    void on_angle1800_clicked();

    void on_angle1900_clicked();

    void on_angle2000_clicked();

    void on_angle2100_clicked();

    void on_angle2200_clicked();

    void on_pwm2_clicked();

    void on_pushButton_3_clicked();

    void on_pwm4_clicked();

    void on_pushButton_5_clicked();

    void on_ultraSonic_clicked();

    void on_measure_clicked();

    void on_smartFactoryStart_clicked();

    void on_grabFromWeightSensor_clicked();

    void on_pushButton_7_clicked();

    void on_moveToB_clicked();

    void on_moveToC_clicked();

private:
    Ui::MainWindow *ui;
};

#endif // MAINWINDOW_H
