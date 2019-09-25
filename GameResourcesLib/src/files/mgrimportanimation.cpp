#include <stdafx.h>

int MgrImportAnimation::loadAnimation(std::fstream& file) {
    animationJoint = std::make_shared<AnimationJoint>();

    loadAnimationName(file);

    if(!errorFlag) loadAnimationFrameCount(file);

    if(!errorFlag) loadAnimationFrames(file);

    return errorFlag;
}


//-----------------------------------------------------------------------------
//-----------------------------------PRIVATE-----------------------------------
//-----------------------------------------------------------------------------

void MgrImportAnimation::loadAnimationName(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    animationJoint->name = std::string(block);

    errorFlag = 0;
}

void MgrImportAnimation::loadAnimationFrameCount(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    file.read(block, blockSize * sizeof(char));
    if ((file.rdstate()&std::fstream::eofbit) != 0){
        errorFlag = 1;
        return;
    }

    animationJoint->frameCount = *((int*)block);

    errorFlag = 0;
}

void MgrImportAnimation::loadAnimationFrames(std::fstream& file)
{
    std::memset(block,0x00,blockSize*sizeof(char));

    while(!errorFlag){
        file.read(block, blockSize * sizeof(char));
        loadAnimationFrameData(file,block);
        if(errorFlag==2) checkAnimationSectionEnd(file,block);
    }

    if(errorFlag==3) errorFlag = 0;
}

void MgrImportAnimation::loadAnimationFrameData(std::fstream& file, const char* headerBlock)
{
    if (strcmp(headerBlock, FILEHDR::frameHeader) == 0) {
        std::unique_ptr<MgrImportAnimationFrame> animFrame = std::make_unique<MgrImportAnimationFrame>(blockSize);
        if (animFrame->loadFrame(file) != 0) {
            errorFlag = 1;
            return;
        }

        if(animationJoint->frames.size() > 0)
            animationJoint->frames[animationJoint->frames.size() - 1]->nextFrame = animFrame->getKeyFrame();
        animationJoint->frames.push_back(animFrame->getKeyFrame());

        errorFlag = 0;
        return;
    }
    errorFlag = 2;
}

void MgrImportAnimation::checkAnimationSectionEnd(std::fstream& file, const char* footerBlock)
{
    if (strncmp(footerBlock, FILEHDR::animationsFooter,sizeof(FILEHDR::animationsFooter)/sizeof(char)) == 0) {
        errorFlag = 3;
        return;
    }
    else if ((file.rdstate()&std::fstream::eofbit) != 0) {
        errorFlag = 1;
        return;
    }
    errorFlag = 0;
}
