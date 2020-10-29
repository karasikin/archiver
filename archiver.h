#ifndef ARCHIVER_H
#define ARCHIVER_H

#include <QString>

class Archiver
{

public:

    const QString COMPRESS_EXTENSION = ".ibzip2";
    const QString UNCOMPRESS_EXTENSION = ".uncomp";

    Archiver(const QString &inputFilename, int blockSize = 10 * 1024)
        : inputFilename(inputFilename),
          blockSize(blockSize),
          message()
    {}

    bool compress();
    bool uncompress();

    QString getMessage() const;

private:

    QString inputFilename;
    int blockSize;
    QString message;
};

#endif // ARCHIVER_H
