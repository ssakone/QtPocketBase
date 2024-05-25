#ifndef COLLECTIONFILEFIELDOPTIONS_H
#define COLLECTIONFILEFIELDOPTIONS_H

#include <QObject>
#include "CollectionFieldOptions.h"

class CollectionFileFieldOptions : public CollectionFieldOptions
{
    Q_OBJECT
    Q_PROPERTY(int maxSelect READ maxSelect WRITE setMaxSelect NOTIFY maxSelectChanged FINAL)
    Q_PROPERTY(int maxSize READ maxSize WRITE setMaxSize NOTIFY maxSizeChanged FINAL)
    Q_PROPERTY(QList<MimeType> mimeTypes READ mimeTypes WRITE setMimeTypes NOTIFY mimeTypesChanged FINAL)
    Q_PROPERTY(QStringList thumbs READ thumbs WRITE setThumbs NOTIFY thumbsChanged FINAL)
public:

    enum MimeType {
        ImageJPEG,
        ImagePNG,
        ImageGIF,
        ImageSVG,
        VideoMP4,
        AudioMP3,
        DocumentPDF,
        DocumentDOC,
        DocumentDOCX,
        DocumentXLS,
        DocumentXLSX,
        DocumentTXT,
        Other
    };

    Q_ENUM(MimeType);

    explicit CollectionFileFieldOptions(QObject *parent = nullptr);

    QString mimeTypeToString(MimeType mimeType) const;
    int maxSelect() const;
    void setMaxSelect(int newMaxSelect);
    int maxSize() const;
    void setMaxSize(int newMaxSize);

    QList<CollectionFileFieldOptions::MimeType> mimeTypes() const;
    void setMimeTypes(const QList<CollectionFileFieldOptions::MimeType> &newMimeTypes);

    QStringList thumbs() const;
    void setThumbs(const QStringList &newThumbs);

    QJsonObject toJson() const override;

signals:
    void maxSelectChanged();
    void maxSizeChanged();

    void mimeTypesChanged();

    void thumbsChanged();

private:
    int m_maxSelect = 1;
    int m_maxSize = 5242880;
    QList<MimeType> m_mimeTypes;
    QStringList m_thumbs;
};

#endif // COLLECTIONFILEFIELDOPTIONS_H
