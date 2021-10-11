#include <jni.h>
#include <string>

#include "android/bitmap.h"
#include "math.h"

#define PI acos(-1)

void gaussBlur1(int *pix, int w, int h, int radius);

extern "C"
JNIEXPORT void JNICALL
Java_com_hd_gaussdemo_MainActivity_gaussBlur(JNIEnv *env, jobject thiz, jobject bitmap) {
    // 初始化结构体
    AndroidBitmapInfo bitmapInfo = {0};
    int *data = NULL;
    // 取得图片信息
    AndroidBitmap_getInfo(env, bitmap, &bitmapInfo);
    // 锁定bitmap并锁定像素地址
    AndroidBitmap_lockPixels(env, bitmap, (void **) &data);
    gaussBlur1(data, bitmapInfo.width, bitmapInfo.height, 5);
    AndroidBitmap_unlockPixels(env, bitmap);
}


/**
 * 高斯模糊算法
 * @param pix 像素地址
 * @param w 宽
 * @param h 高
 * @param radius 模糊范围(影响模糊的程度,模糊半径1：则不使用模糊，模糊半径2，使用附件1一个像素数据生成，模糊半径80，则使用附近80个像素生成模糊数据)
 */
void gaussBlur1(int *pix, int w, int h, int radius) {
    float sigma = (1.0f * radius / 2.57);
    float deno = (1.0f / (sigma * sqrt(2.0 * PI)));
    float nume = (-1.0f / (2.0f * sigma * sigma));
    float *gaussMatrix = (float *) malloc(sizeof(float) * (radius + radius + 1));
    float gaussSum = 0.0;
    for (int i = 0, x = -radius; x <= radius; ++x, ++i) {
        float g = (float) (deno * exp(1.0 * nume * x * x));
        gaussMatrix[i] = g;
        gaussSum += g;
    }
    int len = radius + radius + 1;
    for (int i = 0; i < len; ++i)
        gaussMatrix[i] /= gaussSum;
    int *rowData = (int *) malloc(w * sizeof(int));
    int *listData = (int *) malloc(h * sizeof(int));
    for (int y = 0; y < h; ++y) {
        memcpy(rowData, pix + y * w, sizeof(int) * w);
        for (int x = 0; x < w; ++x) {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;
            for (int i = -radius; i <= radius; ++i) {
                int k = x + i;
                if (0 <= k && k <= w) {
                    //得到像素点的rgb值
                    int color = rowData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);
                    r += cr * gaussMatrix[i + radius];
                    g += cg * gaussMatrix[i + radius];
                    b += cb * gaussMatrix[i + radius];
                    gaussSum += gaussMatrix[i + radius];
                }
            }
            int cr = (int) (r / gaussSum);
            int cg = (int) (g / gaussSum);
            int cb = (int) (b / gaussSum);
            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }
    for (int x = 0; x < w; ++x) {
        for (int y = 0; y < h; ++y)
            listData[y] = pix[y * w + x];
        for (int y = 0; y < h; ++y) {
            float r = 0, g = 0, b = 0;
            gaussSum = 0;
            for (int j = -radius; j <= radius; ++j) {
                int k = y + j;
                if (0 <= k && k <= h) {
                    int color = listData[k];
                    int cr = (color & 0x00ff0000) >> 16;
                    int cg = (color & 0x0000ff00) >> 8;
                    int cb = (color & 0x000000ff);
                    r += cr * gaussMatrix[j + radius];
                    g += cg * gaussMatrix[j + radius];
                    b += cb * gaussMatrix[j + radius];
                    gaussSum += gaussMatrix[j + radius];
                }
            }
            int cr = (int) (r / gaussSum);
            int cg = (int) (g / gaussSum);
            int cb = (int) (b / gaussSum);
            pix[y * w + x] = cr << 16 | cg << 8 | cb | 0xff000000;
        }
    }
    free(gaussMatrix);
    free(rowData);
    free(listData);
}
