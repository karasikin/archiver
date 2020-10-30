#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <memory>
#include <QString>
#include <QFile>

class Archiver
{

public:

//    const QString COMPRESS_EXTENSION = ".ibzip2";
//    const QString UNCOMPRESS_EXTENSION = ".uncomp";

    Archiver(const QString &inputFilename, const QString &outputFilename, int compressBlockSize = 10 * 1024);

    bool compress();
    bool uncompress();

    QString getMessage() const;

    int getCompressBlockSize() const { return compressBlockSize; }
    void setCompressBlockSize(int blockSize) { this->compressBlockSize = blockSize; }

private:

    bool openFiles();

    std::unique_ptr<QByteArray> readBlock(int blockSize);
    void writeBlock(const QByteArray *block);

    int readEncodedBlockSize();
    void writeEncodedBlockSize(int blockSize);

private:

    QFile inputFile;
    QFile outputFile;

    int compressBlockSize;

    QString message;
};

#endif // ARCHIVER_H
