#include "customdialog.h"
#include "stylemanager.h"
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPixmap>
#include <QDebug>

CustomDialog::CustomDialog(QWidget *parent) :
    QDialog(parent),
    backgroundLabel(new QLabel(this)),
    overlayLabel(new QLabel(this)),
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
    backgroundLabel->setGeometry(this->rect());

    // Set up the overlay label
    overlayLabel->setGeometry(this->rect());
    overlayLabel->setStyleSheet("background-color: rgba(0, 0, 0, 128);"); // Semi-transparent black overlay
    overlayLabel->lower(); // Place it behind other widgets but above the background

    // Set up the message label
    messageLabel->setAlignment(Qt::AlignCenter);
    messageLabel->setStyleSheet("color: white; font-size: 24px; font-weight: bold;");
    messageLabel->setWordWrap(true);

    // Set up the buttons
    // Set stylesheets for hover effect on buttons
    QString buttonStyle = StyleManager::buttonStyle();
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

CustomDialog::~CustomDialog()
{
    //delete all widgets
    delete backgroundLabel;
    delete overlayLabel;
    delete messageLabel;
    delete button1;
    delete button2;
}

// Set the message text
void CustomDialog::setMessage(const QString &message)
{
    messageLabel->setTextFormat(Qt::RichText);
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
