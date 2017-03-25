#pragma once
#include <QObject>
#include <QFileSystemWatcher>
#include <string>
#include <fstream>

using std::string;
using std::ifstream;

/*
 * Monitors a file and emits events when changes are detected.
 * User can subscribe to this event to asyncly receive updates.
 * Useful for writing a Logging mechanism based on file output.
*/

class FileWatcher: public QObject
{
    Q_OBJECT

public:
    FileWatcher(string filename);
    void test();

signals:
    void fileChanged(string newContent);

private slots:
    void fileChanged(const QString& filePath);

private:
    ifstream _fileStream;
    QFileSystemWatcher* _fileSystemWatcher;
};
