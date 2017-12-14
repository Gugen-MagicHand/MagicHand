#ifndef FEATURE_POINT_FILTER
#define FEATURE_POINT_FILTER

const int END_POINT_FILTERS_COUNT = 8;
const int BRANCH_POINT_FILTERS_COUNT = 10;

static const uint8_t endPointFilters[END_POINT_FILTERS_COUNT][9] PROGMEM = {
    {
        0, 0, 0,
        0, 1, 0,
        2, 1, 2
    },
    {
        2, 0, 0,
        1, 1, 0,
        2, 0, 0
    },
    {
        2, 1, 2,
        0, 1, 0,
        0, 0, 0
    },
    {
        0, 0, 2,
        0, 1, 1,
        0, 0, 2
    },
    {
        0, 0, 0,
        0, 1, 0,
        1, 0, 0
    },
    {
        1, 0, 0,
        0, 1, 0,
        0, 0, 0
    },
    {
        0, 0, 1,
        0, 1, 0,
        0, 0, 0
    },
    {
        0, 0, 0,
        0, 1, 0,
        0, 0, 1
    }
};


static const uint8_t branchPointFilters[BRANCH_POINT_FILTERS_COUNT][9] PROGMEM = {
    {
        2, 1, 2,
        1, 1, 1,
        0, 0, 0
    },
    {
        0, 1, 2,
        0, 1, 1,
        0, 1, 2
    },
    {
        0, 0, 0,
        1, 1, 1,
        2, 1, 2
    },
    {
        2, 1, 0,
        1, 1, 0,
        2, 1, 0
    },
    {
        1, 0, 0,
        0, 1, 0,
        1, 0, 1
    },
    {
        1, 0, 1,
        0, 1, 0,
        1, 0, 0
    },
    {
        1, 0, 1,
        0, 1, 0,
        0, 0, 1
    },
    {
        0, 0, 1,
        0, 1, 0,
        1, 0, 1
    },
    {
        2, 1, 2,
        1, 1, 1,
        2, 1, 2
    },
    {
        1, 0, 1,
        0, 1, 0,
        1, 0, 1
    }
};







#endif