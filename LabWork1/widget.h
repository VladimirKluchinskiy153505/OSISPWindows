#ifndef WIDGET_H
#define WIDGET_H
#include <QApplication>
#include <QWidget>
#include <QLineEdit>
#include <QPainter>
#include <QRect>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent = nullptr);
    ~Widget();
protected:
    void paintEvent(QPaintEvent *event) override;
private slots:
    //void onDrawButtonClick();
    void on_convertButton_clicked();

private:
    Ui::Widget *ui;
    double fontSizeIncrement = 0.4;
    double fontSize;
    QLineEdit *textEdit;
    double angleIncrement = 0.02; // Угол между символами в радианах
    double angle;
};
#endif // WIDGET_H
