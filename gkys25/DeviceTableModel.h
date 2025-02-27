#ifndef DEVICETABLEMODEL_H
#define DEVICETABLEMODEL_H

#include <QAbstractTableModel>
#include <QJsonDocument>
#include <QJsonArray>
#include <QJsonObject>

class DeviceTableModel : public QAbstractTableModel
{
    Q_OBJECT

    enum DeviceRoles {
        DeviceIndex =  Qt::UserRole + 1,
        CameraNameRole,
        DeviceSerialRole,
        CameraNoRole,
        VideoLevelRole
    };

public:
    explicit DeviceTableModel(QObject *parent = nullptr);

    // Header:
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const override;

    // Basic functionality:
    int rowCount(const QModelIndex &parent = QModelIndex()) const override;
    int columnCount(const QModelIndex &parent = QModelIndex()) const override;

    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const override;

    void updateModelData(const QJsonArray& data);

protected:
    virtual QHash<int, QByteArray> roleNames() const;

private:
    QJsonArray m_modelData;
    QList<QString> m_headerData;
};

#endif // DEVICETABLEMODEL_H
