#include "DeviceTableModel.h"
#include <QDebug>

DeviceTableModel::DeviceTableModel(QObject *parent)
    : QAbstractTableModel(parent)
{
    m_headerData.push_back(u8"序号");
    m_headerData.push_back(u8"设备名称");
    m_headerData.push_back(u8"序列号");
    m_headerData.push_back(u8"通道号");
    m_headerData.push_back(u8"视频清晰度");
}

QVariant DeviceTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    return QVariant();
    if (Qt::DisplayRole != role)
        return QVariant();

    if (Qt::Horizontal == orientation) {
        return m_headerData.at(section);
    }

    return QVariant();
}

int DeviceTableModel::rowCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_modelData.size();
}

int DeviceTableModel::columnCount(const QModelIndex &parent) const
{
    if (parent.isValid())
        return 0;

    return m_headerData.size();
}

QVariant DeviceTableModel::data(const QModelIndex &index, int role) const
{
    if (role != Qt::DisplayRole)
        return QVariant();

    if (index.row() < 0 || index.row() >= m_modelData.count())
        return QVariant();

    int row = index.row();
    QJsonObject item = m_modelData.at(row).toObject();
    int column = index.column();
    int roleType = Qt::UserRole + 1 + column;
    switch (roleType) {
    case DeviceIndex:
        return row + 1;
    case CameraNameRole:
        return item.value("cameraName").toString();
    case DeviceSerialRole:
        return item.value("deviceSerial").toString();
    case CameraNoRole:
        return item.value("cameraNo").toInt();
    case VideoLevelRole:
        return item.value("videoLevel").toInt();
    default:
        break;
    }

    return QVariant();
}


QHash<int, QByteArray> DeviceTableModel::roleNames() const
{
    QHash<int, QByteArray> roles;
    roles[DeviceIndex] = "deviceIndex";
    roles[DeviceSerialRole] = "deviceSerial";
    roles[CameraNoRole]   = "cameraNo";
    roles[CameraNameRole] = "cameraName";
    roles[VideoLevelRole] = "videoLevel";
    return roles;
}

void DeviceTableModel::updateModelData(const QJsonArray& data)
{
    beginResetModel();
    m_modelData = data;
    endResetModel();
}
