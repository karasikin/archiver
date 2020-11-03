#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <memory>
#include <QString>
#include <QFile>

class ByteConverter;


class Archiver : public QObject
{
    Q_OBJECT

public:

    Archiver(const QString &inputFilename, const QString &outputFilename,
             int compressBlockSize = 100 * 1024, QObject *parent = nullptr);
    ~Archiver();

    bool compress();
    bool uncompress();

    void addConvertingAlgorithms(ByteConverter *convertingAlgorithm);

    QString getMessage() const;

    int getCompressBlockSize() const { return compressBlockSize; }
    void setCompressBlockSize(int blockSize) { this->compressBlockSize = blockSize; }

signals:
    void blockCompressed(int blockSize);
    void blockUncompressed(int blockSize);
    void startCompress(int inputFileSize);
    void startUncompress(int inputFileSize);

private:

    bool openFiles();

    std::unique_ptr<QByteArray> readBlock(int blockSize);
    void writeBlock(const QByteArray *block);

    int readEncodedBlockSize();
    void writeEncodedBlockSize(int blockSize);

private:

    QFile inputFile;
    QFile outputFile;

    QVector<ByteConverter *> convertingAlgorithms;

    int compressBlockSize;

    QString message;
};

#endif // ARCHIVER_H
