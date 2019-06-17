#ifndef CONFIG_H
#define CONFIG_H

#include <QObject>
#include <QSettings>

class Config : public QObject
{
  Q_OBJECT
  Q_PROPERTY(QString f_name READ getF_name WRITE setF_name)
public:
  explicit Config(QObject *parent = nullptr);
  Config(QString file, QObject *parent = nullptr);

  QString getF_name() const;
  void setF_name(const QString &value);

signals:

public slots:

private:
  QString f_name;
  QSettings settings = QSettings(QSettings::Format::IniFormat, QSettings::UserScope, "MySoft", "Star Runner");
};

#endif // CONFIG_H
