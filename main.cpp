#include <iostream>
#include <QApplication>
#include <QWidget>
#include <QHBoxLayout>
#include <QVBoxLayout>
#include <QLabel>
#include <QLineEdit>
#include <QPushButton>
#include <QFrame>
#include <QPainter>
#include <QTimer>
#include <QFile>
#include <QTextStream>
#include "tracker.h"

class ProgressRing : public QWidget {
    int targetVal = 0;
    int goalVal = 2000;
    float displayVal = 0.0f;
    QTimer *animTimer;
    QColor ringColor;
public:
    ProgressRing(QColor color, QWidget *parent = nullptr) : QWidget(parent), ringColor(color) {
        setFixedSize(130, 130);
        animTimer = new QTimer(this);
        

        connect(animTimer, &QTimer::timeout, [this]() {
            if (qAbs(displayVal - targetVal) < 0.1f) {
                displayVal = targetVal;
                animTimer->stop();
            } else {
                displayVal += (targetVal - displayVal) * 0.2f;
            }
            update();
        });
    }

    void setData(int current, int goal) {
        targetVal = current;
        goalVal = goal > 0 ? goal : 1;
        if (!animTimer->isActive()) {
            animTimer->start(15);
        }
    }

protected:
    void paintEvent(QPaintEvent *) override {
        QPainter painter(this);
        painter.setRenderHint(QPainter::Antialiasing);
        QRectF rect(10, 10, 110, 110);

        // Background track ring
        QPen bgPen(QColor(36, 36, 51), 10, Qt::SolidLine, Qt::RoundCap);
        painter.setPen(bgPen);
        painter.drawEllipse(rect);

        int percent = (int)((displayVal * 100.0f) / goalVal);
        if (percent > 100) percent = 100;
        int spanAngle = -percent * 360 * 16 / 100;

        QPen fgPen(ringColor, 10, Qt::SolidLine, Qt::RoundCap);
        painter.setPen(fgPen);
        painter.drawArc(rect, 90 * 16, spanAngle);

        // Center Percentage Text
        painter.setPen(QColor(205, 214, 244));
        QFont font = painter.font();
        font.setBold(true);
        font.setPointSize(16);
        painter.setFont(font);

        QString text = QString::number(percent) + "%";
        painter.drawText(rect, Qt::AlignCenter, text);
    }
};

QString loadStyleSheet(const QString &filePath) {
    QFile file(filePath);
    if (file.open(QFile::ReadOnly | QFile::Text)) {
        QTextStream in(&file);
        return in.readAll();
    }
    return "";
}

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);

    QWidget window;
    window.setWindowTitle("Daily Health Tracker");
    window.resize(700, 330);
    window.setStyleSheet(loadStyleSheet("style.qss"));

    int goalCalories = 2000;
    int currentCalories = 0;
    int goalWater = 100;
    int currentWater = 0;

    QHBoxLayout *mainLayout = new QHBoxLayout(&window);
    mainLayout->setContentsMargins(16, 16, 16, 16);
    mainLayout->setSpacing(16);

    // LEFT COLUMN
    QFrame *leftCard = new QFrame(&window);
    leftCard->setObjectName("card");
    QVBoxLayout *leftLayout = new QVBoxLayout(leftCard);
    leftLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *leftTitle = new QLabel("Daily Setup", leftCard);
    leftTitle->setObjectName("titleLabel");

    // Setup Group
    QVBoxLayout *setupGroup = new QVBoxLayout();
    QLabel *cGoalLbl = new QLabel("Calorie Goal:", leftCard);
    QLineEdit *cGoalInput = new QLineEdit(leftCard);
    cGoalInput->setPlaceholderText("e.g. 2000");

    QLabel *wGoalLbl = new QLabel("Water Goal (oz):", leftCard);
    QLineEdit *wGoalInput = new QLineEdit(leftCard);
    wGoalInput->setPlaceholderText("e.g. 100");

    QPushButton *saveButton = new QPushButton("Lock In Goals", leftCard);
    setupGroup->addWidget(cGoalLbl);
    setupGroup->addWidget(cGoalInput);
    setupGroup->addWidget(wGoalLbl);
    setupGroup->addWidget(wGoalInput);
    setupGroup->addSpacing(8);
    setupGroup->addWidget(saveButton);

    //  Logging Group
    QVBoxLayout *logGroup = new QVBoxLayout();
    logGroup->setSpacing(10);

    QLabel *logCalLbl = new QLabel("Log Custom Calories:", leftCard);
    QHBoxLayout *logCalRow = new QHBoxLayout();
    QLineEdit *customCalInput = new QLineEdit(leftCard);
    customCalInput->setPlaceholderText("Amount (e.g. 450)");
    QPushButton *logCalBtn = new QPushButton("Add Cals", leftCard);
    logCalBtn->setObjectName("actionBtn");
    logCalRow->addWidget(customCalInput);
    logCalRow->addWidget(logCalBtn);

    QLabel *logWaterLbl = new QLabel("Log Custom Water (oz):", leftCard);
    QHBoxLayout *logWaterRow = new QHBoxLayout();
    QLineEdit *customWaterInput = new QLineEdit(leftCard);
    customWaterInput->setPlaceholderText("Amount (e.g. 16)");
    QPushButton *logWaterBtn = new QPushButton("Add Water", leftCard);
    logWaterBtn->setObjectName("actionBtn");
    logWaterRow->addWidget(customWaterInput);
    logWaterRow->addWidget(logWaterBtn);

    logGroup->addWidget(logCalLbl);
    logGroup->addLayout(logCalRow);
    logGroup->addSpacing(4);
    logGroup->addWidget(logWaterLbl);
    logGroup->addLayout(logWaterRow);

    QWidget *logWidget = new QWidget(leftCard);
    logWidget->setLayout(logGroup);
    logWidget->hide();

    leftLayout->addWidget(leftTitle);
    leftLayout->addLayout(setupGroup);
    leftLayout->addWidget(logWidget);
    leftLayout->addStretch();


    // RIGHT COLUMN
    QFrame *rightCard = new QFrame(&window);
    rightCard->setObjectName("card");
    QVBoxLayout *rightLayout = new QVBoxLayout(rightCard);
    rightLayout->setContentsMargins(0, 0, 0, 0);

    QLabel *rightTitle = new QLabel("Today's Overview", rightCard);
    rightTitle->setObjectName(QString("titleLabel"));
    rightTitle->setAlignment(Qt::AlignCenter);

    QHBoxLayout *ringsRow = new QHBoxLayout();
    ringsRow->setSpacing(24);

    // Water Ring Column
    QVBoxLayout *wRingBox = new QVBoxLayout();
    ProgressRing *waterRing = new ProgressRing(QColor(137, 220, 235), rightCard); // Cyan
    QLabel *waterValLbl = new QLabel("0 / 100 oz", rightCard);
    waterValLbl->setObjectName("subLabel");
    waterValLbl->setAlignment(Qt::AlignCenter);
    wRingBox->addWidget(waterRing, 0, Qt::AlignCenter);
    wRingBox->addSpacing(6);
    wRingBox->addWidget(waterValLbl, 0, Qt::AlignCenter);

    // Calorie Ring Column
    QVBoxLayout *cRingBox = new QVBoxLayout();
    ProgressRing *calRing = new ProgressRing(QColor(166, 227, 161), rightCard); // Soft Green
    QLabel *calValLbl = new QLabel("0 / 2000 cal", rightCard);
    calValLbl->setObjectName("subLabel");
    calValLbl->setAlignment(Qt::AlignCenter);
    cRingBox->addWidget(calRing, 0, Qt::AlignCenter);
    cRingBox->addSpacing(6);
    cRingBox->addWidget(calValLbl, 0, Qt::AlignCenter);

    ringsRow->addLayout(wRingBox);
    ringsRow->addLayout(cRingBox);

    rightLayout->addWidget(rightTitle);
    rightLayout->addSpacing(12);
    rightLayout->addLayout(ringsRow);
    rightLayout->addStretch();

    mainLayout->addWidget(leftCard, 1);
    mainLayout->addWidget(rightCard, 1);


    // EVENTS   
    QObject::connect(saveButton, &QPushButton::clicked, [=, &goalCalories, &goalWater]() {
        if (!cGoalInput->text().isEmpty()) goalCalories = cGoalInput->text().toInt();
        if (!wGoalInput->text().isEmpty()) goalWater = wGoalInput->text().toInt();

        saveGoals(goalCalories, goalWater);

        leftTitle->setText("Live Logging");
        cGoalLbl->hide();
        cGoalInput->hide();
        wGoalLbl->hide();
        wGoalInput->hide();
        saveButton->hide();

        logWidget->show();
        waterRing->setData(currentWater, goalWater);
        calRing->setData(currentCalories, goalCalories);
    });

    QObject::connect(logWaterBtn, &QPushButton::clicked, [=, &currentWater, &goalWater]() {
        int added = customWaterInput->text().toInt();
        if (added > 0) {
            currentWater += added;
            waterRing->setData(currentWater, goalWater);
            waterValLbl->setText(QString::number(currentWater) + " / " + QString::number(goalWater) + " oz");
            customWaterInput->clear();
        }
    });

    QObject::connect(logCalBtn, &QPushButton::clicked, [=, &currentCalories, &goalCalories]() {
        int added = customCalInput->text().toInt();
        if (added > 0) {
            currentCalories += added;
            calRing->setData(currentCalories, goalCalories);
            calValLbl->setText(QString::number(currentCalories) + " / " + QString::number(goalCalories) + " cal");
            customCalInput->clear();
        }
    });

    window.show();
    return app.exec();
}