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
