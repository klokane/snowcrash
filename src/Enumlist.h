#ifndef SNOWCRASH_ENUMLIST_H
#define SNOWCRASH_ENUMLIST_H

namespace snowcrash {
    template<
        int  T0 = -1,
        int  T1 = -1,
        int  T2 = -1,
        int  T3 = -1,
        int  T4 = -1,
        int  T5 = -1,
        int  T6 = -1,
        int  T7 = -1,
        int  T8 = -1,
        int  T9 = -1,
        int T10 = -1,
        int T11 = -1,
        int T12 = -1,
        int T13 = -1,
        int T14 = -1,
        int T15 = -1,
        int T16 = -1,
        int T17 = -1,
        int T18 = -1,
        int T19 = -1,
        int T20 = -1,
        int T21 = -1,
        int T22 = -1,
        int T23 = -1,
        int T24 = -1,
        int T25 = -1
    > struct EnumList;

    template<
        int  T0,
        int  T1,
        int  T2,
        int  T3,
        int  T4,
        int  T5,
        int  T6,
        int  T7,
        int  T8,
        int  T9,
        int T10,
        int T11,
        int T12,
        int T13,
        int T14,
        int T15,
        int T16,
        int T17,
        int T18,
        int T19,
        int T20,
        int T21,
        int T22,
        int T23,
        int T24,
        int T25
    > struct EnumList {
        enum { head = T0 };
        typedef EnumList<
             T1,  T2,  T3,  T4, T5,
             T6,  T7,  T8,  T9, T10,
            T11, T12, T13, T14, T15,
            T16, T17, T18, T19, T20,
            T21, T22, T23, T24, T25> tail;
        enum { length = tail::length + 1 };
    };

    template<> struct EnumList<
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1,
        -1, -1, -1, -1, -1
        >{
        enum  { length = 0 };
    };
}

#endif
