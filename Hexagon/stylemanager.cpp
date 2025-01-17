#include "stylemanager.h"

QString StyleManager::buttonStyle()
{
    return QString(
        "QPushButton {"
        "        background-color:rgb(158, 175, 76);" /* Normal background color */
        "        font-size: 18px; font-weight: bold;"
        "        color: white;" /* Normal text color */
        "        border: 2px solidrgb(170, 176, 68);"
        "        border-radius: 5px;"
        "        padding: 5px;"
        "    }"
        "    QPushButton:hover {"
        "        background-color:rgb(170, 181, 72);" /* Highlighted background color on hover */
        "        border: 2px solidrgb(170, 176, 68);"
        "    }"
        "    QPushButton:pressed {"
        "        background-color:rgb(128, 154, 43);" /* Background color when pressed */      
        "    }"
        "    QPushButton:disabled {"
        "        background-color: rgb(200, 200, 200);" /* Gray background for disabled state */
        "        color: rgb(150, 150, 150);" /* Light gray text color for disabled state */
        "        border: 2px solid rgb(190, 190, 190);" /* Light gray border for disabled state */
        "    }"
    );
}

QString StyleManager::QComboBoxStyle()
{
    return QString(
        "QComboBox {"
        "    background-color: rgb(158, 175, 76);" /* Normal background color */
        "    color: white;" /* Normal text color */
        "    font-weight: bold;" /* Bold text */
        "    border: 2px solid rgb(170, 176, 68);" /* Normal border */
        "    border-radius: 5px;" /* Rounded corners */
        "    padding: 5px;" /* Padding */
        "    text-align: center;" /* Centered text */
        "}"
        "QComboBox::drop-down {"
        "    border: none;" /* No border */
        "}"
        "QComboBox QAbstractItemView {" /* Style for the dropdown list */
        "    background-color: rgb(158, 175, 76);" /* Normal background color */
        "    color: white;" /* Normal text color */
        "    font-weight: bold;" /* Bold text */
        "    border: 2px solid rgb(170, 176, 68);" /* Normal border */
        "    border-radius: 5px;" /* Rounded corners */
        "    padding: 5px;" /* Padding */
        "    text-align: center;" /* Centered text */
        "}"

    );
}

QString StyleManager::transparentLineEditStyle()
{
    return QString(
       "QLineEdit {"
    "    background: transparent;"    
    "    color: white;"               
    "    font-weight: bold;"          
    "    border: none;"               
    "}"
    );
}

QString StyleManager::transparentQListViewStyle()
{
    return QString(
        "QListView {"
        "    background: transparent;" /* Transparent background */
        "    color: white;" /* White text color */
        "    font-weight: bold;" /* Bold text */
        "    border: none;" /* No border */
        "}"
        "QListView::item:selected {"
        "background: rgb(158, 175, 76);" /* background color for selected Items */
        "color: white;" /* text color for selected items */
        "}"
    );
}

QString StyleManager::transparentQTextBrowserStyle()
{
    return QString(
        "QTextBrowser {"
        "    background: transparent;" /* Transparent background */
        "    color: white;" /* White text color */
        "    font-weight: bold;" /* Bold text */
        "    border: none;" /* No border */
        "}"
    );
}

QString StyleManager::whiteLabelStyle()
{
    return QString(
        "QLabel {"
        "    color: white;" /* White text color */
        "    font-weight: bold;" /* Bold text */
        "}"
    );
}