#include "albummodel_mediaplayer.h"

ListModel::ListModel(QObject *parent) :
    QAbstractListModel(parent),
    m_count(0)
{

}

int ListModel::count() const
{
    return m_count;
}

int ListModel::rowCount(const QModelIndex &p) const
{
    Q_UNUSED(p)
    return m_data.size();
}

QVariant ListModel::data(const QModelIndex &index, int role) const
{
    if (index.row() < 0 || index.row() >= m_data.count())
        return QVariant();
    const ModelItem& item = m_data[index.row()];
    switch (role) {
    case KeyRole: return item.item_key();
    case TitleRole: return item.item_title();
    case SubTitleRole: return item.item_subtitle();
    case TypeRole: return item.item_type();
    case ImageUrlRole: return item.item_imageUrl();
    case CommandsRole: return item.item_commands();
    }
    return QVariant();
}

QHash<int, QByteArray> ListModel::roleNames() const {
    QHash<int, QByteArray> roles;
    roles[KeyRole] = "item_key";
    roles[TitleRole] = "item_title";
    roles[SubTitleRole] = "item_subtitle";
    roles[TypeRole] = "item_type";
    roles[ImageUrlRole] = "item_image";
    roles[CommandsRole] = "item_commands";
    return roles;
}

void ListModel::append(ModelItem& o)
{
    int i = m_data.size();
    beginInsertRows(QModelIndex(), i, i);
    m_data.append(o);

    // Emit changed signals
    emit countChanged(count());

    endInsertRows();
}

void ListModel::setCount(int count)
{
    if (m_count == count)
        return;

    m_count = count;
    emit countChanged(m_count);
}

void BrowseModel::addItem(const QString &key, const QString &title, const QString &subtitle, const QString &type, const QString &imageUrl, const QVariant &commands)
{
    ListModel* model = static_cast<ListModel*>(m_model);
    ModelItem item = ModelItem(key, title, subtitle, type, imageUrl, commands);
    model->append(item);
    emit modelChanged();
}
