#ifndef MGRIMPORTANIMATIONFRAME_H
#define MGRIMPORTANIMATIONFRAME_H

#include <fstream>
#include "exportimportstruct.h"
#include "../gameobjects/basicobject.h"
#include "mgrimport.h"

class MgrImportAnimationFrame: public MgrImport
{
    BasicObjectKeyframePtr keyframe;

    void loadFrameNum(std::fstream& file);
    void loadFrameGlobalTransformMatrix(std::fstream& file);
    void checkFrameSectionEnd(std::fstream& file);

public:
    MgrImportAnimationFrame(int blockSize): MgrImport(blockSize) { }
    ~MgrImportAnimationFrame() { }

    int loadFrame(std::fstream& file);

    BasicObjectKeyframePtr getKeyFrame()const { return keyframe; }
};

typedef std::shared_ptr<MgrImportAnimationFrame> MgrImportAnimationFramePtr;

#endif // IMPORTANIMATIONFRAME_H
