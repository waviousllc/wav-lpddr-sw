#include <string.h>
#include <wddr/eye_algo.h>

#define ALPHA   (1)
#define BETA    (2)

/**
 * @brief  Calculate MAX FOM
 *
 * Finds the rectangle with the largest FOM from a given start position.
 *
 * NOTE: Rectangular notation: Rect(x, y, w, h)
 * NOTE: Size is inclusive of origin point
 * NOTE: Minimum height and width of any valid rectangle is (1, 1)
 * NOTE: If no rectangles are found, Rect(x, y, 0, 0) is returned
 *
 * A valid rectangle is any region of contiguous 1s without any holes. For
 * example, the rectangle described as Rect(1, 0, 5, 4) in the figure below is
 * not valid since there is a hole at coordinate (1, 5). However,
 * Rect(1, 0, 4, 4) is valid.
 *
 *  2-D Eye Example:
 *      0   1   2   3   4   5   6   7   8 ...   63
 * 0    0   1   1   1   1   1   0   0   0 ...   0
 * 1    0   1   1   1   1   0   0   0   0 ...   0
 * 2    0   1   1   1   1   1   1   0   0 ...   0
 * 3    0   1   1   1   1   1   1   0   0 ...   0
 * .                                     .      .
 * .                                       .    .
 * .                                         .  .
 * 63   0   0   0   0   0   0   0   0   0 ...   0
 *
 *
 * Algorithm description:
 * 1. Starting at (x, y) scan from x -> BIT_ARRAY_LEN until a 0 is encountered;
 *    This column positions is c.
 * 2. min_c = min(min_c, c)
 * 3. if min_c == x + 1; terminate search and return maximum figure of merit
 * 4. Compute fom of the current rectangle. Rect(x, y, min_c - x - 1,  r - y + 1)
 *      FOM = w * alpha + h * beta
 * 5. If FOM > MAX_FOM found, update MAX_FOM = FOM; Update W, H
 * 6. Repeat 1 - 5 for range y -> BIT_ARRAY_LEN until terminates
 *
 * Terminating conditions:
 *  1. Exhaust entire range y -> BIT_ARRAY_LEN
 *  2. Reach condition where min_c == x + 1, thus max FOM already found.
 *
 * @param m     2-D bit array representing "eye" to process
 * @param rect  structure used to describe rectangle with largest FOM
 *              origin is expected to indicate the start position of the search
 * @param alpha Alpha parameter of the FOM function
 * @param beta  Beta parameter of the FOM function
 *
 * @return      Maximum Figure of Merit (FOM)
 */
static uint32_t calculate_max_fom(bit_array_t m[BIT_ARRAY_LEN], rect_t *rect, uint8_t alpha, uint8_t beta)
{
    int16_t x = rect->origin.x;
    int16_t y = rect->origin.y;

    if (y >= (int16_t)BIT_ARRAY_LEN ||
        x >= (int16_t)BIT_ARRAY_LEN)
    {
        return 0;
    }

    /** NOTE: Could add pre-search to make this faster */
    int16_t c, min_c = (int16_t) BIT_ARRAY_LEN;
    uint32_t max_fom = 0, tmp_fom = 0;
    for (int16_t r = y; r < (int16_t) BIT_ARRAY_LEN; r++)
    {
        bit_array_t row = m[r] >> x;
        for (c = x; c < (int16_t) BIT_ARRAY_LEN; c++)
        {
            if (!(row & 0x1))
            {
                break;
            }
            row >>= 1;
        }

        min_c = MIN(c + 1, min_c);

        if (min_c == x + 1)
        {
            break;
        }

        tmp_fom = alpha * (min_c - x - 1) + beta * (r - y + 1);
        if (tmp_fom > max_fom)
        {
            max_fom = tmp_fom;
            rect->size.w = min_c - x - 1;
            rect->size.h = r - y + 1;
        }
    }

    return max_fom;
}

/** TODO: Integrate Alpha and Beta as parameters */
uint32_t max_rect(bit_array_t m[BIT_ARRAY_LEN], uint8_t x_len, uint8_t y_len, rect_t *rect)
{
    uint8_t alpha = ALPHA, beta = BETA;
    uint32_t max_fom = 0;
    rect_t tmp_rect;

    memset(rect, 0, sizeof(rect_t));

    /**
     * Find the highest FOM for all possible start positions
     */
    for (uint8_t y = 0; y < y_len; y++)
    {
        for (uint8_t x = 0; x < x_len; x++)
        {
            tmp_rect.origin.x = x;
            tmp_rect.origin.y = y;
            uint32_t fom = calculate_max_fom(m, &tmp_rect, alpha, beta);
            if (fom > max_fom)
            {
                max_fom = fom;
                memcpy(rect, &tmp_rect, sizeof(rect_t));
            }
        }
    }

    return max_fom;
}

void get_window(bit_array_t *array, range_t *x_range, uint8_t cycles, rect_t *rect)
{
    uint16_t curr_len = 0, max_len = 0, start = 0;
    uint8_t i = 0;

    memset(rect, 0, sizeof(rect_t));

    for (uint8_t cycle = 0; cycle < cycles; cycle++)
    {
        bit_array_t val = *(array++);
        for (int16_t x = x_range->start; x < x_range->stop; x += x_range->step_size, i++)
        {
            if (val & 0x1)
            {
                curr_len++;
                if (curr_len > max_len)
                {
                    max_len = curr_len;
                    rect->origin.x = start;
                    rect->size.w = curr_len;
                }
            }
            else
            {
                start = i + 1;
                curr_len = 0;
            }
            val >>= 1;
        }
    }
}

uint16_t find_midpoint_and_convert(int16_t start, uint8_t width, range_t *range)
{
    return ((start + start + (width - 1)) / 2) * range->step_size + range->start;
}
