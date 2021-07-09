#ifndef _EYE_ALGO_H_
#define _EYE_ALGO_H_
#include <stdint.h>

#define BIT_ARRAY_LEN   (sizeof(bit_array_t) * 8)
#define MEAN(x,y)       (((x) & (y)) + (((x) ^ (y)) >> 1))
#define MIN(x,y)        ((x) < (y) ? (x) : (y))
#define MAX(x,y)        ((x) > (y) ? (x) : (y))
#define ROUND_UP(x,y)   (((x) + (y) - 1) / (y))

/** @brief Bit Array structure used for training results */
typedef uint64_t bit_array_t;

/**
 * @brief Coordinate Structure
 *
 * Represents an (x, y) coordinate pair. Used with 2-D bit arrays to describe
 * a particular index pair.
 *
 * @x   column position of bit array
 * @y   row position of bit array
 */
typedef struct coordinate_t
{
    int16_t x;
    int16_t y;
} coordinate_t;

/**
 * @brief   Coordinate Size Structure
 *
 * Represents a width and height size. Used with coorindate strucuture
 * to describe a rectangular area of a bit array
 *
 * @w   width
 * @h   height
 */
typedef struct coord_size_t
{
    uint8_t w;
    uint8_t h;
} coord_size_t;

/**
 * @brief Rect Structure
 *
 * Represents a rectangular region of a bit array. Used to describe the optimal
 * region of a bit result array.
 *
 * @origin   coordinate pair describing the top left corner of the rectangle
 * @size     width and height of the rectangle
 */
typedef struct rect_t
{
    coordinate_t origin;
    coord_size_t  size;
} rect_t;

/**
 * @brief   Range Structure
 *
 * Represents a particular range of values. Used for iteration when converting
 * from a bit array index into an actual value.
 *
 * @start       start value of the range
 * @stop        stop value of the range
 * @step_size   step size of each iteration of the range
 */
typedef struct range_t
{
    int16_t start;
    int16_t stop;
    int8_t step_size;
} range_t;


/**
 * @brief  Max Rectangle
 *
 * 2-D Eye search algorithm that return rectangle with maximum
 * figure of merit (FOM) within the given bit array.
 *
 * @param m         bit array of "eye" data to search
 * @param x_len     valid range of x
 * @param y_len     valid range of y
 * @param rect      in/out pointer to rectangle "eye" region
 *
 * @return      returns FOM of selected rect
 */
uint32_t max_rect(bit_array_t m[BIT_ARRAY_LEN], uint8_t x_len, uint8_t y_len, rect_t *rect);

/**
 * @brief    Get Window
 *
 * 1-D Eye search algorithm that finds maxmium length vector within the
 * given bit_array
 *
 * @param array     pointer to bit array to search
 * @param x_range   pointer to range describing array steps
 * @param cycles    number of "x cycles" to search in the array
 * @param rect      pointer to rect structure that describes maximum vector
 */

void get_window(bit_array_t *array, range_t *x_range, uint8_t cycles, rect_t *rect);

/**
 * @brief   Find Midpoint and Convert
 *
 * 1. Finds midpoint of the valid indices
 * 2. Converts midpoint to real value
 *
 * @param start     start index of valid indices
 * @param width     Number of indices that the valid indices span
 * @param range     Range for conversion
 *
 * @return          converted midpoint
 */
uint16_t find_midpoint_and_convert(int16_t start, uint8_t width, range_t *range);

#endif /* _EYE_ALGO_H_ */
