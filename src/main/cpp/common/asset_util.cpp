/**
 *  author : suzhou
 *  date : 2023/8/19 
 *  description : 
 */

#include "asset_util.h"
#include "android_log.h"


// 返回字符数组，用完后需要手动调用delete释放空间
GLubyte *asset_readFile(AAssetManager *assetManager, const char *filename) {
    AAsset *aAsset = AAssetManager_open(assetManager, filename, AASSET_MODE_BUFFER);
    off_t len = AAsset_getLength(aAsset);
    LOGD("readFile, len = %ld", len);

    auto *buf = new GLubyte[len + 1];
    buf[len] = '\0';

    off_t count = AAsset_read(aAsset, buf, len);
    LOGD("readFile, count = %ld", count);

    return buf;
}


