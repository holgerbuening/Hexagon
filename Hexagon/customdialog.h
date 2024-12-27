#ifndef CUSTOMDIALOG_H
#define CUSTOMDIALOG_H

#include <QDialog>
#include <QLabel>
#include <QPushButton>

class CustomDialog : public QDialog
{
    Q_OBJECT

public:
    explicit CustomDialog(QWidget *parent = nullptr);
    ~CustomDialog();

    // Methods to customize the dialog
    void setMessage(const QString &message);
    void setButtonLabels(const QString &button1Text, const QString &button2Text = "");
    void setBackgroundImage(const QString &imagePath);

    // Static methods for simple usage
    static int showDialogWithOneButton(const QString &message, const QString &button1Text, const QString &backgroundImage = "", QWidget *parent = nullptr);
    static int showDialogWithTwoButtons(const QString &message, const QString &button1Text, const QString &button2Text, const QString &backgroundImage = "", QWidget *parent = nullptr);

private:
    QLabel *backgroundLabel;
    QLabel *messageLabel;
    QPushButton *button1;
    QPushButton *button2;
};

#endif // CUSTOMDIALOG_H

