#include "FragmentShaders.h"
Texture *FragmentShader::texture = 0;
TextureMipMap *FragmentShaderBlendingMain::texture = 0;
TextureMipMap *FragmentShaderBlendingDistPass::texture = 0;

int FragmentShaderBlendingDistPass::idx =-1;
int FragmentShaderBlendingDistPass::triIdx;
int FragmentShaderBlendingMain::triIdx;

const int FragmentShaderBlendingMain::mipmapL[]= {0,2,4};
int FragmentShaderBlendingMain::idx;
bool FragmentShaderVarMain::writeOut;
FILE *FragmentShaderVarMain::f_arr[3];
IplImage *FragmentShaderVarMain::outputImage;
 std::vector<osg::Vec2> FragmentShaderCollectTC:: tc;
Rect gRect;
REGION * FragmentShaderBlendingDistPass::regRange;
REGION * FragmentShaderBlendingMain::regOutput;
REGION * FragmentShaderBlendingMain::regRange;
REGION * FragmentShader::regOutput;
dcm_t *FragmentShaderBlendingMain::doublecountmapPtr;
dcm_t *FragmentShaderBlendingDistPass::doublecountmapPtr;
int *FragmentShaderBlendingDistPass::doubleTouchCountPtr;

int doubleTouchCount[2];

dcm_t doublecountmap[2];
