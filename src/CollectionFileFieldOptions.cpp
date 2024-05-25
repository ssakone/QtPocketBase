#include "CollectionFileFieldOptions.h"
#include "qjsonarray.h"

CollectionFileFieldOptions::CollectionFileFieldOptions(QObject *parent)
    : CollectionFieldOptions{parent}
{}

QString CollectionFileFieldOptions::mimeTypeToString(CollectionFileFieldOptions::MimeType mimeType) const
{
    switch (mimeType) {
    case ImageJPEG:
        return "image/jpeg";
    case ImagePNG:
        return "image/png";
    case ImageGIF:
        return "image/gif";
    case ImageSVG:
        return "image/svg+xml";
    case VideoMP4:
        return "video/mp4";
    case AudioMP3:
        return "audio/mpeg";
    case DocumentPDF:
        return "application/pdf";
    case DocumentDOC:
        return "application/msword";
    case DocumentDOCX:
        return "application/vnd.openxmlformats-officedocument.wordprocessingml.document";
    case DocumentXLS:
        return "application/vnd.ms-excel";
    case DocumentXLSX:
        return "application/vnd.openxmlformats-officedocument.spreadsheetml.sheet";
    case DocumentTXT:
        return "text/plain";
    default:
        return "";
    }
}

int CollectionFileFieldOptions::maxSelect() const
{
    return m_maxSelect;
}

void CollectionFileFieldOptions::setMaxSelect(int newMaxSelect)
{
    if (m_maxSelect == newMaxSelect)
        return;
    m_maxSelect = newMaxSelect;
    emit maxSelectChanged();
}

int CollectionFileFieldOptions::maxSize() const
{
    return m_maxSize;
}

void CollectionFileFieldOptions::setMaxSize(int newMaxSize)
{
    if (m_maxSize == newMaxSize)
        return;
    m_maxSize = newMaxSize;
    emit maxSizeChanged();
}

QList<CollectionFileFieldOptions::MimeType> CollectionFileFieldOptions::mimeTypes() const
{
    return m_mimeTypes;
}

void CollectionFileFieldOptions::setMimeTypes(const QList<CollectionFileFieldOptions::MimeType> &newMimeTypes)
{
    if (m_mimeTypes == newMimeTypes)
        return;
    m_mimeTypes = newMimeTypes;
    emit mimeTypesChanged();
}

QStringList CollectionFileFieldOptions::thumbs() const
{
    return m_thumbs;
}

void CollectionFileFieldOptions::setThumbs(const QStringList &newThumbs)
{
    if (m_thumbs == newThumbs)
        return;
    m_thumbs = newThumbs;
    emit thumbsChanged();
}

QJsonObject CollectionFileFieldOptions::toJson() const
{
    QJsonObject json;
    json["maxSelect"] = m_maxSelect;
    json["maxSize"] = m_maxSize;
    QJsonArray mimeTypesArray;
    for (CollectionFileFieldOptions::MimeType mimeType : m_mimeTypes) {
        mimeTypesArray.append(mimeTypeToString(mimeType));
    }
    json["mimeTypes"] = mimeTypesArray;
    json["thumbs"] = QJsonArray::fromStringList(m_thumbs);
    return json;
}
