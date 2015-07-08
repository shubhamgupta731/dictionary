////////////////////////////////////////////////////////////////////////////////
// The Loki Library
// Copyright (c) 2001 by Andrei Alexandrescu
// This code accompanies the book:
// Alexandrescu, Andrei. "Modern C++ Design: Generic Programming and Design 
//     Patterns Applied". Copyright (c) 2001. Addison-Wesley.
// Permission to use, copy, modify, distribute and sell this software for any 
//     purpose is hereby granted without fee, provided that the above copyright 
//     notice appear in all copies and that both that copyright notice and this 
//     permission notice appear in supporting documentation.
// The author or Addison-Welsey Longman make no representations about the 
//     suitability of this software for any purpose. It is provided "as is" 
//     without express or implied warranty.
////////////////////////////////////////////////////////////////////////////////
#ifndef LOKI_TYPELISTMACROS_INC_
#define LOKI_TYPELISTMACROS_INC_

// $Id: TypelistMacros.h 749 2006-10-17 19:49:26Z syntheticpp $


//#define LOKI_DISABLE_TYPELIST_MACROS
#ifndef LOKI_DISABLE_TYPELIST_MACROS

////////////////////////////////////////////////////////////////////////////////
// macros LOKI_TYPELIST_1, LOKI_TYPELIST_2, ... LOKI_TYPELIST_50
// Each takes a number of arguments equal to its numeric suffix
// The arguments are type names. LOKI_TYPELIST_NN generates a typelist containing 
//     all types passed as arguments, in that order.
// Example: LOKI_TYPELIST_2(char, int) generates a type containing char and int.
////////////////////////////////////////////////////////////////////////////////

#define LOKI_TYPELIST_1(T1) ::Loki::Typelist<T1, ::Loki::NullType>

#define LOKI_TYPELIST_2(T1, T2) ::Loki::Typelist<T1, LOKI_TYPELIST_1(T2) >

#define LOKI_TYPELIST_3(T1, T2, T3) ::Loki::Typelist<T1, LOKI_TYPELIST_2(T2, T3) >

#define LOKI_TYPELIST_4(T1, T2, T3, T4) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_3(T2, T3, T4) >

#define LOKI_TYPELIST_5(T1, T2, T3, T4, T5) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_4(T2, T3, T4, T5) >

#define LOKI_TYPELIST_6(T1, T2, T3, T4, T5, T6) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_5(T2, T3, T4, T5, T6) >

#define LOKI_TYPELIST_7(T1, T2, T3, T4, T5, T6, T7) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_6(T2, T3, T4, T5, T6, T7) >

#define LOKI_TYPELIST_8(T1, T2, T3, T4, T5, T6, T7, T8) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_7(T2, T3, T4, T5, T6, T7, T8) >

#define LOKI_TYPELIST_9(T1, T2, T3, T4, T5, T6, T7, T8, T9) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_8(T2, T3, T4, T5, T6, T7, T8, T9) >

#define LOKI_TYPELIST_10(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_9(T2, T3, T4, T5, T6, T7, T8, T9, T10) >

#define LOKI_TYPELIST_11(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_10(T2, T3, T4, T5, T6, T7, T8, T9, T10, T11) >

#define LOKI_TYPELIST_12(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_11(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12) >

#define LOKI_TYPELIST_13(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, T11, T12, T13) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_12(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13) >

#define LOKI_TYPELIST_14(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_13(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14) >

#define LOKI_TYPELIST_15(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_14(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15) >

#define LOKI_TYPELIST_16(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_15(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16) >

#define LOKI_TYPELIST_17(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_16(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17) >

#define LOKI_TYPELIST_18(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_17(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18) >

#define LOKI_TYPELIST_19(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_18(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19) >

#define LOKI_TYPELIST_20(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_19(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20) >

#define LOKI_TYPELIST_21(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_20(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21) >

#define LOKI_TYPELIST_22(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_21(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22) >

#define LOKI_TYPELIST_23(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_22(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23) >

#define LOKI_TYPELIST_24(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_23(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24) >

#define LOKI_TYPELIST_25(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, T21, T22, T23, T24, T25) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_24(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25) >

#define LOKI_TYPELIST_26(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_25(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26) >

#define LOKI_TYPELIST_27(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_26(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27) >

#define LOKI_TYPELIST_28(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_27(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28) >

#define LOKI_TYPELIST_29(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_28(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29) >

#define LOKI_TYPELIST_30(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_29(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30) >

#define LOKI_TYPELIST_31(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_30(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31) >

#define LOKI_TYPELIST_32(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_31(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32) >

#define LOKI_TYPELIST_33(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_32(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33) >

#define LOKI_TYPELIST_34(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_33(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, T31, T32, T33, T34) >

#define LOKI_TYPELIST_35(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_34(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35) >

#define LOKI_TYPELIST_36(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_35(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36) >

#define LOKI_TYPELIST_37(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_36(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37) >

#define LOKI_TYPELIST_38(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_37(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38) >

#define LOKI_TYPELIST_39(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_38(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39) >

#define LOKI_TYPELIST_40(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_39(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40) >

#define LOKI_TYPELIST_41(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_40(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41) >

#define LOKI_TYPELIST_42(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_41(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42) >

#define LOKI_TYPELIST_43(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_42(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43) >

#define LOKI_TYPELIST_44(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_43(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, T41, T42, T43, T44) >

#define LOKI_TYPELIST_45(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_44(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45) >

#define LOKI_TYPELIST_46(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_45(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46) >

#define LOKI_TYPELIST_47(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_46(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47) >

#define LOKI_TYPELIST_48(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_47(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48) >

#define LOKI_TYPELIST_49(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_48(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49) >

#define LOKI_TYPELIST_50(T1, T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) \
    ::Loki::Typelist<T1, LOKI_TYPELIST_49(T2, T3, T4, T5, T6, T7, T8, T9, T10, \
        T11, T12, T13, T14, T15, T16, T17, T18, T19, T20, \
        T21, T22, T23, T24, T25, T26, T27, T28, T29, T30, \
        T31, T32, T33, T34, T35, T36, T37, T38, T39, T40, \
        T41, T42, T43, T44, T45, T46, T47, T48, T49, T50) >

#define LOKI_TYPELIST_50_(tuple) LOKI_TYPELIST_50  tuple
#define LOKI_TYPELIST_51_(tuple) LOKI_TYPELIST_51  tuple
#define LOKI_TYPELIST_52_(tuple) LOKI_TYPELIST_52  tuple
#define LOKI_TYPELIST_53_(tuple) LOKI_TYPELIST_53  tuple
#define LOKI_TYPELIST_54_(tuple) LOKI_TYPELIST_54  tuple
#define LOKI_TYPELIST_55_(tuple) LOKI_TYPELIST_55  tuple
#define LOKI_TYPELIST_56_(tuple) LOKI_TYPELIST_56  tuple
#define LOKI_TYPELIST_57_(tuple) LOKI_TYPELIST_57  tuple
#define LOKI_TYPELIST_58_(tuple) LOKI_TYPELIST_58  tuple
#define LOKI_TYPELIST_59_(tuple) LOKI_TYPELIST_59  tuple
#define LOKI_TYPELIST_60_(tuple) LOKI_TYPELIST_60  tuple
#define LOKI_TYPELIST_61_(tuple) LOKI_TYPELIST_61  tuple
#define LOKI_TYPELIST_62_(tuple) LOKI_TYPELIST_62  tuple
#define LOKI_TYPELIST_63_(tuple) LOKI_TYPELIST_63  tuple
#define LOKI_TYPELIST_64_(tuple) LOKI_TYPELIST_64  tuple
#define LOKI_TYPELIST_65_(tuple) LOKI_TYPELIST_65  tuple
#define LOKI_TYPELIST_66_(tuple) LOKI_TYPELIST_66  tuple
#define LOKI_TYPELIST_67_(tuple) LOKI_TYPELIST_67  tuple
#define LOKI_TYPELIST_68_(tuple) LOKI_TYPELIST_68  tuple
#define LOKI_TYPELIST_69_(tuple) LOKI_TYPELIST_69  tuple
#define LOKI_TYPELIST_70_(tuple) LOKI_TYPELIST_70  tuple
#define LOKI_TYPELIST_71_(tuple) LOKI_TYPELIST_71  tuple
#define LOKI_TYPELIST_72_(tuple) LOKI_TYPELIST_72  tuple
#define LOKI_TYPELIST_73_(tuple) LOKI_TYPELIST_73  tuple
#define LOKI_TYPELIST_74_(tuple) LOKI_TYPELIST_74  tuple
#define LOKI_TYPELIST_75_(tuple) LOKI_TYPELIST_75  tuple
#define LOKI_TYPELIST_76_(tuple) LOKI_TYPELIST_76  tuple
#define LOKI_TYPELIST_77_(tuple) LOKI_TYPELIST_77  tuple
#define LOKI_TYPELIST_78_(tuple) LOKI_TYPELIST_78  tuple
#define LOKI_TYPELIST_79_(tuple) LOKI_TYPELIST_79  tuple
#define LOKI_TYPELIST_80_(tuple) LOKI_TYPELIST_80  tuple
#define LOKI_TYPELIST_81_(tuple) LOKI_TYPELIST_81  tuple
#define LOKI_TYPELIST_82_(tuple) LOKI_TYPELIST_82  tuple
#define LOKI_TYPELIST_83_(tuple) LOKI_TYPELIST_83  tuple
#define LOKI_TYPELIST_84_(tuple) LOKI_TYPELIST_84  tuple
#define LOKI_TYPELIST_85_(tuple) LOKI_TYPELIST_85  tuple
#define LOKI_TYPELIST_86_(tuple) LOKI_TYPELIST_86  tuple
#define LOKI_TYPELIST_87_(tuple) LOKI_TYPELIST_87  tuple
#define LOKI_TYPELIST_88_(tuple) LOKI_TYPELIST_88  tuple
#define LOKI_TYPELIST_89_(tuple) LOKI_TYPELIST_89  tuple
#define LOKI_TYPELIST_90_(tuple) LOKI_TYPELIST_90  tuple
#define LOKI_TYPELIST_91_(tuple) LOKI_TYPELIST_91  tuple
#define LOKI_TYPELIST_92_(tuple) LOKI_TYPELIST_92  tuple
#define LOKI_TYPELIST_93_(tuple) LOKI_TYPELIST_93  tuple
#define LOKI_TYPELIST_94_(tuple) LOKI_TYPELIST_94  tuple
#define LOKI_TYPELIST_95_(tuple) LOKI_TYPELIST_95  tuple
#define LOKI_TYPELIST_96_(tuple) LOKI_TYPELIST_96  tuple
#define LOKI_TYPELIST_97_(tuple) LOKI_TYPELIST_97  tuple
#define LOKI_TYPELIST_98_(tuple) LOKI_TYPELIST_98  tuple
#define LOKI_TYPELIST_99_(tuple) LOKI_TYPELIST_99  tuple
#define LOKI_TYPELIST_100_(tuple) LOKI_TYPELIST_100 tuple

#define LOKI_TYPELIST_51(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_50_((__VA_ARGS__)) >
#define LOKI_TYPELIST_52(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_51_((__VA_ARGS__)) >
#define LOKI_TYPELIST_53(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_52_((__VA_ARGS__)) >
#define LOKI_TYPELIST_54(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_53_((__VA_ARGS__)) >
#define LOKI_TYPELIST_55(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_54_((__VA_ARGS__)) >
#define LOKI_TYPELIST_56(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_55_((__VA_ARGS__)) >
#define LOKI_TYPELIST_57(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_56_((__VA_ARGS__)) >
#define LOKI_TYPELIST_58(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_57_((__VA_ARGS__)) >
#define LOKI_TYPELIST_59(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_58_((__VA_ARGS__)) >
#define LOKI_TYPELIST_60(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_59_((__VA_ARGS__)) >
#define LOKI_TYPELIST_61(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_60_((__VA_ARGS__)) >
#define LOKI_TYPELIST_62(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_61_((__VA_ARGS__)) >
#define LOKI_TYPELIST_63(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_62_((__VA_ARGS__)) >
#define LOKI_TYPELIST_64(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_63_((__VA_ARGS__)) >
#define LOKI_TYPELIST_65(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_64_((__VA_ARGS__)) >
#define LOKI_TYPELIST_66(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_65_((__VA_ARGS__)) >
#define LOKI_TYPELIST_67(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_66_((__VA_ARGS__)) >
#define LOKI_TYPELIST_68(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_67_((__VA_ARGS__)) >
#define LOKI_TYPELIST_69(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_68_((__VA_ARGS__)) >
#define LOKI_TYPELIST_70(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_69_((__VA_ARGS__)) >
#define LOKI_TYPELIST_71(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_70_((__VA_ARGS__)) >
#define LOKI_TYPELIST_72(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_71_((__VA_ARGS__)) >
#define LOKI_TYPELIST_73(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_72_((__VA_ARGS__)) >
#define LOKI_TYPELIST_74(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_73_((__VA_ARGS__)) >
#define LOKI_TYPELIST_75(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_74_((__VA_ARGS__)) >
#define LOKI_TYPELIST_76(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_75_((__VA_ARGS__)) >
#define LOKI_TYPELIST_77(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_76_((__VA_ARGS__)) >
#define LOKI_TYPELIST_78(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_77_((__VA_ARGS__)) >
#define LOKI_TYPELIST_79(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_78_((__VA_ARGS__)) >
#define LOKI_TYPELIST_80(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_79_((__VA_ARGS__)) >
#define LOKI_TYPELIST_81(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_80_((__VA_ARGS__)) >
#define LOKI_TYPELIST_82(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_81_((__VA_ARGS__)) >
#define LOKI_TYPELIST_83(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_82_((__VA_ARGS__)) >
#define LOKI_TYPELIST_84(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_83_((__VA_ARGS__)) >
#define LOKI_TYPELIST_85(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_84_((__VA_ARGS__)) >
#define LOKI_TYPELIST_86(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_85_((__VA_ARGS__)) >
#define LOKI_TYPELIST_87(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_86_((__VA_ARGS__)) >
#define LOKI_TYPELIST_88(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_87_((__VA_ARGS__)) >
#define LOKI_TYPELIST_89(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_88_((__VA_ARGS__)) >
#define LOKI_TYPELIST_90(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_89_((__VA_ARGS__)) >
#define LOKI_TYPELIST_91(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_90_((__VA_ARGS__)) >
#define LOKI_TYPELIST_92(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_91_((__VA_ARGS__)) >
#define LOKI_TYPELIST_93(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_92_((__VA_ARGS__)) >
#define LOKI_TYPELIST_94(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_93_((__VA_ARGS__)) >
#define LOKI_TYPELIST_95(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_94_((__VA_ARGS__)) >
#define LOKI_TYPELIST_96(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_95_((__VA_ARGS__)) >
#define LOKI_TYPELIST_97(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_96_((__VA_ARGS__)) >
#define LOKI_TYPELIST_98(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_97_((__VA_ARGS__)) >
#define LOKI_TYPELIST_99(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_98_((__VA_ARGS__)) >
#define LOKI_TYPELIST_100(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_99_((__VA_ARGS__)) >
#define LOKI_TYPELIST_101(T1, ...) ::Loki::Typelist<T1, LOKI_TYPELIST_100_((__VA_ARGS__)) >

#endif //LOKI_DISABLE_TYPELIST_MACROS

#endif // end file guardian

