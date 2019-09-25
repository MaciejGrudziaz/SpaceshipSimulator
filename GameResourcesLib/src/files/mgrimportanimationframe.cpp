//#include "mgrimportanimationframe.h"
#include <stdafx.h>

int MgrImportAnimationFrame::loadFrame(std::fstream& file) {
    keyframe = std::make_shared<Keyframe>();

    loadFrameNum(file);

    if(!errorFlag) loadFrameGlobalTransformMatrix(file);

    if(!errorFlag) checkFrameSectionEnd(file);

    return errorFlag;
}

//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportAnimationFrame::loadFrameNum(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }

    keyframe->frameNum = *((int*)block);

    errorFlag = 0;
}

void MgrImportAnimationFrame::loadFrameGlobalTransformMatrix(std::fstream& file)
{
    Mat4Struct globalTransform;

    file.read((char*)(&globalTransform), sizeof(Mat4Struct));
    if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }

    keyframe->globalTransform = globalTransform.getGlmMat();

    errorFlag = 0;
}

void MgrImportAnimationFrame::checkFrameSectionEnd(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }
    if (strcmp(block, FILEHDR::frameFooter) != 0) {
        errorFlag = 3;
        return;
    }

    errorFlag = 0;
}
