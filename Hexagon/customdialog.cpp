#include "customdialog.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>

CustomDialog::CustomDialog(QWidget *parent) :
    QDialog(parent),
    backgroundLabel(new QLabel(this)),
    messageLabel(new QLabel(this)),
    button1(new QPushButton(this)),
    button2(new QPushButton(this))
{
    // Set dialog properties
    setWindowFlags(Qt::FramelessWindowHint | Qt::Dialog);
    setModal(true);
    setAttribute(Qt::WA_TranslucentBackground);
    resize(400, 300); // Default size

    // Set up the background label
    backgroundLabel->setScaledContents(true); // Ensure it fills the entire dialog
    backgroundLabel->setGeometry(this->rect()); // Make it fill the dialog

    // Set up the message label
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("color: white; font-size: 16px; font-weight: bold;");
    messageLabel->setWordWrap(true);

    // Set up the buttons
    // Set stylesheets for hover effect on buttons
    QString buttonStyle = R"(
        QPushButton {
            background-color:rgb(158, 175, 76); /* Normal background color */
            color: white; /* Normal text color */
            border: 2px solidrgb(170, 176, 68);
            border-radius: 5px;
            padding: 5px;
        }
        QPushButton:hover {
            background-color:rgb(170, 181, 72); /* Highlighted background color on hover */
            border: 2px solidrgb(170, 176, 68);
        }
        QPushButton:pressed {
            background-color:rgb(128, 154, 43); /* Background color when pressed */
        }
        QPushButton:disabled {
            background-color: rgb(200, 200, 200); /* Gray background for disabled state */
            color: rgb(150, 150, 150); /* Light gray text color for disabled state */
            border: 2px solid rgb(190, 190, 190); /* Light gray border for disabled state */
        }
    )";
    button1->setStyleSheet(buttonStyle);
    button2->setStyleSheet(buttonStyle);
    button2->setVisible(false); // Hide the second button by default

    // Layout for buttons
    QHBoxLayout *buttonLayout = new QHBoxLayout();
    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);

    // Main layout
    QVBoxLayout *mainLayout = new QVBoxLayout(this);
    mainLayout->setContentsMargins(20, 20, 20, 20); // Add margins for proper positioning
    mainLayout->addWidget(messageLabel, 1, Qt::AlignCenter);
    mainLayout->addLayout(buttonLayout);

    setLayout(mainLayout);

    // Connect buttons to dialog actions
    connect(button1, &QPushButton::clicked, this, &CustomDialog::accept);
    connect(button2, &QPushButton::clicked, this, &CustomDialog::reject);
}

CustomDialog::~CustomDialog() = default;

// Set the message text
void CustomDialog::setMessage(const QString &message)
{
    messageLabel->setText(message);
}

// Set the button labels
void CustomDialog::setButtonLabels(const QString &button1Text, const QString &button2Text)
{
    button1->setText(button1Text);
    if (!button2Text.isEmpty()) {
        button2->setText(button2Text);
        button2->setVisible(true);
    }
}

// Set the background image
void CustomDialog::setBackgroundImage(const QString &imagePath)
{
    QPixmap pixmap(imagePath);
    if (!pixmap.isNull()) {
        backgroundLabel->setPixmap(pixmap);
        backgroundLabel->setGeometry(this->rect()); // Ensure it fills the entire dialog
        backgroundLabel->lower(); // Send it to the back
    } else {
        qWarning() << "Could not load background image:" << imagePath;
    }
}

// Static method for one-button dialog
int CustomDialog::showDialogWithOneButton(const QString &message, const QString &button1Text, const QString &backgroundImage, QWidget *parent)
{
    CustomDialog dialog(parent);
    dialog.setMessage(message);
    dialog.setButtonLabels(button1Text);
    if (!backgroundImage.isEmpty()) {
        dialog.setBackgroundImage(backgroundImage);
    }
    return dialog.exec();
}

// Static method for two-button dialog
int CustomDialog::showDialogWithTwoButtons(const QString &message, const QString &button1Text, const QString &button2Text, const QString &backgroundImage, QWidget *parent)
{
    CustomDialog dialog(parent);
    dialog.setMessage(message);
    dialog.setButtonLabels(button1Text, button2Text);
    if (!backgroundImage.isEmpty()) {
        dialog.setBackgroundImage(backgroundImage);
    }
    return dialog.exec();
}
