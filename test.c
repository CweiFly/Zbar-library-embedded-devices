#include "image.h"
#include "zbar.h"

typedef struct
{
    s16 x;
    s16 y;
} point;

void Zbar_Lib(const void *img_ptr, int width, int height, char *res_ptr, point *p_ptr)
{
    /* 创建扫描器 */
    zbar_image_scanner_t *scanner = NULL;
    scanner = zbar_image_scanner_create();
    zbar_image_scanner_set_config(scanner, 0, ZBAR_CFG_ENABLE, 1);
    /* 创建图像对象 */
    zbar_image_t *image = zbar_image_create();
    zbar_image_set_format(image, *(int *)"Y800");
    zbar_image_set_size(image, width, height);
    zbar_image_set_data(image, (void *)img_ptr, width * height, zbar_image_free_data);
    /* Z字形扫描图像像素点 */
    int n = zbar_scan_image(scanner, image);
    /* 条码或者二维码对象 */
    const zbar_symbol_t *symbol = zbar_image_first_symbol(image);
    for (; symbol; symbol = zbar_symbol_next(symbol))
    {
        unsigned int size = zbar_symbol_get_loc_size(symbol);  // 码的角个数
        zbar_symbol_type_t typ = zbar_symbol_get_type(symbol); // 码的对象类型
        const char *data = zbar_symbol_get_data(symbol);       // 码的数据
        if (size == 4)
        {
            point _pp[4];
            u8 index = 0;
            for (u8 i = 0; i < size; i++)
            {
                _pp[i].x = zbar_symbol_get_loc_x(symbol, i);
                _pp[i].y = zbar_symbol_get_loc_y(symbol, i);
            }
            p_ptr->x = ((_pp[0].x + _pp[2].x) >> 1);
            p_ptr->y = ((_pp[0].y + _pp[2].y) >> 1);
            for (u8 k = 0; k < strlen(data); k++)
            {
                if ((data[k] > 0x7E) || (data[k] < 0x20)) // 排除非字符的ASCII编码
                    continue;
                *(res_ptr + index) = data[k];
                index++;
            }
        }
    }

    /* 清除 */
    zbar_image_scanner_destroy(scanner);
    zbar_image_destroy(image);
}

int main(int argc, char **argv)
{
    // 根据自身情况进行传参
    Zbar_Lib(NULL, 640, 480, NULL, NULL);
    return 0;
}