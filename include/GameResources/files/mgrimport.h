#ifndef MGRIMPORT_H
#define MGRIMPORT_H

class MgrImport
{
protected:
    int errorFlag;  //0 - no error; 1 - error while loading; 2 - wrong header, function skipped; 3 - section footer found; 4 - section footer expected but not found; 5 - error handler
    int blockSize;
    char* block;

public:
    MgrImport(int blockSize)
        : errorFlag(0)
    {
        this->blockSize = blockSize;
        block = new char[blockSize];
    }

    int getErrorFlag()const { return errorFlag; }

    virtual ~MgrImport(){
        delete[]block;
    }
};

#endif // MGRIMPORT_H
