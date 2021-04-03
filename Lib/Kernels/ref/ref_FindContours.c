
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
         if (src_data[ind] > value || ind / src_width == 0 || ind / src_width == src_width - 1 || )
         {
            dst_data[ind] = 0;
         }
         else
         {
            dst_data[ind] = 127;
         }
    }

    
    int32_t b0 = -1;
    int32_t b1 = -1;
    int32_t b = -1;
    int32_t c = -1;


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

    while (b != b0 && c != b1)
    {
        // First iteration
        if (b1 == -1)
        {
            c = b0 - 1;

            // b0 is upper left corner -> c is right to it
            if(c == -1)
            { 
                c = b0 + 1;

                if (dst_data[c] != 0)
                {
                    b1 = c;
                    c = b1 + 1;
                }
                else
                {
                    c = c + dst_width;
                    if (dst_data[c] != 0)
                    {
                        b1 = c;
                        c = c - dst_width;
                    }
                    else
                    {
                        c = c - 1;
                        if (dst_data[c] != 0)
                        {
                            b1 = c;
                            c = c + 1;
                        }
                        // Isolated pixel -> stop searching
                        else
                        {
                            break;
                        }
                    }
                }
            }
            else
            {
                // b0 is on the left border -> c is up to it
                if (c / dst_width != b0 / dst_width)
                {
                    c = b0 - dst_width;

                    if (dst_data[c] != 0)
                    {
                        b1 = c;
                        b = b1;
                        c = b1 - dst_width;
                    }
                    else
                    {
                        c = c + 1;
                        if (dst_data[c] != 0)
                        {
                            b1 = c;
                            b = b1;
                            c = c - 1;
                        }
                        else
                        {
                            c = c + dst_width;
                            if (dst_data[c] != 0)
                            {
                                b1 = c;
                                b = b1;
                                c = c - dst_width;
                            }
                            else
                            {
                                c = c + dst_width;
                                if (dst_data[c] != 0)
                                {
                                    b1 = c;
                                    b = b1;
                                    c = c - dst_width;
                                }
                                else
                                {
                                    c = c - 1;
                                    if (dst_data[c] != 0)
                                    {
                                        b1 = c;
                                        b = b1;
                                        c = c + 1;
                                    }
                                    // Isolated pixel -> stop searching
                                    else
                                    {
                                        break;
                                    }
                                }
                            }
                        }
                    }
                }
                // c is left to b0 
                else
                {
                    if (dst_data[c] != 0)
                    {
                        b1 = c;
                        b = b1;
                        c = c + dst_width;
                    }
                    else
                    {
                        c = c - dst_width;
                        if (dst_data[c] != 0)
                        {
                            b1 = c;
                            b = b1;
                            c = c + dst_width;
                        }
                        else
                        {
                            c = c + 1;
                            if (dst_data[c] != 0)
                            {
                                b1 = c;
                                b = b1;
                                c = c - 1;
                            }
                            else
                            {
                                c = c + 1;
                                if (dst_data[c] != 0)
                                {
                                    b1 = c;
                                    b = b1;
                                    c = c - 1;
                                }
                                else
                                {
                                    c = c + dst_width;
                                    if (dst_data[c] != 0)
                                    {
                                        b1 = c;
                                        b = b1;
                                        c = c - dst_width;
                                    }
                                    else
                                    {
                                        c = c + dst_width;
                                        if (dst_data[c] != 0)
                                        {
                                            b1 = c;
                                            b = b1;
                                            c = c - dst_width;
                                        }
                                        else
                                        {
                                            c = c - 1;
                                            if (dst_data[c] != 0)
                                            {
                                                b1 = c;
                                                b = b1;
                                                c = c + 1;
                                            }
                                            else
                                            {
                                                c = c - 1;
                                                if (dst_data[c] != 0)
                                                {
                                                    b1 = c;
                                                    b = b1;
                                                    c = c + 1;
                                                }
                                                // Isolated pixel -> stop searching
                                                else
                                                {
                                                    break;
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

        }

        // Core iterations
        else
        {
            if (dst_data[c] != 0)
            {
                dst_data[c] = 255;
                b = c;
                c = c + dst_width;
            }
            else
            {
                c = c - dst_width;
                if (dst_data[c] != 0)
                {
                    dst_data[c] = 255;
                    b = c;
                    c = c + dst_width;
                }
                else
                {
                    c = c + 1;
                    if (dst_data[c] != 0)
                    {
                        dst_data[c] = 255;
                        b = c;
                        c = c - 1;
                    }
                    else
                    {
                        c = c + 1;
                        if (dst_data[c] != 0)
                        {
                            dst_data[c] = 255;
                            b = c;
                            c = c - 1;
                        }
                        else
                        {
                            c = c + dst_width;
                            if (dst_data[c] != 0)
                            {
                                dst_data[c] = 255;
                                b = c;
                                c = c - dst_width;
                            }
                            else
                            {
                                c = c + dst_width;
                                if (dst_data[c] != 0)
                                {
                                    dst_data[c] = 255;
                                    b = c;
                                    c = c - dst_width;
                                }
                                else
                                {
                                    c = c - 1;
                                    if (dst_data[c] != 0)
                                    {
                                        dst_data[c] = 255;
                                        b = c;
                                        c = c + 1;
                                    }
                                    else
                                    {
                                        c = c - 1;
                                        if (dst_data[c] != 0)
                                        {
                                            dst_data[c] = 255;
                                            b = c;
                                            c = c + 1;
                                        }
                                        // Isolated pixel -> stop searching
                                        else
                                        {
                                            break;
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

    return VX_SUCCESS;
}