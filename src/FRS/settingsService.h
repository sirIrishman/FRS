#ifndef FRS_SETTINGSSERVICE_H
#define FRS_SETTINGSSERVICE_H

#include <QtCore/qsettings.h>
#include <QtCore/qfile.h>
#include <QtWidgets/qapplication.h>

namespace services {
    class SettingsService sealed {
    public:
        static void associateWith(QObject* const& parent) {
            instance()._settings.setParent(parent);
        }
        static QVariant getSetting(QString const& groupName, QString const& subgroupName, QString const& settingName) {
            return getSetting(groupName, subgroupName, settingName, QVariant());
        }
        static QVariant getSetting(QString const& groupName, QString const& subgroupName, QString const& settingName, QVariant const& defaultValue) {
            instance()._settings.beginGroup(groupName);
            instance()._settings.beginGroup(subgroupName);
            QVariant settingValue = instance()._settings.value(settingName, defaultValue);
            instance()._settings.endGroup(); //subgroupName
            instance()._settings.endGroup(); //groupName
            return settingValue;
        }
        static void fill() {
            instance()._settings.beginGroup("objectDetection");

            instance()._settings.beginGroup("haarCascade");
            instance()._settings.setValue("cascadeFileName", "haarcascade_frontalface_alt.xml");
            instance()._settings.setValue("scaleFactor", 1.15);
            instance()._settings.setValue("minNeighbors", 3);
            instance()._settings.setValue("flags", 2);
            instance()._settings.setValue("minSizeWidth", 40);
            instance()._settings.setValue("minSizeHeight", 40);
            instance()._settings.endGroup();

            instance()._settings.beginGroup("lbpCascade");
            instance()._settings.setValue("cascadeFileName", "lbpcascade_frontalface.xml");
            instance()._settings.setValue("scaleFactor", 1.15);
            instance()._settings.setValue("minNeighbors", 3);
            instance()._settings.setValue("flags", 2);
            instance()._settings.setValue("minSizeWidth", 40);
            instance()._settings.setValue("minSizeHeight", 40);
            instance()._settings.endGroup();

            instance()._settings.endGroup();
        }
        static bool empty() {
            return QFile::exists(instance().settingsFileFullName()) == false;
        }

    private:
        const QString __settingsFileName;
        QSettings _settings;

        SettingsService() 
            : __settingsFileName("Settings.ini"),
            _settings(settingsFileFullName(), QSettings::IniFormat) {
        }

        static SettingsService& instance() {
            static SettingsService instance;
            return instance;
        }
        QString settingsFileFullName() const {
            return QString("%1/%2").arg(QApplication::applicationDirPath(), __settingsFileName);
        }
    };
}

#endif //FRS_SETTINGSSERVICE_H
