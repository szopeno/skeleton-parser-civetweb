#include "ZipFile.h"
#include <istream>
#include <iostream>
#include <sys/stat.h>

int main(int argc, char **argv)
{
    std::stringbuf sb;
    sb << "ala";
    char *s = "hek";
    sb << s;
    ZipArchive::Ptr archive = ZipFile::Open(argv[1]);

    int noe = archive->GetEntriesCount();

    for (int i;i<noe;i++) {
        auto entry = archive->GetEntry(i);

        std::string name = entry->GetFullName();
        std::cout << "Extracting " << name << std::endl;
        if (entry->IsDirectory()) {
            mkdir( name.c_str(), 0766 );
            continue;
        }
        ZipFile::ExtractFile( argv[1], name, name );
    }

 /*   ZipArchiveEntry::Ptr entry = archive->GetEntry("main.jpdl");

    size_t fs = entry->GetSize();
    std::istream *uncompressed = entry->GetDecompressionStream();

    char *buffer = new char[fs+1];
    buffer[fs]=0;

    uncompressed->read(buffer,fs);

    ZipFile::SaveAndClose(archive, argv[1]);

    std::cout << buffer << std::endl;*/
}
