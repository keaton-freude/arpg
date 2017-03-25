#include "FileWatcher.h"

FileWatcher::FileWatcher(std::string filename)
    : _fileStream(filename)
{
    _fileSystemWatcher = new QFileSystemWatcher(
        { QString::fromStdString(filename) } );
}

void FileWatcher::test()
{
    emit(fileChanged("Blah"));
}
