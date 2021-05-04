#include "stdio.h"
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
            dst_data[ind] = value;
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
    uint32_t count;
    // contours_count = 0;

    uint32_t min_height;
    uint32_t max_height;
    uint32_t min_width;
    uint32_t max_width;

    //uint32_t* contours = malloc(sizeof(uint32_t) * dst_height * dst_width);

    while (true)
    {
        b0 = -1;
        b1 = -2;
        b = -4;
        c = -5;

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
                        if (dst_data[ind_vertical * dst_width + column_right] == value)
                        {
                            b0 = ind_vertical * dst_width + column_right;
                        }
                    }

                }
            }
            // Square case & vertical rectangle case
            else
            {
                for (uint32_t jnd = 0; jnd <= ind && b0 == -1; jnd++)
                {
                    // Horizontal side of the right angle
                    if (jnd < ind)
                    {
                        if (dst_data[ind * dst_width + jnd] == value)
                        {
                            b0 = ind * dst_width + jnd;
                        }
                    }
                    // Vertical side of the right angle
                    else
                    {
                        for (uint32_t string_down = 0; string_down <= ind && b0 == -1; string_down++)
                        {
                            if (dst_data[string_down * dst_width + jnd] == value)
                            {
                                b0 = string_down * dst_width + jnd;
                            }
                        }
                    }
                }
            }
        }    

        if (b0 != -1)
        {
            dst_data[b0] = 255;
            c = b0 - 1;
        }
        else
            break;

        


        min_height = b0 / dst_width;
        max_height = b0 / dst_width;
        min_width = b0 % dst_width;
        max_width = b0 % dst_width;

        //Contours builder
        /*
        * 2  3  4
        * 1     5
        * 8  7  6
        */

        count = 0;

        while (b != b0 && c != b1 && count < dst_width * dst_height)
        {
            count += 1;


            if (b1 == -2)
            {
                //printf("%d %d\n", dst_data[b0], dst_data[c]);
                
                if (dst_data[c] != 0)
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
                                                break;
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
                                                    continue;
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
                                                    continue;
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
                                                    continue;
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
                                                    continue;
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
                                                    continue;
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
                                                    continue;
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
                                                    continue;
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
                                                    continue;
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



            if (b != -2)
            {
                dst_data[b] = 255;

                if (b / dst_width < min_height)
                    min_height = b / dst_width;
                else
                    if (b / dst_width > max_height)
                        max_height = b / dst_width;

                if (b % dst_width < min_width)
                    min_width = b % dst_width;
                else
                    if (b % dst_width > max_width)
                        max_width = b % dst_width;
            }
            else
                break;
        }

        for (uint32_t j = 0, starting_point = min_height * dst_width + min_width; j < max_height - min_height + 1; j++)
            for (uint32_t i = 0; i < max_width - min_width + 1; i++)
                if (dst_data[starting_point + j * dst_width + i] != 255)
                    dst_data[starting_point + j * dst_width + i] = 0;
    }
    return VX_SUCCESS;
}