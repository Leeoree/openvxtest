#include <stdio.h>
#include<windows.h>
#include "../ref.h"

vx_status ref_FindContours(const vx_image src_image, vx_image dst_image)
{
    const uint32_t src_width = src_image->width;
    const uint32_t src_height = src_image->height;
    const uint32_t dst_width = dst_image->width;
    const uint32_t dst_height = dst_image->height;

    if (src_width != dst_width || src_height != dst_height || src_height < 3 || src_width < 3)
    {
        return VX_ERROR_INVALID_PARAMETERS;
    }

    const uint8_t* src_data = src_image->data;
    uint8_t* dst_data = dst_image->data;
    uint8_t value = 127;

    for (uint32_t ind = 0; ind < src_width * src_height; ++ind)
    {
        dst_data[ind] = src_data[ind];
         if (src_data[ind] > value && ind / src_width != 0 && ind / src_width != src_width - 1 && ind % src_width != 0 && ind % src_width != src_width - 1)
         {
            dst_data[ind] = 127;
         }
         else
         {
            dst_data[ind] = 0;
         }
    }


    int32_t b0 = -1;
    int32_t b1 = -2;
    int32_t b = -4;
    int32_t c = -5;


    // Search for the first non-zero pixel

    for (uint32_t ind = 0; ind <= dst_height && b0 == -1; ind++)
    {
        // Horizontal rectangle case
        if (ind == dst_height && dst_height != dst_width)
        {
            for (uint32_t column_right = ind; column_right < dst_width && b0 == -1; column_right++)
            {
                for (uint32_t ind_vertical = 0; ind_vertical < dst_height && b0 == -1; ind_vertical++)
                {
                    if (dst_data[ind_vertical * dst_width + column_right] != 0)
                    {
                        b0 = ind_vertical * dst_width + column_right;
                    }
                }

            }
        }
        // Square case & vertical rectangle case
        else
        {
            for (uint32_t jnd = 0; jnd < dst_width && b0 == -1; jnd++)
            {
                // Horizontal side of the right angle
                if (jnd < ind)
                {
                    if (dst_data[ind * dst_width + jnd] != 0)
                    {
                        b0 = ind * dst_width + jnd;
                    }
                }
                // Vertical side of the right angle
                else
                {
                    for (uint32_t string_down = 0; string_down <= ind && b0 == -1; string_down++)
                    {
                        if (dst_data[string_down * dst_width + jnd] != 0)
                        {
                            b0 = string_down * dst_width + jnd;
                        }
                    }
                }
            }
        }
    }

    printf("%d %d %d\n", b0, dst_width, dst_height);
    dst_data[b0] = 255;
    c = b0 - 1;

    //Contours builder
    /*
    * 2  3  4
    * 1     5
    * 8  7  6
    */

    while (b != b0 && c != b1)
    {
        if (b1 == -2)
        {
            if(dst_data[c] != 0)
            {
                b1 = c; c += dst_width; b = b1;
            }
            else
            {
                c -= dst_width;
                if (dst_data[c] != 0)
                {
                    b1 = c; c += dst_width; b = b1;
                }
                else
                {
                    c += 1;
                    if (dst_data[c] != 0)
                    {
                        b1 = c; c -= 1; b = b1;
                    }
                    else
                    {
                        c += 1;
                        if (dst_data[c] != 0)
                        {
                            b1 = c; c -= 1; b = b1;
                        }
                        else
                        {
                            c += dst_width;
                            if (dst_data[c] != 0)
                            {
                                b1 = c; c -= dst_width; b = b1;
                            }
                            else
                            {
                                c += dst_width;
                                if (dst_data[c] != 0)
                                {
                                    b1 = c; c -= dst_width; b = b1;
                                }
                                else
                                {
                                    c -= 1;
                                    if (dst_data[c] != 0)
                                    {
                                        b1 = c; c += 1; b = b1;
                                    }
                                    else
                                    {
                                        c -= 1;
                                        if (dst_data[c] != 0)
                                        {
                                            b1 = c; c += 1; b = b1;
                                        }
                                        else
                                        {
                                            return VX_SUCCESS;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }

        else
        {
            //1
            if (c == b - 1)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c += dst_width;
                }
                else
                {
                    c -= dst_width;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c += dst_width;
                    }
                    else
                    {
                        c += 1;
                        if (dst_data[c] != 0)
                        {
                            b = c; c -= 1;
                        }
                        else
                        {
                            c += 1;
                            if (dst_data[c] != 0)
                            {
                                b = c; c -= 1;
                            }
                            else
                            {
                                c += dst_width;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c -= dst_width;
                                }
                                else
                                {
                                    c += dst_width;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c -= dst_width;
                                    }
                                    else
                                    {
                                        c -= 1;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c += 1;
                                        }
                                        else
                                        {
                                            c -= 1;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c += 1;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //2
            if (c == b - 1 - dst_width)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c += dst_width;
                }
                else
                {
                    c += 1;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c -= 1;
                    }
                    else
                    {
                        c += 1;
                        if (dst_data[c] != 0)
                        {
                            b = c; c -= 1;
                        }
                        else
                        {
                            c += dst_width;
                            if (dst_data[c] != 0)
                            {
                                b = c; c -= dst_width;
                            }
                            else
                            {
                                c += dst_width;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c -= dst_width;
                                }
                                else
                                {
                                    c -= 1;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c += 1;
                                    }
                                    else
                                    {
                                        c -= 1;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c += 1;
                                        }
                                        else
                                        {
                                            c -= dst_width;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c += dst_width;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //3
            if (c == b - dst_width)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c -= 1;
                }
                else
                {
                    c += 1;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c -= 1;
                    }
                    else
                    {
                        c += dst_width;
                        if (dst_data[c] != 0)
                        {
                            b = c; c -= dst_width;
                        }
                        else
                        {
                            c += dst_width;
                            if (dst_data[c] != 0)
                            {
                                b = c; c -= dst_width;
                            }
                            else
                            {
                                c -= 1;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c += 1;
                                }
                                else
                                {
                                    c -= 1;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c += 1;
                                    }
                                    else
                                    {
                                        c -= dst_width;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c += dst_width;
                                        }
                                        else
                                        {
                                            c -= dst_width;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c += dst_width;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //4
            if (c == b + 1 - dst_width)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c -= 1;
                }
                else
                {
                    c += dst_width;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c -= dst_width;
                    }
                    else
                    {
                        c += dst_width;
                        if (dst_data[c] != 0)
                        {
                            b = c; c -= dst_width;
                        }
                        else
                        {
                            c -= 1;
                            if (dst_data[c] != 0)
                            {
                                b = c; c += 1;
                            }
                            else
                            {
                                c -= 1;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c += 1;
                                }
                                else
                                {
                                    c -= dst_width;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c += dst_width;
                                    }
                                    else
                                    {
                                        c -= dst_width;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c += dst_width;
                                        }
                                        else
                                        {
                                            c += 1;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c -= 1;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //5
            if (c == b + 1)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c -= dst_width;
                }
                else
                {
                    c += dst_width;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c -= dst_width;
                    }
                    else
                    {
                        c -= 1;
                        if (dst_data[c] != 0)
                        {
                            b = c; c += 1;
                        }
                        else
                        {
                            c -= 1;
                            if (dst_data[c] != 0)
                            {
                                b = c; c += 1;
                            }
                            else
                            {
                                c -= dst_width;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c += dst_width;
                                }
                                else
                                {
                                    c -= dst_width;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c += dst_width;
                                    }
                                    else
                                    {
                                        c += 1;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c -= 1;
                                        }
                                        else
                                        {
                                            c += 1;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c -= 1;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //6
            if (c == b + 1 + dst_width)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c -= dst_width;
                }
                else
                {
                    c -= 1;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c += 1;
                    }
                    else
                    {
                        c -= 1;
                        if (dst_data[c] != 0)
                        {
                            b = c; c += 1;
                        }
                        else
                        {
                            c -= dst_width;
                            if (dst_data[c] != 0)
                            {
                                b = c; c += dst_width;
                            }
                            else
                            {
                                c -= dst_width;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c += dst_width;
                                }
                                else
                                {
                                    c += 1;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c -= 1;
                                    }
                                    else
                                    {
                                        c += 1;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c -= 1;
                                        }
                                        else
                                        {
                                            c += dst_width;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c -= dst_width;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //7
            if (c == b + dst_width)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c -= 1;
                }
                else
                {
                    c -= 1;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c += 1;
                    }
                    else
                    {
                        c -= dst_width;
                        if (dst_data[c] != 0)
                        {
                            b = c; c += dst_width;
                        }
                        else
                        {
                            c -= dst_width;
                            if (dst_data[c] != 0)
                            {
                                b = c; c += dst_width;
                            }
                            else
                            {
                                c += 1;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c -= 1;
                                }
                                else
                                {
                                    c += 1;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c -= 1;
                                    }
                                    else
                                    {
                                        c += dst_width;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c -= dst_width;
                                        }
                                        else
                                        {
                                            c += dst_width;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c -= dst_width;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }

            //8
            if (c == b - 1 + dst_width)
            {
                if (dst_data[c] != 0)
                {
                    b = c; c -= 1;
                }
                else
                {
                    c -= dst_width;
                    if (dst_data[c] != 0)
                    {
                        b = c;  c += dst_width;
                    }
                    else
                    {
                        c -= dst_width;
                        if (dst_data[c] != 0)
                        {
                            b = c; c += dst_width;
                        }
                        else
                        {
                            c += 1;
                            if (dst_data[c] != 0)
                            {
                                b = c; c -= 1;
                            }
                            else
                            {
                                c += 1;
                                if (dst_data[c] != 0)
                                {
                                    b = c; c -= 1;
                                }
                                else
                                {
                                    c += dst_width;
                                    if (dst_data[c] != 0)
                                    {
                                        b = c; c -= dst_width;
                                    }
                                    else
                                    {
                                        c += dst_width;
                                        if (dst_data[c] != 0)
                                        {
                                            b = c; c -= dst_width;
                                        }
                                        else
                                        {
                                            c -= 1;
                                            if (dst_data[c] != 0)
                                            {
                                                b = c; c += 1;
                                            }
                                            else
                                            {
                                                return VX_SUCCESS;
                                            }
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }


        dst_data[b] = 255;
        
        //printf("%d %d\n", b, c);
        //Sleep(2000);
    }

    return VX_SUCCESS;
}