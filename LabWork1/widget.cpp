#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Widget), angle(0)
{
    ui->setupUi(this);
    //QLabel *titleLabel = new QLabel("Введите текст и нажмите кнопку для отображения:");
   // QPushButton *drawButton = new QPushButton("Отобразить");
    //connect(drawButton, &QPushButton::clicked, this, &Widget::onDrawButtonClick);
     textEdit = ui->lineInput;
    //QVBoxLayout *layout = new QVBoxLayout;
    //layout->addWidget(titleLabel);
    //layout->addWidget(textEdit);
    //layout->addWidget(drawButton);
    //setLayout(layout);
    //setLayout(ui->mainLayout);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::paintEvent(QPaintEvent *event)
{
    QWidget::paintEvent(event);

            QPainter painter(this);
            painter.setRenderHint(QPainter::Antialiasing);

            QFont font("Arial", fontSize);
            painter.setFont(font);

            QPen pen(Qt::darkRed);
            painter.setPen(pen);

            QRect ellipseRect(50, 100, width() - 100, height() - 100);
            QString text = textEdit->text();
            bool widen=false;
            for (int i = 0; i < text.length(); ++i) {
                QPointF point;
                point.setX(ellipseRect.center().x() + ellipseRect.width() / 2 * cos(angle));
                point.setY(ellipseRect.center().y() + ellipseRect.height() / 2 * sin(angle));

                painter.drawText(point, QString(text[i]));

                // Увеличиваем угол для следующего символа
                angle += angleIncrement;
                if(i>88&&!widen){
                angleIncrement+=0.001;
                widen=true;
                }
                if(i<88){
                    fontSize += fontSizeIncrement;
                    painter.setFont(QFont("Arial", fontSize));
                }
                // Увеличиваем размер шрифта для следующего символа
            }
}
void Widget::on_convertButton_clicked()
{
    angle = 3.1415926535;
            fontSize = 10;
            update();
}
/*void Widget::onDrawButtonClick()
{
    angle = 3.14;
            fontSize = 10;
            update();
}*/

